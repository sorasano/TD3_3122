#include "Message.h"

Message* Message::GetInstance()
{
	static Message instance;
	return &instance;
}

bool Message::Update()
{
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // ���b�Z�[�W������H
	{
		TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);  // �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	if (msg.message == WM_QUIT) // �I�����b�Z�[�W�������烋�[�v�𔲂���
	{
		return true;
	}
	return false;

	////X�{�^���ŏI�����b�Z����������Q�[�����[�v�𔲂��� 
	//if (msg.message == WM_QUIT)
	//{
	//	IDXGIDebug* giDebugInterface = nullptr;

	//	if (giDebugInterface == nullptr)
	//	{
	//		//�쐬
	//		typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
	//		HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	//		if (hDll != 0)
	//		{
	//			fPtr DXGIGetDebugInterface =
	//				(fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

	//			DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&giDebugInterface);

	//			//�o��
	//			giDebugInterface->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_DETAIL);
	//		}
	//	}
	//	return 1;
	//}
}

bool Message::CheckError()
{
	//IDXGIDebug* giDebugInterface = nullptr;

	//if (giDebugInterface == nullptr)
	//{
	//	//�쐬
	//	typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
	//	HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
	//	if (hDll != 0)
	//	{
	//		fPtr DXGIGetDebugInterface =
	//			(fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

	//		DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&giDebugInterface);

	//		//�o��
	//		giDebugInterface->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_DETAIL);
	//	}
	//}
	return 1;

}
