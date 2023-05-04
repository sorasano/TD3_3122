#include "Light.h"

void Light::Initialize()
{
	//�ˉe�ϊ�
	//matProjection_ = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(45.0f),			//�㉺��p45�x
	//	(float)window_width / window_height,//�A�X�y�N�g��(��ʉ���/��ʗ���)
	//	0.1f, 1000.0f						//�O�[�A���[
	//);

	matProjection_ = XMMatrixOrthographicLH(
		40,40,1.0f,100.0f);

	//�s��v�Z
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}

void Light::Update()
{
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}
