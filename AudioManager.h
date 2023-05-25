#pragma once

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>
#include <array>
#include <cstdint>
#include <set>
#include <string>
#include <map>
#include <combaseapi.h>
#include <Windows.h>
#include <cassert>
#include <iterator>
#include <wrl.h>


//�`�����N�w�b�_
struct ChunkHeader {
	char id[4];		//�`�����N����ID
	int32_t size;	//�`�����N�T�C�Y
};

//RIFF�w�b�_�`�����N
struct RiffHeader {
	ChunkHeader chunk;	//"RIFF"
	char type[4];		//"WAVE"
};

//FMT�`�����N
struct FormatChunk {
	ChunkHeader chunk;	//"fmt"
	WAVEFORMATEX fmt;	//�g�`�t�H�[�}�b�g
};

class AudioManager final
{
public:
	AudioManager();
	~AudioManager();


public:

	//�R�s�[�R���X�g���N�^����
	AudioManager(const AudioManager& obj) = delete;
	//������Z�q�𖳌�
	AudioManager& operator=(const AudioManager& obj) = delete;

	//�C���X�^���X�A�N�Z�X��p�֐�
	static AudioManager* GetInstance();

	static Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	static IXAudio2MasteringVoice* masterVoice_;


	// ������
	static void StaticInitialize();
	//�����ǂݍ���
	void SoundLoadWave(const char* filename);
	//�����Đ�
	void SoundPlayWave(bool loop = false, float volume = 1.0f);
	// ������~
	void StopWave();

	//�����f�[�^���
	void SoundUnload();


private:
	//�����f�[�^
	struct SoundData {
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex;
		//�o�b�t�@�̐擪�A�h���X
		BYTE* pBuffer;
		//�o�b�t�@�T�C�Y
		unsigned int bufferSize;
	};

	SoundData soundData_;

	IXAudio2SourceVoice* pSourceVoice = nullptr;

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};

	bool isPlay = false;


};