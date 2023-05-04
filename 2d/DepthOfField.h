#pragma once
#include "d3dx12.h"
#include "array"
#include "DirectXMath.h"
#include "DirectXTex.h"
#include "vector"

class DepthOfField
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	 //�萔
	//SRV�̍ő��
	static const size_t kMaxSrvCount = 2056;

public:	//�T�u�N���X
	//�萔�o�b�t�@
	struct ConstBuffMaterial
	{
		//�F
		XMFLOAT4 color;
		//�E�B���h�E
		XMFLOAT2 window;
	};
	//�萔�o�b�t�@2
	struct ConstBuffTransform
	{
		XMMATRIX mat;	//3D�ϊ��s��
	};
	//�萔�o�b�t�@3
	struct ConstBuffStatus
	{
		//�t�H�[�J�X
		float focus;
		//f�l
		float fnumber;
		//����
		float strength;
	};
	//���_�f�[�^�p�\����
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

public:	//�����o�֐�
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//�p�C�v���C���ݒ�A�쐬
	void CreateGraphicsPipeLine();

	//�`��O����
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	//�`��㏈��
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

public:	//�ÓI�����o�֐�
	static void SetDevice(ID3D12Device* device) { DepthOfField::device = device; }

public:	//�Z�b�^�[
	//�A���t�@�l
	void SetAlpha(float alpha) { color.w = alpha; }
	//�F
	void SetColor(XMFLOAT3 c) { color.x = c.x; color.y = c.y; color.z = c.z; }
	//���W
	void SetPosition(XMFLOAT2 pos) { position = pos; }
	//�p�x
	void SetRotation(float rot) { rotation = rot; }
	//�X�P�[��
	void SetScale(XMFLOAT2 sca) { scale = sca; }
	//�t�H�[�J�X
	void SetFocus(float f) { focus = f; }
	//F�l
	void SetFNumber(float f) { fNumber = f; }
	//����
	void SetStrength(float s) { strength = s; }

private:	//�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature>rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState>pipelinestate;
	//��ʃN���A�J���[
	static const float clearColor[4];

private:	//�����o�ϐ�
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//���_�f�[�^
	Vertex vertices[6];
	Vertex* vertMap = nullptr;
	//�萔�o�b�t�@ �}�e���A��
	ComPtr<ID3D12Resource>constBuffMaterial;
	ConstBuffMaterial* constMapMaterial = nullptr;
	//�萔�o�b�t�@ �ό`�s��
	ComPtr<ID3D12Resource>constBuffTransform;
	ConstBuffTransform* constMapTransform = nullptr;
	//�萔�o�b�t�@ �t�H�[�J�X�Ƃ�
	ComPtr<ID3D12Resource>constBuffStatus;
	ConstBuffStatus* constMapStatus = nullptr;
	//�e�N�X�`���̐F
	XMFLOAT4 color = { 1,1,1,1 };
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource>textureBuff;
	//�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap;

	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource>depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>depthSRVHeap;
	ComPtr<ID3D12DescriptorHeap>descSRVHeap;

private:
	float rotation = 0;
	XMFLOAT2 position = { 0,0 };
	XMFLOAT2 scale = { 100.0f,100.0f };
	//�t�H�[�J�X
	float focus = 0.5f;
	//F�l
	float fNumber = 0.5f;
	//����
	float strength = 10.0f;
};

