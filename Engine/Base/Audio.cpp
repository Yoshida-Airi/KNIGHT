#include "Audio.h"

using namespace AobaraEngine;

Audio* Audio::GetInstance()
{
	if (instance_ == NULL)
	{
		instance_ = new Audio;
	}
	return instance_;
}

Audio::~Audio()
{

}

void Audio::Initialize()
{
	//xAuido2エンジンのインスタンス生成
	HRESULT hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//マスターボイスの生成
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
}

void Audio::Update()
{
}

void Audio::Draw()
{
}

uint32_t Audio::SoundLoadWave(const char* filename)
{
	uint32_t index = 0;

	for (int i = 0; i < kMaxAudio_; i++)
	{
		//同じ音声があった場合
		if (audios_[i].filename == filename)
		{
			return audios_[i].textureHandle;
		}

		if (isUsedAudio_[i] == false) {
			index = i;
			isUsedAudio_[i] = true;
			break;
		}
	}

	//indexが不正な値だった場合止める
	if (index < 0 || kMaxAudio_ <= index) {
		//MaxSpriteより多い
		assert(false);
	}

	//ファイルオープン
	std::ifstream file;
	file.open(filename, std::ios_base::binary);
	assert(file.is_open());
	//.wavデータの読み込み
	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}
	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		//読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}
	//Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//ファイルクローズ
	file.close();

	//読み込んだ音声データをreturn
	audios_.at(index).wfex = format.fmt;
	audios_.at(index).pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	audios_.at(index).bufferSize = data.size;
	audios_.at(index).filename = filename;
	audios_.at(index).textureHandle = index;
	

	return index;

}

void Audio::SoundUoload(uint32_t soundData)
{
	//バッファのメモリを解放
	delete[]audios_.at(soundData).pBuffer;

	audios_.at(soundData).pBuffer = 0;
	audios_.at(soundData).bufferSize = 0;
	audios_.at(soundData).wfex = {};
}

void Audio::SoundPlayWave(const uint32_t& soundHandle, bool isRoop)
{

	HRESULT hr;

	//波形フォーマットをもとにSourceVoiceの生成
	hr = xAudio2_->CreateSourceVoice(&audios_.at(soundHandle).pSourceVoice, &audios_.at(soundHandle).wfex);
	assert(SUCCEEDED(hr));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = audios_.at(soundHandle).pBuffer;
	buf.AudioBytes = audios_.at(soundHandle).bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (isRoop == true)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//波形データの再生
	hr = audios_.at(soundHandle).pSourceVoice->SubmitSourceBuffer(&buf);
	hr = audios_.at(soundHandle).pSourceVoice->Start();



}

void Audio::SoundStopWave(const uint32_t& soundHandle)
{

	audios_[soundHandle].pSourceVoice->Stop();
}

Audio* Audio::instance_ = NULL;