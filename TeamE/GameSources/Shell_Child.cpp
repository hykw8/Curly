#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	///	テンプレート　こんな風に書いていってほしい
	//--------------------------------------------------------------------------------------
	TempShell::TempShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale) :
		Shell(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{
		SetStartScale(StartScale);
	}

	TempShell::~TempShell()
	{
		if (!GetEnemy())
		{
			auto scene = App::GetApp()->GetScene<Scene>();
			if (scene->GetStageNum() == GetStageNum() && scene->GetStageName() == L"GameStage" && !scene->m_Dead)
			{
				//死んだとき自分をリスポーン
				auto temp = GetStage()->AddGameObject<TempShell>(Vec3(GetTakePos().x, GetTakePos().y + 3.0f, GetTakePos().z), Vec3(m_Scale));
				auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
				group->IntoGroup(temp);
			}
		}
	}
	

	void TempShell::OnCreate()
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
			Vec3(-30.0f * XM_PI / 180.0f, 0.0f, 0.0f),//rotation
			Vec3(0.0f, -0.8f, 0.2f)//position
		);

		//コリジョンを付ける
		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetIsHitAction(IsHitAction::Stop);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrRigid = AddComponent<Rigidbody>();
		PtrRigid->SetGravityVelocity(0.0f, -9.8f, 0.0f);
		//描画
		auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
		PtrDraw->SetMeshResource(L"Normal");
		//PtrDraw->SetMeshResource(L"Unko");
		PtrDraw->SetMeshToTransformMatrix(mat);
		PtrDraw->AddAnimation(L"d", 30, 30, true);
		PtrDraw->ChangeCurrentAnimation(L"d");

		//重さの設定 軽いか重いか
		SetWeight(Weight::m_NomalWeight);
		AddTag(L"Temp");
		SetName(L"Temp");

		auto scene = App::GetApp()->GetScene<Scene>();
		SetStageNum(scene->GetStageNum());

		SetSpeechballoon(GetStage()->AddGameObject<SpeechBalloon>(Vec3(m_Pos.x, m_Pos.y + 0.1f, m_Pos.z), Vec3(1.8f, 1.3f, 1.0f), GetThis<TempShell>()));
	}

	void TempShell::OnUpdate()
	{
		//auto ptrDraw = GetComponent<BcPNTStaticModelDraw>();
		//ptrDraw->UpdateAnimation(App::GetApp()->GetElapsedTime());
	}

	Vec3 TempShell::Ability()
	{
		//能力をここに書く
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto velo = player->GetComponent<Rigidbody>()->GetVelocity();
		return Vec3(velo);
	}

	void TempShell::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Floor"))
			{
				GetComponent<CollisionObb>()->SetIsHitAction(IsHitAction::Stop);

				if (HitFloor(v, GetComponent<Transform>()->GetPosition()))
				{
				}
			}

			if (v->FindTag(L"Wall"))
			{
				auto playerPos = GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
				auto pos = GetComponent<Transform>()->GetPosition();
				auto velo = playerPos - pos;
				//GetComponent<Rigidbody>()->SetVelocity(velo.x, 6.0f, velo.z);
				//GetComponent<Rigidbody>()->SetVelocity(velo.x, velo.y, velo.z);

				GetStage()->RemoveGameObject<SpeechBalloon>(GetThis<Shell>()->GetSpeechballoon().lock());
				for (int i = 0; i < 7; i++)
				{
					GetStage()->RemoveGameObject<GameObject>(GetThis<Shell>()->GetSpeechballoon().lock()->GetThis<SpeechBalloon>()->m_UI[i].lock());
				}
				GetStage()->RemoveGameObject<Shell>(GetThis<Shell>());
				auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
				group->Remove(GetThis<Shell>());
			}

			if (v->FindTag(L"Player"))
			{
				GetComponent<CollisionObb>()->SetIsHitAction(IsHitAction::Stop);
			}
		}
	}


	//回るやつ
	SpinShell::SpinShell(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale) :
		Shell(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{
		m_Speed = 10.0f;
		m_height = 2.0f;
		SetStartScale(StartScale);
	}

	SpinShell::~SpinShell()
	{
		if (!GetEnemy())
		{
			auto scene = App::GetApp()->GetScene<Scene>();
			if (scene->GetStageNum() == GetStageNum() && scene->GetStageName() == L"GameStage" && !scene->m_Dead)
			{
				//死んだとき自分をリスポーン
				auto spin = GetStage()->AddGameObject<SpinShell>(Vec3(GetTakePos().x, GetTakePos().y + 3.0f, GetTakePos().z), Vec3(m_Scale));
				auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
				group->IntoGroup(spin);
			}
		}
	}

	void SpinShell::CreateResourses()
	{
		wstring datadir;
		auto& app = App::GetApp();
		//サウンド
		app->GetDataDirectory(datadir);
		datadir += L"SE\\";

		auto SE = datadir + L"Spin.wav";
		app->RegisterWav(L"Spin_1", SE);
	}

	void SpinShell::OnCreate()
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
			Vec3(-30.0f * XM_PI / 180.0f, 0.0f, 0.0f),//rotation
			Vec3(0.0f, -0.5f, 0.1f)//position
		);

		auto PtrRigid = AddComponent<Rigidbody>();
		PtrRigid->SetGravityVelocity(Vec3(0.0f, -9.8f, 0.0f));
		//コリジョンを付ける
		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetIsHitAction(IsHitAction::Stop);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画
		auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
		PtrDraw->SetMeshResource(L"Spin");
		PtrDraw->SetMeshToTransformMatrix(mat);

		//重さの設定 軽いか重いか
		SetWeight(Weight::m_Light);
		AddTag(L"Spin");
		SetName(L"Spin");
		auto scene = App::GetApp()->GetScene<Scene>();

		SetStageNum(scene->GetStageNum());

		SetSpeechballoon(GetStage()->AddGameObject<SpeechBalloon>(Vec3(m_Pos.x, m_Pos.y + 0.1f, m_Pos.z), Vec3(1.8f, 1.3f, 1.0f), GetThis<SpinShell>()));
	}

	void SpinShell::OnUpdate()
	{

	}

	Vec3 SpinShell::Ability()
	{
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");

		//回転
		auto time = App::GetApp()->GetElapsedTime();
		SetTotalTime(GetTotalTime() + time);
		auto rot = GetComponent<Transform>()->GetRotation();
		SetRot(Vec3(0.0f, rot.y + time * m_Speed, 0.0f));

		auto rigid = player->GetComponent<Rigidbody>();
		auto velo = rigid->GetVelocity();

		auto veloGra = rigid->GetGravityVelocity();

		if (GetTotalTime() <= 1.0f / 2.0f)
		{
			m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
			m_AudioObjectPtr->AddAudioResource(L"Spin_1");
			m_AudioObjectPtr->Start(L"Spin_1", 0, 0.2f);

			veloGra = Vec3(0.0f, 0.2f, 0.0f);
			velo = Vec3(velo.x, 0.15f * 2.0f, velo.z);
		}
		else
		{
			veloGra = Vec3(0.0f, 0.0f, 0.0f);
			velo = Vec3(velo.x, 0.0f, velo.z);
		}
		rigid->SetGravityVelocity(veloGra);

		return velo;
	}

	void SpinShell::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Floor"))
			{
				GetComponent<CollisionObb>()->SetIsHitAction(IsHitAction::Stop);

				if (HitFloor(v, GetComponent<Transform>()->GetPosition()))
				{
				}
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
	//--------------------------------------------------------------------------------------------------------


	// 水鉄砲
	Shell_WaterGun::Shell_WaterGun(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const float & StartWater) :
		Shell(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Water(StartWater)
	{}


	float water;

	void Shell_WaterGun::OnCreate()
	{

		auto PtrString = AddComponent<StringSprite>();

		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);

		//コリジョンを付ける
		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetIsHitAction(IsHitAction::Stop);
		//描画
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetDiffuse(Col4(0.0f, 0.0f, 1.0f, 1.0f));

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrRigid = AddComponent<Rigidbody>();
		PtrRigid->SetGravityVelocity(0.0f, -9.8f, 0.0f);

		//	//重さの設定 軽いか重いか
		//	SetWeight(Weight::m_Light);

		// 水鉄砲残弾設定
		water = m_Water;
		AddTag(L"WaterGun");
	}

	void Shell_WaterGun::OnUpdate()
	{

		//water += 0.1f;	
	}

	
	Vec3 Shell_WaterGun::Ability()
	{
		auto UI = GetStage()->GetSharedGameObject<SkillGauge>(L"SkillGauge");
		UI->WaterGunGauge(water, m_Water);

		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		//auto playerVelo = player->GetComponent<Rigidbody>()->GetVelocity();
		auto playerVelo = player->GetDire();
		if (water > 1.0f) {

			// 能力：水鉄砲
			auto Key = App::GetApp()->GetInputDevice().GetKeyState();
			auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			//auto playerVelo = player->GetDire();	
			if (CntlVec[0].bConnected) {
				if (CntlVec[0].wButtons & XINPUT_GAMEPAD_A)
				{
					water -= 0.5f;


					auto Ptr = GetComponent<Transform>();
					Vec3 Pos = Ptr->GetWorldPosition();
					Pos.y += 0.25f;
					Quat Qt = Ptr->GetQuaternion();
					Vec3 Rot = Qt.toRotVec();
					float RotY = Rot.y - 90.0f * XM_PI / 180.0f;
					Vec3 velo(playerVelo.x, 0.05f, playerVelo.z);
					velo.normalize();
					velo *= 20.0f;
					auto Group = GetStage()->GetSharedObjectGroup(L"ShellGroup");
					for (size_t i = 0; i < Group->size(); i++) {
						auto shptr = dynamic_pointer_cast<Water>(Group->at(i));
						//auto shptr = dynamic_pointer_cast<ShellSphere>(Group->at(i));
						if (shptr && !shptr->IsUpdateActive()) {
							//空きが見つかった
							shptr->Reset(Pos, velo);
							return playerVelo;
						}
					}
					//ここまで来てれば空きがない
					GetStage()->AddGameObject<Water>
					(
						Pos,
						Vec3(0.5f,0.5f,0.5f),
						velo,
						L"DEFAULT_SPHERE",
						WaterShotState::Instance()
					);
					//GetStage()->AddGameObject<ShellSphere>(Pos, velo);
				}
			}

		}

		if (water >= m_Water ) {
			water = m_Water;
		}
		return playerVelo;
	}

	// 気球
	BallonShell::BallonShell(const shared_ptr<Stage>& StagePtr,const Vec3 & StartPos,const float StartScale):
		Shell(StagePtr),m_Pos(StartPos),m_Scale(StartScale)
	{
		m_VecScale = Vec3(m_Scale, m_Scale, m_Scale);
		SetStartScale(Vec3(StartScale, StartScale, StartScale));
	}
	BallonShell::BallonShell(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale):
		Shell(StagePtr), m_Pos(StartPos), m_VecScale(StartScale), m_Scale(StartScale.x)
	{
		SetStartScale(StartScale);
	}

	BallonShell::~BallonShell()
	{
		if (!GetEnemy())
		{
			auto scene = App::GetApp()->GetScene<Scene>();
			if (scene->GetStageNum() == GetStageNum() && scene->GetStageName() == L"GameStage" && !scene->m_Dead)
			{
				//死んだとき自分をリスポーン
				auto ballon = GetStage()->AddGameObject<BallonShell>(Vec3(GetTakePos().x, GetTakePos().y + 3.0f, GetTakePos().z), Vec3(m_Scale));
				auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
				group->IntoGroup(ballon);
			}
		}
	}

	void BallonShell::CreateResourses()
	{
		wstring datadir;
		auto& app = App::GetApp();
		//サウンド
		app->GetDataDirectory(datadir);
		datadir += L"SE\\";

		auto SE = datadir + L"Ballon.wav";
		app->RegisterWav(L"Ballon", SE);
	}

	void BallonShell::OnCreate()
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
			Vec3(-30.0f * XM_PI / 180.0f, 0.0f, 0.0f),//rotation
			Vec3(0.0f, -0.8f, 0.2f)//position
		);

		//コリジョンを付ける
		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetIsHitAction(IsHitAction::Stop);
		//PtrCol->SetDrawActive(true);

		auto PtrRigid = AddComponent<Rigidbody>();
		PtrRigid->SetGravityVelocity(0.0f, -9.8f, 0.0f);
		//描画
		auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
		PtrDraw->SetMeshResource(L"Normal");
		PtrDraw->SetMeshToTransformMatrix(mat);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//重さの設定 軽いか重いか
		SetWeight(Weight::m_Light);
		AddTag(L"Ballon");
		SetName(L"Ballon");

		auto scene = App::GetApp()->GetScene<Scene>();
		SetStageNum(scene->GetStageNum());
		NormalShellCreate();

		SetSpeechballoon(GetStage()->AddGameObject<SpeechBalloon>(Vec3(m_Pos.x, m_Pos.y + 0.1f, m_Pos.z), Vec3(1.8f, 1.3f, 1.0f), GetThis<BallonShell>()));
	}

	void BallonShell::OnUpdate()
	{
		Limit();
	}

	Vec3 BallonShell::Ability()
	{
		auto PtrTrans = GetComponent<Transform>();
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto rigid = player->GetComponent<Rigidbody>();
		auto veloGra = rigid->GetGravityVelocity();
		veloGra = Vec3(0.0f,2.5f, 0.0f);

		rigid->SetGravityVelocity(veloGra);

		m_VecScale += 0.01f;
		for (int i = 0; i < 3; i++)
		{
			m_ballon[i].lock()->GetComponent<Transform>()->SetScale(m_VecScale);
		}

		m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
		m_AudioObjectPtr->AddAudioResource(L"Ballon");
		m_AudioObjectPtr->Start(L"Ballon", 0, 0.5f);

		return Vec3(0);
	}

	void BallonShell::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Floor"))
			{
				GetComponent<CollisionObb>()->SetIsHitAction(IsHitAction::Stop);

				if (HitFloor(v, GetComponent<Transform>()->GetPosition()))
				{
				}
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

				for (int j = 0; j < static_cast<int>(GetThis<BallonShell>()->GetBallonChild().size()); j++)
				{
					GetStage()->RemoveGameObject<GameObject>(GetThis<BallonShell>()->GetBallonChild()[j].lock());
				}
				GetStage()->RemoveGameObject<Shell>(GetThis<Shell>());
				auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
				group->Remove(GetThis<Shell>());
			}
		}
	}

	void BallonShell::Limit()
	{
		auto Scale = m_ballon[0].lock()->GetComponent<Transform>()->GetScale();

		if (Scale.x >= 2.0f)
		{
			auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
			player->ShellThrow();

			for (int i = 0; i < 7; i++)
			{
				GetStage()->RemoveGameObject<GameObject>(GetSpeechballoon().lock()->GetThis<SpeechBalloon>()->m_UI[i].lock());
				if (i < 3)
				{
					GetStage()->RemoveGameObject<GameObject>(m_ballon[i].lock());
				}
			}
			GetStage()->RemoveGameObject<SpeechBalloon>(GetSpeechballoon().lock());

			GetStage()->RemoveGameObject<BallonShell>(GetThis<BallonShell>());
		}
	}

	void BallonShell::NormalShellCreate()
	{
		m_ballon.push_back(GetStage()->AddGameObject<StaticObject>(Vec3(m_Pos.x + 0.5f, m_Pos.y, m_Pos.z), Vec3(m_Scale), Vec3(0)));
		auto transRight = m_ballon[0].lock()->GetComponent<Transform>();
		transRight->SetScale(GetStartScale() * GetScaleMagni());
		auto ptrDrawRight = m_ballon[0].lock()->AddComponent<PNTStaticDraw>();
		ptrDrawRight->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDrawRight->SetTextureResource(L"BALLN_TX");

		m_ballon.push_back(GetStage()->AddGameObject<StaticObject>(Vec3(m_Pos.x - 0.5f, m_Pos.y, m_Pos.z), Vec3(m_Scale), Vec3(0)));
		auto transLeft = m_ballon[1].lock()->GetComponent<Transform>();
		transLeft->SetScale(GetStartScale() * GetScaleMagni());
		auto ptrDrawLeft = m_ballon[1].lock()->AddComponent<PNTStaticDraw>();
		ptrDrawLeft->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDrawLeft->SetTextureResource(L"BALLN_TX");

		m_ballon.push_back(GetStage()->AddGameObject<StaticObject>(Vec3(m_Pos.x, m_Pos.y + 0.5f, m_Pos.z), Vec3(m_Scale), Vec3(0)));
		auto transUp = m_ballon[2].lock()->GetComponent<Transform>();
		transUp->SetScale(GetStartScale() * GetScaleMagni());
		auto ptrDrawUp = m_ballon[2].lock()->AddComponent<PNTStaticDraw>();
		ptrDrawUp->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDrawUp->SetTextureResource(L"BALLN_TX");

		for (int i = 0; i < 3; i++)
		{
			m_ballon[i].lock()->GetComponent<Transform>()->SetParent(GetThis<BallonShell>());
		}
	}

	// 鉄の殻
	IronShell::IronShell(const shared_ptr<Stage>& StagePtr,const Vec3 & StartPos,const Vec3 & StartScale)
		:Shell(StagePtr),m_Pos(StartPos),m_Scale(StartScale)
	{
		SetStartScale(StartScale);
		SetScaleMagni(0.3f);
	}

	IronShell::~IronShell()
	{
		if (!GetEnemy())
		{
			auto scene = App::GetApp()->GetScene<Scene>();
			if (scene->GetStageNum() == GetStageNum() && scene->GetStageName() == L"GameStage" && !scene->m_Dead)
			{
				//死んだとき自分をリスポーン
				auto iron = GetStage()->AddGameObject<IronShell>(Vec3(GetTakePos().x, GetTakePos().y + 3.0f, GetTakePos().z), Vec3(m_Scale));
				auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
				group->IntoGroup(iron);
			}
		}
	}

	void IronShell::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(GetStartScale() * GetScaleMagni());
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);

		Mat4x4 mat;
		mat.affineTransformation
		(
			Vec3(0.4f, 0.4f, 0.4f),//scale
			Vec3(0.0f, 0.0f, 0.0f),//rotationの原点
			Vec3(0.0f, 0.0f, 0.0f),//rotation
			Vec3(0.0f, 0.3f, 0.1f)//position
		);

		//コリジョンを付ける
		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetIsHitAction(IsHitAction::Stop);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
		PtrDraw->SetMeshResource(L"Metal");
		PtrDraw->SetTextureResource(L"MetalTex_TX");
		PtrDraw->SetMeshToTransformMatrix(mat);
		PtrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));

		auto PtrRigid = AddComponent<Rigidbody>();
		PtrRigid->SetGravityVelocity(0.0f, -9.8f, 0.0f);

		//重さの設定 軽いか重いか
		SetWeight(Weight::m_Heavy);
		AddTag(L"Iron");
		SetName(L"Iron");

		auto scene = App::GetApp()->GetScene<Scene>();
		SetStageNum(scene->GetStageNum());

		SetSpeechballoon(GetStage()->AddGameObject<SpeechBalloon>(Vec3(m_Pos.x, m_Pos.y + 0.1f, m_Pos.z), Vec3(1.8f, 1.3f, 1.0f), GetThis<IronShell>()));
	}

	void IronShell::OnUpdate()
	{

	}

	Vec3 IronShell::Ability()
	{
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto velo = player->GetComponent<Rigidbody>()->GetVelocity();
		return Vec3(velo);
	}

	void IronShell::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Floor"))
			{
				GetComponent<CollisionObb>()->SetIsHitAction(IsHitAction::Stop);

				if (HitFloor(v, GetComponent<Transform>()->GetPosition()))
				{
				}
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
}