#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//リソースの作成
	void TitleStage::CreateResourses()
	{
		wstring datadir;
		auto& app = App::GetApp();
		//サウンド
		app->GetDataDirectory(datadir);
		datadir += L"SE\\";
		auto SE = datadir + L"OK.wav";
		app->RegisterWav(L"OK", SE);
	}
	
	//	タイトルステージクラス
	void TitleStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 2.0f, -3.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();

	}

	//スプライトの作成
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

	// タイトルロゴ
	void TitleStage::CreateTitleLogo() {
		AddGameObject<TitleLogo>(L"TitleLogo_TX", false,
	Vec2(1100.0f, 400.0f), Vec2(0.0f, 50.0f));
		

	}

	//初期化
	void TitleStage::OnCreate() {
		CreateViewLight();
		//スプライトの作成
		CreateTitleSprite();

		CreateTitleLogo();
		m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
		m_AudioObjectPtr->AddAudioResource(L"OK");

	}

	//更新
	void TitleStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Aボタン
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