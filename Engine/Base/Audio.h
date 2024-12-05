/**
*	@file Audio.h
*	@brief 音クラスヘッダ
*/

#pragma once
#include<xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

#include<fstream>
#include<wrl.h>
#include<cassert>
#include<array>


namespace AobaraEngine
{
	struct ChunkHeader
	{
		char id[4];
		int32_t size;
	};

	struct  RiffHeader
	{
		ChunkHeader chunk;
		char type[4];
	};

	struct  FormatChunk
	{
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

	struct SoundData
	{
		//波形フォーマット
		WAVEFORMATEX wfex;
		//バッファの先頭アドレス
		BYTE* pBuffer;
		//バッファのサイズ
		unsigned int bufferSize;
		IXAudio2SourceVoice* pSourceVoice;

		std::string filename{};
		uint32_t textureHandle;
	};

	/**
	*   @class Audio
	*	@brief  音クラス
	*/
	class Audio
	{
	public:

		static Audio* GetInstance();
		~Audio();
		void Initialize();
		void Update();
		void Draw();

		/// <summary>
		/// 音声ファイルの読み込み
		/// </summary>
		/// <param name="filename"></param>
		/// <returns></returns>
		uint32_t SoundLoadWave(const char* filename);

		/// <summary>
		/// 音声ファイルの解放
		/// </summary>
		/// <param name="soundData"></param>
		void SoundUoload(uint32_t soundData);

		/// <summary>
		/// 音声ファイルの実行
		/// </summary>
		/// <param name="xAudio2"></param>
		/// <param name="soundData"></param>
		void SoundPlayWave(const uint32_t& soundData, bool isRoop);

		void SoundStopWave(const uint32_t& soundHandle);

	private:
		static const size_t kMaxAudio_ = 256;	//最大テクスチャ数

		Microsoft::WRL::ComPtr<IXAudio2>xAudio2_;
		IXAudio2MasteringVoice* masterVoice_;

		bool isUsedAudio_[kMaxAudio_];
		std::array<SoundData, kMaxAudio_> audios_;
		static Audio* instance_;

		bool isroop_;	//false::ループ再生しない　true::ループ再生する

	};
}