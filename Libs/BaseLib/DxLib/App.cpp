/*!
@file App.cpp
@brief �A�v���P�[�V�����N���X�B���͋@�퓙����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct AudioManager::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct AudioManager::Impl {
		HWND m_hWnd;
		bool m_audioAvailable;
		ComPtr<IXAudio2>    m_musicEngine;
		ComPtr<IXAudio2>    m_soundEffectEngine;
		IXAudio2MasteringVoice* m_musicMasteringVoice;
		IXAudio2MasteringVoice* m_soundEffectMasteringVoice;
		Impl(HWND hWnd) :
			m_hWnd(hWnd),
			m_audioAvailable{ false },
			m_musicMasteringVoice(nullptr),
			m_soundEffectMasteringVoice(nullptr)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class AudioManager;
	//--------------------------------------------------------------------------------------
	AudioManager::AudioManager(HWND hWnd) :
		pImpl(new Impl(hWnd))
	{
	}
	AudioManager::~AudioManager() {
		if (pImpl->m_soundEffectEngine) {
			pImpl->m_soundEffectEngine->StopEngine();
			pImpl->m_soundEffectEngine = nullptr;
		}
		if (pImpl->m_musicEngine) {
			pImpl->m_musicEngine->StopEngine();
			pImpl->m_musicEngine = nullptr;
		}
	}

	void AudioManager::AudioUnAvailableMassage() {
		MessageBox(pImpl->m_hWnd, L"�I�[�f�B�I���擾�ł��Ȃ������̂ŁA�����Ȃ��Ŏ��s���܂�", L"�x��", MB_OK);
	}


	void AudioManager::CreateDeviceIndependentResources()
	{
		UINT32 flags = 0;
		HRESULT hr = XAudio2Create(&pImpl->m_musicEngine, flags);
		if (FAILED(hr)) {
			pImpl->m_audioAvailable = false;
			AudioUnAvailableMassage();
			return;
		}

#if defined(_DEBUG)
		XAUDIO2_DEBUG_CONFIGURATION debugConfiguration = { 0 };
		debugConfiguration.BreakMask = XAUDIO2_LOG_ERRORS;
		debugConfiguration.TraceMask = XAUDIO2_LOG_ERRORS;
		pImpl->m_musicEngine->SetDebugConfiguration(&debugConfiguration);
#endif
		hr = pImpl->m_musicEngine->CreateMasteringVoice(&pImpl->m_musicMasteringVoice);
		if (FAILED(hr))
		{
			pImpl->m_audioAvailable = false;
			AudioUnAvailableMassage();
			return;
		}

		hr = XAudio2Create(&pImpl->m_soundEffectEngine, flags);
		if (FAILED(hr)) {
			pImpl->m_audioAvailable = false;
			AudioUnAvailableMassage();
			return;
		}
#if defined(_DEBUG)
		pImpl->m_soundEffectEngine->SetDebugConfiguration(&debugConfiguration);
#endif

		hr = pImpl->m_soundEffectEngine->CreateMasteringVoice(&pImpl->m_soundEffectMasteringVoice);
		if (FAILED(hr)) {
			pImpl->m_audioAvailable = false;
			AudioUnAvailableMassage();
			return;
		}
		pImpl->m_audioAvailable = true;
	}

	IXAudio2* AudioManager::GetMusicEngine()const
	{
		return pImpl->m_musicEngine.Get();
	}

	IXAudio2* AudioManager::GetSoundEffectEngine()const
	{
		return pImpl->m_soundEffectEngine.Get();
	}

	void AudioManager::SuspendAudio()
	{
		if (pImpl->m_audioAvailable)
		{
			pImpl->m_musicEngine->StopEngine();
			pImpl->m_soundEffectEngine->StopEngine();
		}
	}

	void AudioManager::ResumeAudio()
	{
		if (pImpl->m_audioAvailable)
		{
			ThrowIfFailed(
				pImpl->m_musicEngine->StartEngine(),
				L"���y�p�G���W���̃X�^�[�g�Ɏ��s���܂���",
				L"m_musicEngine->StartEngine()",
				L"AudioManager::CreateDeviceIndependentResources()"
			);
			ThrowIfFailed(
				pImpl->m_soundEffectEngine->StartEngine(),
				L"�T�E���h�p�G���W���̃X�^�[�g�Ɏ��s���܂���",
				L"m_soundEffectEngine->StartEngine()",
				L"AudioManager::CreateDeviceIndependentResources()"
			);
		}
	}

	bool AudioManager::IsAudioAvailable()const {
		return pImpl->m_audioAvailable;
	}



	//--------------------------------------------------------------------------------------
	//	struct AudioResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct AudioResource::Impl {
		WAVEFORMATEX m_WaveFormat;	//�E�F�u�t�H�[�}�b�g
		vector<byte> m_SoundData;	//�f�[�^
		wstring m_FileName;		//�t�@�C���ւ̃p�X
		Impl(const wstring& FileName) :
			m_FileName(FileName) {}
		~Impl() {}
		//�~���[�e�b�N�X
		std::mutex Mutex;

	};



	//--------------------------------------------------------------------------------------
	//	class AudioResource : public BaseResource;
	//	�p�r: �I�[�f�B�I���\�[�X�iwav�Ȃǁj�̃��b�s���O�N���X
	//--------------------------------------------------------------------------------------
	AudioResource::AudioResource(const wstring& FileName) :
		BaseResource(),
		pImpl(new Impl(FileName))
	{
		try {
			if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
				//�}�l�[�W���������Ȃ烊�^�[��
				return;
			}

			ThrowIfFailed(
				MFStartup(MF_VERSION),
				L"MediaFoundation�̏������Ɏ��s���܂���",
				L"MFStartup(MF_VERSION)",
				L"AudioResource::AudioResource()"
			);

			ComPtr<IMFSourceReader> reader;

			ThrowIfFailed(
				MFCreateSourceReaderFromURL(FileName.c_str(), nullptr, &reader),
				L"�T�E���h���[�_�[�̍쐬�Ɏ��s���܂���",
				L"MFCreateSourceReaderFromURL(FileName.c_str(),nullptr,&reader)",
				L"AudioResource::AudioResource()"
			);

			// Set the decoded output format as PCM.
			// XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
			// When using MediaFoundation, this sample always decodes into PCM.
			Microsoft::WRL::ComPtr<IMFMediaType> mediaType;

			ThrowIfFailed(
				MFCreateMediaType(&mediaType),
				L"���f�B�A�^�C�v�̍쐬�Ɏ��s���܂���",
				L"MFCreateMediaType(&mediaType)",
				L"AudioResource::AudioResource()"
			);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio),
				L"���f�B�AGUID�̐ݒ�Ɏ��s���܂���",
				L"mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)",
				L"AudioResource::AudioResource()"
			);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM),
				L"���f�B�A�T�uGUID�̐ݒ�Ɏ��s���܂���",
				L"mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)",
				L"AudioResource::AudioResource()"
			);

			ThrowIfFailed(
				reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get()),
				L"���[�_�[�ւ̃��f�B�A�^�C�v�̐ݒ�Ɏ��s���܂���",
				L"reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get())",
				L"AudioResource::AudioResource()"
			);


			// Get the complete WAVEFORMAT from the Media Type.
			Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;

			ThrowIfFailed(
				reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType),
				L"�o�͗p�̃��f�B�A�^�C�v�̐ݒ�Ɏ��s���܂���",
				L"reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType)",
				L"AudioResource::AudioResource()"
			);

			UINT32 size = 0;
			WAVEFORMATEX* waveFormat;

			ThrowIfFailed(
				MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size),
				L"�E�F�u�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���",
				L"MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size)",
				L"AudioResource::AudioResource()"
			);


			CopyMemory(&pImpl->m_WaveFormat, waveFormat, sizeof(pImpl->m_WaveFormat));
			CoTaskMemFree(waveFormat);

			PROPVARIANT propVariant;

			ThrowIfFailed(
				reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant),
				L"�A�g���r���[�g�̐ݒ�Ɏ��s���܂���",
				L"reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant)",
				L"AudioResource::AudioResource()"
			);

			// 'duration' is in 100ns units; convert to seconds, and round up
			// to the nearest whole byte.
			LONGLONG duration = propVariant.uhVal.QuadPart;
			unsigned int maxStreamLengthInBytes =
				static_cast<unsigned int>(
				((duration * static_cast<ULONGLONG>(pImpl->m_WaveFormat.nAvgBytesPerSec)) + 10000000) /
					10000000
					);

			pImpl->m_SoundData.resize(maxStreamLengthInBytes);

			ComPtr<IMFSample> sample;
			ComPtr<IMFMediaBuffer> mediaBuffer;
			DWORD flags = 0;

			int positionInData = 0;
			bool done = false;
			while (!done)
			{

				ThrowIfFailed(
					reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample),
					L"�T���v���[�̓ǂݍ��݂Ɏ��s���܂���",
					L"reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample)",
					L"AudioResource::AudioResource()"
				);

				if (sample != nullptr)
				{
					ThrowIfFailed(
						sample->ConvertToContiguousBuffer(&mediaBuffer),
						L"�T���v���[�̃R���o�[�g�Ɏ��s���܂���",
						L"sample->ConvertToContiguousBuffer(&mediaBuffer)",
						L"AudioResource::AudioResource()"
					);

					BYTE *audioData = nullptr;
					DWORD sampleBufferLength = 0;

					ThrowIfFailed(
						mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength),
						L"�o�b�t�@��Lock�Ɏ��s���܂���",
						L"mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength)",
						L"AudioResource::AudioResource()"
					);

					for (DWORD i = 0; i < sampleBufferLength; i++)
					{
						pImpl->m_SoundData[positionInData++] = audioData[i];
					}
				}
				if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
				{
					done = true;
				}
			}
		}
		catch (...) {
			throw;
		}
	}
	AudioResource::~AudioResource() {}

	const vector<byte>& AudioResource::GetSoundData()const {
		return pImpl->m_SoundData;
	}

	WAVEFORMATEX*  AudioResource::GetOutputWaveFormatEx()const {
		return &pImpl->m_WaveFormat;
	}



	//--------------------------------------------------------------------------------------
	//	struct EventDispatcher::Impl;
	//	�p�r: Impl�\����
	//--------------------------------------------------------------------------------------
	struct EventDispatcher::Impl {
		//�C�x���g�̃L���[
		list< shared_ptr<Event> > m_PriorityQ;
		map<wstring, vector<weak_ptr<ObjectInterface>>> m_EventInterfaceGroupMap;
		//
		//--------------------------------------------------------------------------------------
		//	void Discharge(
		//	const Event& event	//�C�x���g
		//	);
		//�p�r: �C�x���g�̑��M
		//�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void Discharge(const shared_ptr<Event>& event);
		Impl() {}
		~Impl() {}
	};

	void EventDispatcher::Impl::Discharge(const shared_ptr<Event>& event) {
		auto shptr = event->m_Receiver.lock();
		if (shptr) {
			//�󂯎肪�L��
			shptr->OnEvent(event);
		}
	}



	//--------------------------------------------------------------------------------------
	///	�C�x���g�z���N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	EventDispatcher::EventDispatcher():
		pImpl(new Impl())
	{}
	EventDispatcher::~EventDispatcher() {}

	void EventDispatcher::AddEventReceiverGroup(const wstring& GroupKey, const shared_ptr<ObjectInterface>& Receiver) {
		auto it = pImpl->m_EventInterfaceGroupMap.find(GroupKey);
		if (it != pImpl->m_EventInterfaceGroupMap.end()) {
			//�L�[��������
			it->second.push_back(Receiver);
		}
		else {
			//�O���[�v���Ȃ�
			vector<weak_ptr<ObjectInterface>> vec;
			pImpl->m_EventInterfaceGroupMap[GroupKey] = vec;
			pImpl->m_EventInterfaceGroupMap[GroupKey].push_back(Receiver);
		}
	}


	//�C�x���g��POST�i�L���[�ɓ����j
	void EventDispatcher::PostEvent(float Delay, const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info) {
		//�C�x���g�̍쐬 
		auto Ptr = make_shared<Event>(Delay, Sender, Receiver, MsgStr, Info);
		//�L���[�ɂ��߂�
		pImpl->m_PriorityQ.push_back(Ptr);
	}

	void EventDispatcher::PostEvent(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
		const wstring& MsgStr, shared_ptr<void>& Info) {
		//ReceiverKey�ɂ�鑊��̓���
		//�d���L�[�̌���
		auto it = pImpl->m_EventInterfaceGroupMap.find(ReceiverKey);
		if (it != pImpl->m_EventInterfaceGroupMap.end()) {
			//�L�[��������
			for (auto v : it->second) {
				auto shptr = v.lock();
				if (shptr) {
					//�C�x���g�̍쐬 
					auto Ptr = make_shared<Event>(0.0f, Sender, shptr, MsgStr, Info);
					//�L���[�ɂ��߂�
					pImpl->m_PriorityQ.push_back(Ptr);
				}
			}
		}
		//�L�[��������Ȃ��Ă��������Ȃ�
	}


	//�C�x���g��SEND�i�L���[�ɓ��ꂸ�ɂ��̂܂ܑ���j
	void EventDispatcher::SendEvent(const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info) {
		//�C�x���g�̍쐬 
		auto Ptr = make_shared<Event>(0.0f, Sender, Receiver, MsgStr, Info);
		//���M
		pImpl->Discharge(Ptr);
	}

	void EventDispatcher::SendEvent(const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
		const wstring& MsgStr, shared_ptr<void>& Info) {
		//ReceiverKey�ɂ�鑊��̓���
		//�d���L�[�̌���
		auto it = pImpl->m_EventInterfaceGroupMap.find(ReceiverKey);
		if (it != pImpl->m_EventInterfaceGroupMap.end()) {
			//�L�[��������
			for (auto v : it->second) {
				auto shptr = v.lock();
				if (shptr) {
					//�C�x���g�̍쐬 
					auto Ptr = make_shared<Event>(0.0f, Sender, shptr, MsgStr, Info);
					//�C�x���g�̑��o
					pImpl->Discharge(Ptr);
				}
			}
		}
		//�L�[��������Ȃ��Ă��������Ȃ�
	}


	void EventDispatcher::DispatchDelayedEvwnt() {
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto it = pImpl->m_PriorityQ.begin();
		while (it != pImpl->m_PriorityQ.end()) {
			(*it)->m_DispatchTime -= ElapsedTime;
			if ((*it)->m_DispatchTime <= 0.0f) {
				(*it)->m_DispatchTime = 0.0f;
				//���b�Z�[�W�̑��M
				pImpl->Discharge(*it);
				//�L���[����폜
				it = pImpl->m_PriorityQ.erase(it);
				//�폜��̃C�e���[�^���u�Ō�v��
				//�Ƃ��̓��[�v�𔲂���
				if (it == pImpl->m_PriorityQ.end()) {
					break;
				}
			}
			else {
				it++;
			}
		}
	}

	void EventDispatcher::ClearEventQ() {
		pImpl->m_PriorityQ.clear();
	}





	//--------------------------------------------------------------------------------------
	//	class App;
	//	�p�r: �A�v���P�[�V�����N���X
	//--------------------------------------------------------------------------------------
	//static�ϐ�����
	unique_ptr<App, App::AppDeleter> App::m_App;
	//�\�z
	App::App(HINSTANCE hInstance, HWND hWnd, bool FullScreen, UINT Width, UINT Height) :
		m_hInstance{ hInstance },
		m_hWnd{ hWnd },
		m_FullScreen{ FullScreen },
		m_GameWidth{ Width },
		m_GameHeight{ Height },
		m_Timer(),
		m_ScriptsDirActive(false)
	{
		try {
			//��f�B���N�g���̐ݒ�
			//���΃p�X�ɂ���ƁA�t�@�C���_�C�A���O�ŃJ�����g�p�X�������̂�
			//��΃p�X�w��
			wchar_t Modulebuff[MAX_PATH];
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];

			::ZeroMemory(Modulebuff, sizeof(Modulebuff));
			::ZeroMemory(Drivebuff, sizeof(Drivebuff));
			::ZeroMemory(Dirbuff, sizeof(Dirbuff));
			::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
			::ZeroMemory(Extbuff, sizeof(Extbuff));

			//���W���[�����i�v���O�����t�@�C�����j�𓾂�
			if (!::GetModuleFileName(nullptr, Modulebuff, sizeof(Modulebuff))) {
				throw BaseException(
					L"���W���[�����擾�ł��܂���B",
					L"if(!::GetModuleFileName())",
					L"App::App()"
				);
			}
			m_wstrModulePath = Modulebuff;
			//���W���[��������A�e�u���b�N�ɕ�����
			_wsplitpath_s(Modulebuff,
				Drivebuff, _MAX_DRIVE,
				Dirbuff, _MAX_DIR,
				FileNamebuff, _MAX_FNAME,
				Extbuff, _MAX_EXT);

			//�h���C�u���̎擾
			m_wstrDir = Drivebuff;
			//�f�B���N�g�����̎擾
			m_wstrDir += Dirbuff;
			//media�f�B���N�g����T��
			m_wstrDataPath = m_wstrDir;
			m_wstrDataPath += L"media";
			//�܂��A���s�t�@�C���Ɠ����f�B���N�g����T��
			DWORD RetCode;
			RetCode = GetFileAttributes(m_wstrDataPath.c_str());
			if (RetCode == 0xFFFFFFFF) {
				//���s����
				m_wstrDataPath = m_wstrDir;
				m_wstrDataPath += L"..\\media";
				RetCode = GetFileAttributes(m_wstrDataPath.c_str());
				if (RetCode == 0xFFFFFFFF) {
					//�Ăю��s����
					throw BaseException(
						L"media�f�B���N�g�����m�F�ł��܂���B",
						L"if(RetCode == 0xFFFFFFFF)",
						L"App::App()"
					);
				}
				else {
					m_wstrDataPath += L"\\";
					//���΃p�X�̐ݒ�
					m_wstrRelativeDataPath = L"..\\media\\";
				}
			}
			else {
				m_wstrDataPath += L"\\";
				//���΃p�X�̐ݒ�
				m_wstrRelativeDataPath = L"media\\";
			}
			m_wstrShadersPath = m_wstrDataPath + L"Shaders\\";
			m_wstrRelativeShadersPath = m_wstrRelativeDataPath + L"Shaders\\";

			//Scripts�f�B���N�g����T��
			m_wstrScriptsPath = m_wstrDir;
			m_wstrScriptsPath += L"scripts";
			//�܂��A���s�t�@�C���Ɠ����f�B���N�g����T��
			RetCode = GetFileAttributes(m_wstrScriptsPath.c_str());
			if (RetCode == 0xFFFFFFFF) {
				//���s����
				m_wstrScriptsPath = m_wstrDir;
				m_wstrScriptsPath += L"..\\scripts";
				RetCode = GetFileAttributes(m_wstrDataPath.c_str());
				if (RetCode == 0xFFFFFFFF) {
					//�Ăю��s����
					//Scripts�f�B���N�g���͕K�{�ł͂Ȃ��̂ōĂ�
					//���s�t�@�C���Ɠ����f�B���N�g���ɐݒ�
					m_ScriptsDirActive = false;
					m_wstrScriptsPath = L"";
					m_wstrScriptsPath += L"";
				}
				else {
					m_ScriptsDirActive = true;
					m_wstrScriptsPath += L"\\";
					//���΃p�X�̐ݒ�
					m_wstrRelativeScriptsPath = L"..\\scripts\\";
				}
			}
			else {
				m_ScriptsDirActive = true;
				m_wstrScriptsPath += L"\\";
				//���΃p�X�̐ݒ�
				m_wstrRelativeScriptsPath = L"scripts\\";
			}

			//Assets�f�B���N�g����T��
			m_wstrRelativeAssetsPath = m_wstrDir;
			m_wstrRelativeAssetsPath += L"..\\..\\Assets";
			//���΃f�B���N�g����T��
			RetCode = GetFileAttributes(m_wstrRelativeAssetsPath.c_str());
			if (RetCode == 0xFFFFFFFF) {
				//���s����
				//�A�Z�b�g�f�B���N�g�������f�B�A�f�B���N�g���ɂ���
				m_wstrRelativeAssetsPath = m_wstrRelativeDataPath;
			}
			else {
				//��������
				m_wstrRelativeAssetsPath += L"\\";
			}



			////�f�o�C�X���\�[�X�̍\�z
			m_DeviceResources = shared_ptr<DeviceResources>(new DeviceResources(hWnd, FullScreen, Width, Height));
			//�I�[�f�B�I�}�l�[�W���̎擾
			GetAudioManager();
			//�C�x���g�z���N���X
			m_EventDispatcher = make_shared<EventDispatcher>();
			//�����̏�����
			srand((unsigned)time(nullptr));

		}
		catch (...) {
			throw;
		}
	}

	//�V���O���g���\�z
	unique_ptr<App, App::AppDeleter>& App::CreateApp(HINSTANCE hInstance, HWND hWnd,
		bool FullScreen, UINT Width, UINT Height, bool ShadowActive) {
		try {
			if (m_App.get() == 0) {
				//�������g�̍\�z
				m_App.reset(new App(hInstance, hWnd, FullScreen, Width, Height));
				m_App->AfterInitContents(ShadowActive);

			}
			return m_App;
		}
		catch (...) {
			throw;
		}
	}

	//�V���O���g���A�N�Z�T
	unique_ptr<App, App::AppDeleter>& App::GetApp() {
		try {
			if (m_App.get() == 0) {
				throw BaseException(
					L"�A�v���P�[�V�������܂��쐬����Ă܂���",
					L"if (m_App.get() == 0)",
					L"App::GetApp()"
				);
			}
			return m_App;
		}
		catch (...) {
			throw;
		}

	}

	bool App::AppCheck() {
		if (m_App.get() == 0) {
			return false;
		}
		return true;
	}

	//�����j��
	void App::DeleteApp() {
		if (m_App.get()) {
			m_App.reset();
		}
	}

	// �I�[�f�B�I�}�l�[�W���̎擾
	unique_ptr<AudioManager>& App::GetAudioManager() {
		try {
			if (m_AudioManager.get() == 0) {
				m_AudioManager.reset(new AudioManager(m_hWnd));
				m_AudioManager->CreateDeviceIndependentResources();

			}
			return m_AudioManager;
		}
		catch (...) {
			throw;
		}
	}

	void App::AfterInitContents(bool ShadowActive) {
		if (!m_DeviceResources) {
			throw BaseException(
				L"�f�o�C�X������������Ă��܂���",
				L"if (!m_DeviceResources)",
				L"App::AfterInitContents()"
			);
		}
		m_DeviceResources->AfterInitContents(ShadowActive);
	}


	void App::RegisterResource(const wstring& Key, const shared_ptr<BaseResource>& ResObj) {
		try {
			if (Key == L"") {
				throw BaseException(
					L"�L�[���󔒂ł�",
					L"if(Key == L\"\")",
					L"App::RegisterResource()"
				);
			}
			if (!ResObj) {
				throw BaseException(
					L"���\�[�X���s��ł�",
					L"if(!pResObj)",
					L"App::RegisterResource()"
				);
			}
			map<wstring, shared_ptr<BaseResource> >::iterator it;
			//�d���|�C���^�̌���
			for (it = m_ResMap.begin(); it != m_ResMap.end(); it++) {
				if (it->second == ResObj) {
					//�|�C���^���d�����Ă��Ă��A�L�[�������Ȃ�
					//���łɓo�^����Ă���̂Ń��^�[��
					if (it->first == Key) {
						return;
					}
					wstring keyerr = Key;
					throw BaseException(
						L"���łɂ��̃��\�[�X�͓o�^����Ă��܂�",
						keyerr,
						L"App::RegisterResource()"
					);
				}
			}
			//�d���L�[�̌���
			it = m_ResMap.find(Key);
			if (it != m_ResMap.end()) {
				//�w��̖��O����������
				//��O����
				wstring keyerr = Key;
				throw BaseException(
					L"���łɂ��̃L�[�͓o�^����Ă��܂�",
					keyerr,
					L"App::RegisterResource()"
				);
			}
			else {
				//������Ȃ�
				//���\�[�X�y�A�̒ǉ�
				m_ResMap[Key] = ResObj;
			}
		}
		catch (...) {
			throw;
		}
	}

	//�e�N�X�`���̓o�^(�����L�[�̃e�N�X�`�����Ȃ���΃t�@�C�����ō쐬���A�o�^)
	//�������O�̃e�N�X�`��������΂��̃|�C���^��Ԃ�
	shared_ptr<TextureResource> App::RegisterTexture(const wstring& Key, const wstring& TextureFileName, const wstring& TexType) {
		if (CheckResource<TextureResource>(Key)) {
			return GetResource<TextureResource>(Key);
		}
		//
		auto PtrTexture = TextureResource::CreateTextureResource(TextureFileName, TexType);
		RegisterResource(Key, PtrTexture);
		return PtrTexture;
	}


	//Wav�̓o�^(�����L�[��Wav���Ȃ���΃t�@�C�����ō쐬���A�o�^)
	//�������O��Wav������΂��̃|�C���^��Ԃ�
	shared_ptr<AudioResource> App::RegisterWav(const wstring& Key, const wstring& WavFileName) {
		if (CheckResource<AudioResource>(Key)) {
			return GetResource<AudioResource>(Key);
		}
		//
		auto PtrWav = ObjectFactory::Create<AudioResource>(WavFileName);
		RegisterResource(Key, PtrWav);
		return PtrWav;
	}

	void App::UpdateDraw(unsigned int SyncInterval) {
		if (!m_SceneInterface) {
			//�V�[�����������Ȃ�
			throw BaseException(
				L"�V�[��������܂���",
				L"if(!m_SceneInterface)",
				L"App::UpdateDraw()"
			);
		}

		// �V�[�� �I�u�W�F�N�g���X�V���܂��B
		m_InputDevice.ResetControlerState();
		m_Timer.Tick([&]()
		{
			//�C�x���g�L���[�̑��M
			m_EventDispatcher->DispatchDelayedEvwnt();
			m_SceneInterface->OnUpdate();
		});
		// ����X�V�O�Ƀ����_�����O�͍s��Ȃ��B
		if (GetFrameCount() == 0)
		{
			return;
		}
		m_SceneInterface->OnDraw();
		// �o�b�N�o�b�t�@����t�����g�o�b�t�@�ɓ]��
		m_DeviceResources->Present(SyncInterval,0);
	}


	void App::OnMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	}





}
//end basecross
