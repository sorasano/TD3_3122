#include "DirectionalLight.h"

using namespace DirectX;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* DirectionalLight::device = nullptr;

void DirectionalLight::StaticInitialize(ID3D12Device* device)
{
	//�ď������`�F�b�N 
	assert(!DirectionalLight::device); 
	//nullptr�`�F�b�N 
	assert(device);
	// �ÓI�����o�ϐ��̃Z�b�g 
	DirectionalLight::device = device;
}

DirectionalLight* DirectionalLight::Create()
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐� 
	DirectionalLight* instance = new DirectionalLight();
	//������
	instance->Initialize();
	//���������C���X�^���X��Ԃ�
	return instance;

}

void DirectionalLight::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�w�f�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap); 
	if (SUCCEEDED(result)) 
	{
		constMap->lightv = -lightdir;
		constMap->lightcolor = lightcolor; 
		constBuff->Unmap(0, nullptr);
	}
}

void DirectionalLight::SetLightDir(const XMVECTOR& lightdir)
{
	// ���K�����ăZ�b�g
	this->lightdir = XMVector3Normalize(lightdir);
	dirty = true;
}

void DirectionalLight::SetLightColor(const XMFLOAT3& lightcolor)
{
	this->lightcolor = lightcolor;
	dirty = true;

}

void DirectionalLight::Initialize()
{
	HRESULT result;

	//�萔�o�b�t�@�̐���
	CD3DX12_HEAP_PROPERTIES v1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC v2 = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
	result = device->CreateCommittedResource(
		&v1,
		D3D12_HEAP_FLAG_NONE,
		&v2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	//�萔�o�b�t�@�փf�[�^�]��
	TransferConstBuffer();
}

void DirectionalLight::Update()
{
	// �l�̍X�V���������������萔�o�b�t�@�ɓ]������
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}

}

void DirectionalLight::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}
