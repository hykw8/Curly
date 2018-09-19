#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//	タイトルステージクラス
	void StageSelect::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 2.0f, -3.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		auto ptrSingleLight = CreateLight<SingleLight>();
		ptrSingleLight->GetLight().SetPositionToDirectional(0.0f, 1.0f, -0.25f);
	}

	//スプライトの作成
	void StageSelect::CreateBGSprite() {
		float size = 1.7f;
		AddGameObject<TitleSprite>(L"StageSelectBG_TX", false,
			Vec2(792.0f * size, 612.0f * size), Vec2(0.0f, 0.0f));
	}

	void StageSelect::CreateStageImage() {
		float uenodan = -230.0f;
		float sitanodan = -340.0f;
		AddGameObject<StageImage>
			(L"METAL_SHELL_TX", false,
				Vec3(-300.0f, uenodan, 0.0f), Vec2(75.0f, 75.0f), 1);	// Stage:1
		AddGameObject<StageImage>
			(L"METAL_SHELL_TX", false,
				Vec3(-150.0f, uenodan, 0.0f), Vec2(75.0f, 75.0f), 2);	// Stage:2
		AddGameObject<StageImage>
			(L"METAL_SHELL_TX", false,
				Vec3(0.0f, uenodan, 0.0f), Vec2(75.0f, 75.0f), 3);		// Stage:3
		AddGameObject<StageImage>
			(L"METAL_SHELL_TX", false,
				Vec3(150.0f, uenodan, 0.0f), Vec2(75.0f, 75.0f), 4);	// Stage:4
		AddGameObject<StageImage>
			(L"METAL_SHELL_TX", false,
				Vec3(300.0f, uenodan, 0.0f), Vec2(75.0f, 75.0f), 5);	// Stage:5

		AddGameObject<StageImage>
			(L"METAL_SHELL_TX", false,
				Vec3(-300.0f, sitanodan, 0.0f), Vec2(75.0f, 75.0f), 6);	// Stage:6
		AddGameObject<StageImage>
			(L"METAL_SHELL_TX", false,
				Vec3(-150.0f, sitanodan, 0.0f), Vec2(75.0f, 75.0f), 7);	// Stage:7
		AddGameObject<StageImage>
			(L"METAL_SHELL_TX", false,
				Vec3(0.0f, sitanodan, 0.0f), Vec2(75.0f, 75.0f), 8);	// Stage:8
		AddGameObject<StageImage>
			(L"METAL_SHELL_TX", false,
				Vec3(150.0f, sitanodan, 0.0f), Vec2(75.0f, 75.0f), 9);	// Stage:9
		AddGameObject<StageImage>
			(L"METAL_SHELL_TX", false,
				Vec3(300.0f, sitanodan, 0.0f), Vec2(75.0f, 75.0f), 10);	// Stage:10

		//ステージごとのサムネ
		AddGameObject<SelectStageDisplay>
			(
				Vec2(0.0f, 70.0f), Vec2(0.6f, 0.6f)
			);

		AddGameObject<TitleLogo>(L"SELELOGO_TX", false,
			Vec2(1280.0f / 2.5f, 800.0f  / 2.5f), Vec2(-350.0f, 350.0f));

	}
	
	void StageSelect::CreateSelectCursor() {
		AddGameObject<SelectCursor>
			(true, Vec3(-500.0f, 50.0f, 0.0f), Vec2(100.0f, 100.0f), 0.5f);
	}

	//初期化
	void StageSelect::OnCreate() {

		AddGameObject<Fade>
			(
				Vec2(0.0f, 0.0f),
				Vec2(1280.0f, 800.0f),
				false
			);

		CreateViewLight();
		//スプライトの作成
		CreateBGSprite();
		CreateStageImage();
		CreateSelectCursor();
	}


	//更新
	void StageSelect::OnUpdate() {
		auto cntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (cntVec[0].bConnected)
		{
			//リセットボタン
			if (cntVec[0].wPressedButtons & XINPUT_GAMEPAD_START && XINPUT_GAMEPAD_BACK)
			{
				auto scene = App::GetApp()->GetScene<Scene>();
				//scene->m_StageName = L"Title";
				scene->SetStageName(L"Title");
				scene->SetStageNum(0);
				scene->SetCheckPoint(false);
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"ToTitleStage");
			}
		}
	}

}