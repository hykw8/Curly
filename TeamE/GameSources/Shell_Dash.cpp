#include "stdafx.h"
#include "Project.h"

namespace basecross
{

	DashShell::DashShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale) :
		Shell(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{
		m_speed = 20.0f;
		m_Time = 0;
		SetStartScale(StartScale);
	}

	DashShell::~DashShell()
	{
		if (!GetEnemy())
		{
			auto scene = App::GetApp()->GetScene<Scene>();
			if (scene->GetStageNum() == GetStageNum() && scene->GetStageName() == L"GameStage" && !scene->m_Dead)
			{
				//死んだとき自分をリスポーン
				auto dash = GetStage()->AddGameObject<DashShell>(Vec3(GetTakePos().x, GetTakePos().y + 3.0f, GetTakePos().z), Vec3(m_Scale));
				auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
				group->IntoGroup(dash);
			}
		}
	}

	void DashShell::CreateResourses()
	{
		wstring datadir;
		auto& app = App::GetApp();
		//サウンド
		app->GetDataDirectory(datadir);
		datadir += L"SE\\";

		auto SE = datadir + L"Dash.wav";
		app->RegisterWav(L"Dash_1", SE);

	}

	//ダッシュの殻
	
	void DashShell::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(GetStartScale() * GetScaleMagni());
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);

		Mat4x4 mat;
		mat.affineTransformation
		(
			Vec3(0.3f, 0.3f, 0.3f),//scale
			Vec3(0.0f, 0.0f, 0.0f),//rotationの原点
			Vec3(-30.0f * XM_PI / 180.0f, 180.0f * XM_PI / 180.0f, 0.0f),//rotation
			Vec3(0.0f, -2.5f, 0.0f)//position
		);

		//コリジョンを付ける
		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetIsHitAction(IsHitAction::Stop);
		//PtrCol->SetDrawActive(true);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画
		auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
		PtrDraw->SetMeshResource(L"Dash");
		PtrDraw->SetMeshToTransformMatrix(mat);
	
		//リジットボディー
		auto PtrRigid = AddComponent<Rigidbody>();
		PtrRigid->SetGravityVelocity(0.0f, -9.8f, 0.0f);
	
		//重さの設定 軽いか重いか
		SetWeight(Weight::m_NomalWeight);
		AddTag(L"Dash");
		SetName(L"Dash");

		auto scene = App::GetApp()->GetScene<Scene>();
		SetStageNum(scene->GetStageNum());

		SetSpeechballoon(GetStage()->AddGameObject<SpeechBalloon>(Vec3(m_Pos.x, m_Pos.y + 0.1f, m_Pos.z), Vec3(1.8f, 1.3f, 1.0f), GetThis<DashShell>()));
	}
	
	void DashShell::OnUpdate()
	{
				
	}
	
	Vec3 DashShell::Ability()
	{
		//ダッシュ
		auto time = App::GetApp()->GetElapsedTime();
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		Vec3 dash(m_Velo);
		if (player->GetDashFlag())
		{

			m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
			m_AudioObjectPtr->AddAudioResource(L"Dash_1");
			m_AudioObjectPtr->Start(L"Dash_1", 0, 0.1f);

			dash = Vec3(m_Velo.x, 0.0f, m_Velo.z);
			dash.normalize();
			dash *= m_speed;
		}
		else
		{
			m_Velo = player->GetComponent<Rigidbody>()->GetVelocity();
		}
	
		player->SetDashFlag(true);
	
		return dash;
	};

	void DashShell::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Floor"))
			{
				GetComponent<CollisionObb>()->SetIsHitAction(IsHitAction::Stop);
			}

			if (v->FindTag(L"Wall"))
			{
				auto playerPos = GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
				auto pos = GetComponent<Transform>()->GetPosition();
				auto velo = playerPos - pos;
				GetStage()->RemoveGameObject<SpeechBalloon>(GetThis<Shell>()->GetSpeechballoon().lock());
				for (int i = 0; i < 7; i++)
				{
					GetStage()->RemoveGameObject<GameObject>(GetThis<Shell>()->GetSpeechballoon().lock()->GetThis<SpeechBalloon>()->m_UI[i].lock());
				}
				GetStage()->RemoveGameObject<Shell>(GetThis<Shell>());
				auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
				group->Remove(GetThis<Shell>());
			}
		}
	}
};
