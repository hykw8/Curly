/*!
@file DebugStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージ1
	//--------------------------------------------------------------------------------------


	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<MyCamera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		PtrCamera->SetEye(Vec3(0.0f, 3.0f, -10.0f));
		PtrCamera->SetHeight(3.0f);

		auto multiLight = CreateLight<MultiLight>();
		multiLight->SetDefaultLighting();
	}


	// ステージクリアに必要なアイテムの作成
	void GameStage::CreateGoalItem()
	{
		AddGameObject<GoalCount>(
			Vec3(-400.0f, -348.0f, 0.0f),
			Vec3(70.0f, 70.0f, 1.0f)
			);

		AddGameObject<GoalAllUI>(
			Vec2(0.0f, 0.0f),
			Vec2(2126.0f, 591.0f)
			);

		AddGameObject<OnlyDisPlay>(
			Vec3(2048.0f / 10.0f, 1536.0f / 10.0f, 1.0f),
			Vec3(-500.0f, -350.0f, 0.0f),
			L"RESTCORALS2_TX"
			);

		AddGameObject<OnlyDisPlay>(
			Vec3(2048.0f / 5.0f, 1536.0f / 5.0f, 1.0f),
			Vec3(-390.0f, -350.0f, 0.0f),
			L"BATU_TX"
			);
	}

	void GameStage::OnCreate() {
		try {

			m_EfkInterface = ObjectFactory::Create<EfkInterface>();
			auto ef = AddGameObject<Effect>(L"Effect\\SangoEffect.efk");
			SetSharedGameObject(L"Effect", ef);
			auto ef1 = AddGameObject<Effect>(L"Effect\\Brake_Wall.efk");
			SetSharedGameObject(L"BreakWall", ef1);
			auto goal = AddGameObject<GoalEffect>();

			//ビューとライトの作成
			CreateViewLight();

			auto scene = App::GetApp()->GetScene<Scene>();
			wstring dataDir;
			App::GetApp()->GetDataDirectory(dataDir);

			UnityCreate unityCreate(GetThis<GameStage>(), L"Stage" + Util::IntToWStr(scene->GetStageNum()) + L".csv");
			unityCreate.OnCreate();

			CreateGoalItem();

			AddGameObject<hukidasi>
				(
					Vec2(420.0f, 250.0f),
					Vec2(1280.0f / 4.0f, 800.0f / 4.0f)
					);
			
			AddGameObject<BackGround>(Vec3(scene->m_StageCenter), Vec3(scene->GetStageSize() * -2.0f));
			AddGameObject<Pause>(Vec3(0), Vec2(1));

		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnUpdate() {

		m_EfkInterface->OnUpdate();
	
		//リセットボタン
		auto cntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (cntVec[0].bConnected)
		{
			if (cntVec[0].wPressedButtons & XINPUT_GAMEPAD_START && cntVec[0].wPressedButtons & XINPUT_GAMEPAD_BACK)
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

	void GameStage::OnDraw() {

		auto& camera = GetView()->GetTargetCamera();
		m_EfkInterface->SetViewProj(camera->GetViewMatrix(), camera->GetProjMatrix());
		m_EfkInterface->OnDraw();
		
	}
}
//end basecross