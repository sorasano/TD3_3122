#include "Shadow.h"
#include "WinApp.h"

void Shadow::SetLightPos(const XMFLOAT3& lightPos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	//�ˉe�ϊ�
	lightViewProj = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(45.0f),			//�㉺��p45�x
		(float)window_width / window_height,//�A�X�y�N�g��(��ʉ���/��ʗ���)
		0.1f, 1000.0f						//�O�[�A���[
	);

	//�s��v�Z
	lightView = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&lightPos), 
		DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&up));
}
