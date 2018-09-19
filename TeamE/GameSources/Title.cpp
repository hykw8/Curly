#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//���\�[�X�̍쐬
	void TitleStage::CreateResourses()
	{
		wstring datadir;
		auto& app = App::GetApp();
		//�T�E���h
		app->GetDataDirectory(datadir);
		datadir += L"SE\\";
		auto SE = datadir + L"OK.wav";
		app->RegisterWav(L"OK", SE);
	}
	
	//	�^�C�g���X�e�[�W�N���X
	void TitleStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 2.0f, -3.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();

	}

	//�X�v���C�g�̍쐬
	void TitleStage::CreateTitleSprite() {
		AddGameObject<TitleSprite>(L"TitleSprite_TX", false,
			Vec2(1500.0f, 900.0f), Vec2(0.0f, 0.0f));
		
		AddGameObject<PressAnyButton>
			(
				Vec2(0.0f, -300.0f),
				Vec2(0.4f, 0.4f),
				L"PushBottom2_TX"
				);


	}

	// �^�C�g�����S
	void TitleStage::CreateTitleLogo() {
		AddGameObject<TitleLogo>(L"TitleLogo_TX", false,
	Vec2(1100.0f, 400.0f), Vec2(0.0f, 50.0f));
		

	}

	//������
	void TitleStage::OnCreate() {
		CreateViewLight();
		//�X�v���C�g�̍쐬
		CreateTitleSprite();

		CreateTitleLogo();
		m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
		m_AudioObjectPtr->AddAudioResource(L"OK");

	}

	//�X�V
	void TitleStage::OnUpdate() {
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//A�{�^��
			if (CntlVec[0].wPressedButtons) {
				m_AudioObjectPtr->Start(L"OK", 0, 0.5f);
				AddGameObject<Fade>
					(
						Vec2(0.0f, 0.0f),
						Vec2(1280.0f, 800.0f),
						true
						);
				auto scene = App::GetApp()->GetScene<Scene>();
				//scene->m_StageName = L"StageSelect";
				scene->SetStageName(L"StageSelect");
				PostEvent(1.0f, GetThis<ObjectInterface>(), scene, L"ToStageSelect");
				SetUpdateActive(false);
			}
		}

	}
}