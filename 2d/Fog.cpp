#include "Fog.h"
#include "string.h"
#include "WinApp.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")
#define PI 3.1415

using namespace Microsoft::WRL;

ID3D12Device* Fog::device = nullptr;
ComPtr<ID3D12RootSignature>Fog::rootsignature;
ComPtr<ID3D12PipelineState>Fog::pipelinestate;
const float Fog::clearColor[4] = { 0.25f,0.5f, 0.1f, 0.0f };

void Fog::Initialize()
{
	HRESULT result;

	SetScale({ window_width,window_height });
	SetPosition({ 0.0f, 0.0 });

	//���_�f�[�^
	vertices[0] = { {    0.0f, scale.y, 0.0f },{ 0.0f,1.0f } };	//����
	vertices[1] = { {    0.0f,    0.0f, 0.0f },{ 0.0f,0.0f } };	//����
	vertices[2] = { { scale.x, scale.y, 0.0f },{ 1.0f,1.0f } };	//�E��
	vertices[3] = { { scale.x, scale.y, 0.0f },{ 1.0f,1.0f } };	//�E��
	vertices[4] = { {    0.0f,    0.0f, 0.0f },{ 0.0f,0.0f } };	//����
	vertices[5] = { { scale.x,    0.0f, 0.0f },{ 1.0f,0.0f } };	//�E��

	//���_�f�[�^�̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(vertices));

	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};	//�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	/*Vertex* vertMap = nullptr;*/
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];	//���W���R�s�[
	}
	//�q���������
	vertBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	//���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(Vertex);

	//�萔�o�b�t�@�̐ݒ�
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES v0{};
	v0.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC v1{};
	v1.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	v1.Width = (sizeof(ConstBuffMaterial) + 0xff) & ~0xff;
	v1.Height = 1;
	v1.DepthOrArraySize = 1;
	v1.MipLevels = 1;
	v1.SampleDesc.Count = 1;
	v1.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&v0,
		D3D12_HEAP_FLAG_NONE,
		&v1,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�}�b�s���O
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));

	//�s��p�萔�o�b�t�@�ݒ�
	D3D12_HEAP_PROPERTIES v2{};
	v2.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC v3{};
	v3.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	v3.Width = (sizeof(ConstBuffTransform) + 0xff) & ~0xff;
	v3.Height = 1;
	v3.DepthOrArraySize = 1;
	v3.MipLevels = 1;
	v3.SampleDesc.Count = 1;
	v3.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&v2,
		D3D12_HEAP_FLAG_NONE,
		&v3,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);

	//�萔�o�b�t�@�}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));
	constMapTransform->mat = DirectX::XMMatrixIdentity();

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		window_width,
		(UINT)window_height,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_HEAP_PROPERTIES a0 = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	CD3DX12_CLEAR_VALUE a1 = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);
	//�e�N�X�`���o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&a0,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&a1,
		IID_PPV_ARGS(&textureBuff)
	);
	assert(SUCCEEDED(result));

	//�e�N�X�`�������p�ݒ�
	const UINT pixelCount = window_width * window_height;
	const UINT rowPitch = sizeof(UINT) * window_width;
	const UINT depthPitch = rowPitch * window_height;
	//�摜�C���[�W
	UINT* img = new UINT[pixelCount];
	for (int i = 0; i < pixelCount; i++)
	{
		img[i] = 0xff0000ff;
	}

	//�e�N�X�`���o�b�t�@�Ƀf�[�^���M
	result = textureBuff->WriteToSubresource(
		0,
		nullptr,
		img,
		rowPitch,
		depthPitch
	);
	assert(SUCCEEDED(result));
	delete[] img;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = 2;

	//�ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//�V�F�[�_���\�[�X�r���[�̍쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	//�n���h���̎w���ʒu�ɃV�F�[�_���\�[�X�r���[�쐬
	device->CreateShaderResourceView(textureBuff.Get(), &srvDesc, srvHeap->GetCPUDescriptorHandleForHeapStart());


	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc{};
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDesc.NumDescriptors = 2;
	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));


	//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC renderDesc{};
	renderDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//�f�X�N���v�^�q�[�v��RTV�쐬
	device->CreateRenderTargetView(
		textureBuff.Get(),
		&renderDesc,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart()
	);


	//�[�x�o�b�t�@�̍쐬
	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = window_width;
	depthResDesc.Height = window_height;
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//�[�x�l�p�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	//�N���A�o�b�t�@�r���[
	/*D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;*/

	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		nullptr,
		IID_PPV_ARGS(&depthBuff)
	);
	assert(SUCCEEDED(result));

	//�ŃX�N���v�^�p�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 2;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV));

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	D3D12_CPU_DESCRIPTOR_HANDLE handle = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		handle
	);
	assert(SUCCEEDED(result));

	//�[�x�e�N�X�`���p�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 2;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	result = device->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(&depthSRVHeap)
	);
	assert(SUCCEEDED(result));

	//�V�F�[�_���\�[�X�r���[�̍쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC sResDesc{};
	sResDesc.Format = DXGI_FORMAT_R32_FLOAT;
	sResDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	sResDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	sResDesc.Texture2D.MipLevels = 1;
	D3D12_CPU_DESCRIPTOR_HANDLE handle2 = depthSRVHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateShaderResourceView(depthBuff.Get(), &sResDesc, 
		handle2);
}

void Fog::Update()
{
	std::copy(std::begin(vertices), std::end(vertices), vertMap);

	//�}�e���A��
	constMapMaterial->color = color;
	constMapMaterial->window.x = window_width;
	constMapMaterial->window.y = window_height;
	constMapMaterial->resolution = resolution;
	constMapMaterial->startDepth = startDepth;
	constMapMaterial->strength = strength;

	//�ό`�s��
	//���[���h�ϊ��s��
	XMMATRIX matWorld;
	matWorld = DirectX::XMMatrixIdentity();
	//��]
	XMMATRIX matRot;
	matRot = DirectX::XMMatrixIdentity();
	matRot *= DirectX::XMMatrixRotationZ(rotation);
	matWorld *= matRot;
	//���s�ړ�
	XMMATRIX matTrans;
	matTrans = DirectX::XMMatrixTranslation(position.x, position.y, 0.0f);
	matWorld *= matTrans;

	//����
	constMapTransform->mat = matWorld;
	//2D���W�ɕϊ�
	constMapTransform->mat.r[0].m128_f32[0] = 2.0f / window_width;
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / window_height;
	constMapTransform->mat.r[3].m128_f32[0] = -1.0f + (position.x / window_width) * 2;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f - (position.y / window_height) * 2;
}

//void ShadowMap::Draw0(ID3D12GraphicsCommandList* cmdList)
//{
//
//	//�p�C�v���C���A���[�g�V�O�l�`�����Z�b�g
//	cmdList->SetPipelineState(pipelinestate1.Get());
//	cmdList->SetGraphicsRootSignature(rootsignature1.Get());
//
//	//�v���~�e�B�u�`��̐ݒ�R�}���h
//	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
//	cmdList->IASetVertexBuffers(0, 1, &vbView);
//
//	//�萔�o�b�t�@�r���[�̐ݒ�R�}���h
//	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
//	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
//	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
//	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
//	//SRV�q�[�v�̐擪�n���h�����擾
//	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap.Get()->GetGPUDescriptorHandleForHeapStart();
//
//	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�ӂɐݒ�
//	cmdList->SetGraphicsRootDescriptorTable(2, srvGpuHandle);
//
//	D3D12_GPU_DESCRIPTOR_HANDLE handle = depthSRVHeap->GetGPUDescriptorHandleForHeapStart();
//	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//	cmdList->SetDescriptorHeaps(1, depthSRVHeap.GetAddressOf());
//	cmdList->SetGraphicsRootDescriptorTable(3, handle);
//
//	//�萔�o�b�t�@�r���[�̐ݒ�R�}���h
//	cmdList->SetGraphicsRootConstantBufferView(1, constBuffTransform->GetGPUVirtualAddress());
//
//	//�`��R�}���h
//	cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
//}

void Fog::Draw(ID3D12GraphicsCommandList* cmdList)
{

	//�p�C�v���C���A���[�g�V�O�l�`�����Z�b�g
	cmdList->SetPipelineState(pipelinestate.Get());
	cmdList->SetGraphicsRootSignature(rootsignature.Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//�萔�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap.Get()->GetGPUDescriptorHandleForHeapStart();

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�ӂɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

	cmdList->SetDescriptorHeaps(1, depthSRVHeap.GetAddressOf());
	D3D12_GPU_DESCRIPTOR_HANDLE handle = depthSRVHeap->GetGPUDescriptorHandleForHeapStart();
	cmdList->SetGraphicsRootDescriptorTable(3, handle);
	/*handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cmdList->SetGraphicsRootDescriptorTable(4, handle);*/

	//�萔�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffTransform->GetGPUVirtualAddress());

	//�`��R�}���h
	cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
}

void Fog::CreateGraphicsPipeLine0()
{
	HRESULT result;

	ID3DBlob* vsBlob = nullptr;	//���_�V�F�[�_�[�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr;	//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr;	//�G���[�I�u�W�F�N�g

	//���_�V�F�[�_�̓ǂݍ��� �R���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/Fog/FogVertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);

	//�G���[�̏ꍇ
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//�s�N�Z���V�F�[�_�̓ǂݍ��� �R���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/Fog/FogPixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);

	//�G���[�̏ꍇ
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//�V�F�[�_�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//�W���ݒ�

	//���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	//�J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//�|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true;	//�[�x�N���b�s���O��L����
	/*pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;*/

	//�u�����h�X�e�[�g
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//���ʂ̐ݒ�
	pipelineDesc.BlendState.RenderTarget[0].BlendEnable = false;
	//pipelineDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	//pipelineDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	//pipelineDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	////�A���t�@�u�����h
	//pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	//pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`��
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��ݒ�
	pipelineDesc.NumRenderTargets = 2;	//�`��Ώۂ�1
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//RGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8_UNORM;	//RGBA
	pipelineDesc.SampleDesc.Count = 1;	//1�s�N�Z���ɂ�1��T���v�����O

	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange0{};
	descriptorRange0.NumDescriptors = 1;
	descriptorRange0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange0.BaseShaderRegister = 0;
	descriptorRange0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRange1{};
	descriptorRange1.NumDescriptors = 1;
	descriptorRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange1.BaseShaderRegister = 1;

	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 2;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParameter[4] = {};
	//�萔�o�b�t�@0��
	rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[0].Descriptor.ShaderRegister = 0;
	rootParameter[0].Descriptor.RegisterSpace = 0;
	rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�萔�o�b�t�@1��
	rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[1].Descriptor.ShaderRegister = 1;
	rootParameter[1].Descriptor.RegisterSpace = 0;
	rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�e�N�X�`�����W�X�^0��
	rootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[2].DescriptorTable.pDescriptorRanges = &descriptorRange0;
	rootParameter[2].DescriptorTable.NumDescriptorRanges = 1;
	rootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�e�N�X�`�����W�X�^1��
	rootParameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[3].DescriptorTable.pDescriptorRanges = &descriptorRange1;
	rootParameter[3].DescriptorTable.NumDescriptorRanges = 1;
	rootParameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParameter;
	rootSignatureDesc.NumParameters = _countof(rootParameter);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootsignature.Get();

	//�p�C�v���C���X�e�[�g�̐���
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(pipelinestate.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
}

void Fog::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//���\�[�X�o���A��ύX
	CD3DX12_RESOURCE_BARRIER a0 = CD3DX12_RESOURCE_BARRIER::Transition(
		textureBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	cmdList->ResourceBarrier(1, &a0);

	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//�k�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	/*dsvHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);*/
	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//�r���[�|�[�g�̐ݒ�
	CD3DX12_VIEWPORT a1 = CD3DX12_VIEWPORT(0.0f, 0.0f, window_width, window_height);
	cmdList->RSSetViewports(1, &a1);
	//�V�U�[��`�̐ݒ�
	CD3DX12_RECT a2 = CD3DX12_RECT(0, 0, window_width, window_height);
	cmdList->RSSetScissorRects(1, &a2);


	//�S��ʃN���A
	cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	//�k�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void Fog::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//���\�[�X�o���A��ύX
	CD3DX12_RESOURCE_BARRIER a0 = CD3DX12_RESOURCE_BARRIER::Transition(
		textureBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	cmdList->ResourceBarrier(1, &a0);
}