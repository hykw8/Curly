#include "stdafx.h"
#include "Project.h"
#include "SimpleObject.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------
	
	//ゴールするためのアイテム
	GoalItem::GoalItem(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{}

	void GoalItem::CreateResourses()
	{
		wstring datadir;
		auto& app = App::GetApp();
		//サウンド
		app->GetDataDirectory(datadir);
		datadir += L"SE\\";

		auto SE = datadir + L"Item.wav";
		app->RegisterWav(L"Item", SE);
	}

	void GoalItem::OnCreate()
	{
		auto scene = App::GetApp()->GetScene<Scene>();
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos.x, m_Pos.y + 20.0f, m_Pos.z);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);

		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetMakedSize(0.5f);
		PtrCol->SetIsHitAction(IsHitAction::None);

		auto ptrRigid = AddComponent<Rigidbody>();

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SQUARE");
		PtrDraw->SetTextureResource(L"SANGO_TX");
		PtrDraw->SetDepthStencilState(DepthStencilState::Read);
		SetAlphaActive(true);

		auto group = GetStage()->GetSharedObjectGroup(L"GoalItemGroup");
		group->IntoGroup(GetThis<GoalItem>());

		m_Ef = GetStage()->AddGameObject<Effect>(L"Effect\\SangoBeacon.efk");
		m_Ef.lock()->GetThis<Effect>()->Play(Vec3(m_Pos.x, m_Pos.y - 0.5f, m_Pos.z));
	}
	void GoalItem::OnUpdate()
	{
		auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		auto pos = GetComponent<Transform>()->GetPosition();
		float radian = atan2(pos.z - camera->GetEye().z, pos.x - camera->GetEye().x);
		GetComponent<Transform>()->SetRotation(0.0f, -(radian + XM_PIDIV2), 0.0f);

		if (camera->GetStartFlag())
		{
			StartMove();
		}
		else
		{
			GetComponent<Transform>()->SetPosition(m_Pos);
		}
	}

	void GoalItem::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Player"))
			{
				auto Ptr = GetComponent<Transform>();

				auto ef = GetStage()->GetSharedGameObject<Effect>(L"Effect");
				ef->Play(Vec3(Ptr->GetPosition().x, Ptr->GetPosition().y, Ptr->GetPosition().z));

				m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
				m_AudioObjectPtr->AddAudioResource(L"Item");
				m_AudioObjectPtr->Start(L"Item", 0, 0.5f);
				GetStage()->RemoveGameObject<Effect>(m_Ef.lock());
				//触れた瞬間削除
				GetStage()->RemoveGameObject<GoalItem>(GetThis<GoalItem>());

				GetStage()->GetSharedObjectGroup(L"GoalItemGroup")->Remove(GetThis<GoalItem>());

			}
		}
	}

	void GoalItem::StartMove()
	{
		auto trans = GetComponent<Transform>();
		if (m_Pos.y > trans->GetPosition().y)
		{
			GetComponent<Rigidbody>()->SetGravityVelocityZero();
		}
		else
		{
			GetComponent<Rigidbody>()->SetGravityVelocity(0.0f, -3.0f, 0.0f);
		}
	}

	//死に場所
	DeadZone::DeadZone(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{}

	void DeadZone::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(Vec3(0.0f, 0.0f, 0.0f));

		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetIsHitAction(IsHitAction::None);

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"VOLGANON_TX");

		//PtrCol->SetDrawActive(true);
	}

	void DeadZone::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			//Playerが当たったら
			if (v->FindTag(L"Player"))
			{
				v->GetThis<Player>()->GetStateMachine()->Reset(DeathState::Instance());

				auto scene = App::GetApp()->GetScene<Scene>();
				//PostEvent(3.0f, GetThis<ObjectInterface>(), scene, L"ToGameStage");
				scene->m_Dead = true;
				//GetStage()->RemoveGameObject<Player>(v);
				auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
				group->Remove(v);
			}

			//殻が当たったら
			if (v->FindTag(L"Shell"))
			{
				if (v->FindTag(L"Ballon"))
				{
					GetStage()->RemoveGameObject<SpeechBalloon>(v->GetThis<Shell>()->GetSpeechballoon().lock());
					for (int i = 0; i < 7; i++)
					{
						GetStage()->RemoveGameObject<GameObject>(v->GetThis<Shell>()->GetSpeechballoon().lock()->GetThis<SpeechBalloon>()->m_UI[i].lock());
					}

					for (int j = 0; j < static_cast<int>(v->GetThis<BallonShell>()->GetBallonChild().size()); j++)
					{
						GetStage()->RemoveGameObject<GameObject>(v->GetThis<BallonShell>()->GetBallonChild()[j].lock());
					}
					GetStage()->RemoveGameObject<Shell>(v);
					auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
					group->Remove(v);
				}
				else
				{
					GetStage()->RemoveGameObject<SpeechBalloon>(v->GetThis<Shell>()->GetSpeechballoon().lock());
					for (int i = 0; i < 7; i++)
					{
						GetStage()->RemoveGameObject<GameObject>(v->GetThis<Shell>()->GetSpeechballoon().lock()->GetThis<SpeechBalloon>()->m_UI[i].lock());
					}
					GetStage()->RemoveGameObject<Shell>(v);
					auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
					group->Remove(v);
				}
			}

			if (v->FindTag(L"Enemy"))
			{
				GetStage()->RemoveGameObject<GameObject>(v->GetThis<Enemy>()->GetMyShell().lock()->GetThis<Shell>()->GetSpeechballoon().lock());
				for (int i = 0; i < 7; i++)
				{
					GetStage()->RemoveGameObject<GameObject>(v->GetThis<Enemy>()->GetMyShell().lock()->GetThis<Shell>()->GetSpeechballoon().lock()->GetThis<SpeechBalloon>()->m_UI[i].lock());
				}
				GetStage()->RemoveGameObject<Shell>(v->GetThis<Enemy>()->GetMyShell().lock());
				GetStage()->RemoveGameObject<Enemy>(v);

				auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
				group->Remove(v);
			}
		}
	}



	SangoSpark::SangoSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	SangoSpark::~SangoSpark() {}

	//初期化
	void SangoSpark::OnCreate() 
	{
		//SangoSpark::InsertSpark(Vec3(0, 0, 0));
	}

	// サンゴの演出
    
	void SangoSpark::InsertSpark(const Vec3& Pos) {

			auto ParticlePtr = InsertParticle(20);
			ParticlePtr->SetEmitterPos(Pos);
			ParticlePtr->SetMaxTime(2.0f);

			vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
				rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f - 0.05f;
				rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
				//各パーティクルの移動速度を指定
				rParticleSprite.m_Velocity = Vec3(
					rParticleSprite.m_LocalPos.x * 50.0f,
					rParticleSprite.m_LocalPos.y * 30.0f,
					rParticleSprite.m_LocalPos.z * 30.0f
				);
				//色の指定
				rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			}
	}

	//------------------------------------------------------------------
	///水辺 奥山
	//------------------------------------------------------------------
	Water::Water
	(
		const shared_ptr<Stage>&StagePtr, 
		const Vec3 & StartPos, 
		const Vec3 & StartScale,
		const Vec3& Velocity,
		wstring StrMesh,
		shared_ptr<ObjState<Water>> StateMachine
	) :
		GameObject(StagePtr), 
		m_Pos(StartPos), 
		m_Scale(StartScale),
		m_Velocity(Velocity),
		m_Mesh(StrMesh),
		m_State(StateMachine)
	{
		m_Gravity = Vec3(0.0f, -9.8f, 0.0f);
	}

	void Water::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(Vec3(0.0f, 0.0f, 0.0f));

		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetIsHitAction(IsHitAction::None);

		auto PtrRigid = AddComponent<Rigidbody>();
		PtrRigid->SetVelocity(m_Velocity);

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(m_Mesh);

		m_StateMachine.reset(new StateMachine<Water>(GetThis<Water>()));
		m_StateMachine->ChangeState(m_State.lock());
	}

	void Water::OnUpdate()
	{
		m_StateMachine->Update();

		//m_Rotation.x += m_Dir.x;
		//m_Rotation.y += m_Dir.y;
		//m_Rotation.z += m_Dir.z;
		//auto PtrTrans = GetComponent<Transform>();
		//PtrTrans->SetRotation(m_Rotation);
		////if (m_Rotation.z > 360.0f) {
		////	m_Rotation.z = 0.0f;
		////}

		//if (m_Rotation.x > 360.0f) {
		//	m_Rotation.x = 0.0f;
		//}
		//if (m_Rotation.y > 360.0f) {
		//	m_Rotation.y = 0.0f;
		//}
		//if (m_Rotation.z > 360.0f) {
		//	m_Rotation.z = 0.0f;
		//}

	}

	void Water::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Player"))
			{
				//GetStage()->RemoveGameObject<Water>(GetThis<Water>());
			}
		}
	}

	void Water::Active(bool flag)
	{
		SetUpdateActive(flag);
		SetDrawActive(flag);
	}

	void Water::Reset(const Vec3 & Emitter, const Vec3 & Velocity)
	{
		Active(true);

		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->ResetPosition(Emitter);
		auto ptrRigid = GetComponent<Rigidbody>();
		ptrRigid->SetGravityVelocityZero();
		ptrRigid->SetVelocity(Velocity);
	}

	void Water::Gravity()
	{
		auto time = App::GetApp()->GetElapsedTime();
		auto ptrRigid = GetComponent<Rigidbody>();
		auto velo = ptrRigid->GetGravityVelocity();
		velo += m_Gravity * time;
		ptrRigid->SetGravityVelocity(velo);
	}

	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	IMPLEMENT_SINGLETON_INSTANCE(WaterDefalutState)
	void WaterDefalutState::Enter(const shared_ptr<Water>& Obj)
	{
	}
	void WaterDefalutState::Execute(const shared_ptr<Water>& Obj)
	{
	}
	void WaterDefalutState::Exit(const shared_ptr<Water>& Obj)
	{
	}

	IMPLEMENT_SINGLETON_INSTANCE(WaterShotState)
	void WaterShotState::Enter(const shared_ptr<Water>& Obj)
	{
		auto Group = Obj->GetStage()->GetSharedObjectGroup(L"ShellGroup");
		Group->IntoGroup(Obj->GetThis<GameObject>());

		Obj->GetComponent<CollisionObb>()->SetIsHitAction(IsHitAction::None);
	}
	void WaterShotState::Execute(const shared_ptr<Water>& Obj)
	{
		Obj->Gravity();
	}
	void WaterShotState::Execute2(const shared_ptr<Water>& Obj)
	{
		auto PtrTransform = Obj->GetComponent<Transform>();
		if (PtrTransform->GetPosition().y < -0.5f) {
			Obj->Active(false);
		}
		if (Obj->GetComponent<Collision>()->GetHitObjectVec().size() > 0) {
			for (auto& v : Obj->GetComponent<Collision>()->GetHitObjectVec()) {
				auto& ptr = dynamic_pointer_cast<Player>(v);
				if (ptr) {
					//GetStage()->RemoveGameObject<Player>(ptr);
				}
			}
		}
	}
	void WaterShotState::Exit(const shared_ptr<Water>& Obj)
	{
	}

	//------------------------------------------------------------------
	///ただの床
	//------------------------------------------------------------------
	Floor::Floor(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale, const Vec3& StartRot) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Rot(StartRot)
	{}

	void Floor::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(m_Rot);

		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetFixed(true);

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"CONKURI_TX");
		PtrDraw->SetOwnShadowActive(true);

		PtrDraw->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 0.0f));

		AddTag(L"Floor");
	}

	void Floor::OnUpdate()
	{

	}

	void Floor::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{

	}

	//------------------------------------------------------------------
	///外壁
	//------------------------------------------------------------------
	Wall::Wall(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{}
	void Wall::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);

		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetFixed(true);

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++)
		{
			if (static_cast<Vec3>(
				XMVector3AngleBetweenNormals(
					static_cast<XMVECTOR>(vertices[i].normal),
					static_cast<XMVECTOR>(Vec3(0.0f,1.0f,0.0f)))).x < 0.01f
				)
			{
				//上
				if (vertices[i].textureCoordinate.x >= 1.0f)
				{
					vertices[i].textureCoordinate.x = m_Scale.x;
				}
				if (vertices[i].textureCoordinate.y >= 1.0f)
				{
					vertices[i].textureCoordinate.y = m_Scale.z;
				}
			}
			else if (static_cast<Vec3>(
				XMVector3AngleBetweenNormals(
					static_cast<XMVECTOR>(vertices[i].normal),
					static_cast<XMVECTOR>(Vec3(0.0f, -1.0f, 0.0f)))).x < 0.01f
				)
			{
				//下
				if (vertices[i].textureCoordinate.x >= 1.0f)
				{
					vertices[i].textureCoordinate.x = m_Scale.x;
				}
				if (vertices[i].textureCoordinate.y >= 1.0f)
				{
					vertices[i].textureCoordinate.y = m_Scale.z;
				}
			}
			else if (static_cast<Vec3>(
				XMVector3AngleBetweenNormals(
					static_cast<XMVECTOR>(vertices[i].normal),
					static_cast<XMVECTOR>(Vec3(-1.0f, 0.0f, 0.0f)))).x < 0.01f
				)
			{
				//左
				if (vertices[i].textureCoordinate.x >= 1.0f)
				{
					vertices[i].textureCoordinate.x = m_Scale.z;
				}
				if (vertices[i].textureCoordinate.y >= 1.0f)
				{
					vertices[i].textureCoordinate.y = m_Scale.y;
				}
			}
			else if (static_cast<Vec3>(
				XMVector3AngleBetweenNormals(
					static_cast<XMVECTOR>(vertices[i].normal),
					static_cast<XMVECTOR>(Vec3(1.0f, 0.0f, 0.0f)))).x < 0.01f
				)
			{
				//右
				if (vertices[i].textureCoordinate.x >= 1.0f)
				{
					vertices[i].textureCoordinate.x = m_Scale.z;
				}
				if (vertices[i].textureCoordinate.y >= 1.0f)
				{
					vertices[i].textureCoordinate.y = m_Scale.y;
				}
			}
			if (static_cast<Vec3>(
				XMVector3AngleBetweenNormals(
					static_cast<XMVECTOR>(vertices[i].normal),
					static_cast<XMVECTOR>(Vec3(0.0f, 0.0f, -1.0f)))).x < 0.01f
				)
			{
				//手前
				if (vertices[i].textureCoordinate.x >= 1.0f)
				{
					vertices[i].textureCoordinate.x = m_Scale.x;
				}
				if (vertices[i].textureCoordinate.y >= 1.0f)
				{
					vertices[i].textureCoordinate.y = m_Scale.y;
				}
			}
			if (static_cast<Vec3>(
				XMVector3AngleBetweenNormals(
					static_cast<XMVECTOR>(vertices[i].normal),
					static_cast<XMVECTOR>(Vec3(0.0f, 0.0f, 1.0f)))).x < 0.01f
				)
			{
				//奥
				if (vertices[i].textureCoordinate.x >= 1.0f)
				{
					vertices[i].textureCoordinate.x = m_Scale.x;
				}
				if (vertices[i].textureCoordinate.y >= 1.0f)
				{
					vertices[i].textureCoordinate.y = m_Scale.y;
				}
			}
		}
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);

		//描画コンポーネントテクスチャの設定
		PtrDraw->SetTextureResource(L"NETWORK_TX");
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		AddTag(L"Wall");
	}
	void Wall::OnUpdate()
	{

	}

	void Wall::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		//for (auto v : OtherVec)
		//{
		//	//殻が当たったら
		//	if (v->FindTag(L"Shell"))
		//	{
		//		if (v->FindTag(L"Ballon"))
		//		{
		//			GetStage()->RemoveGameObject<SpeechBalloon>(v->GetThis<Shell>()->GetSpeechballoon().lock());
		//			for (int i = 0; i < 7; i++)
		//			{
		//				GetStage()->RemoveGameObject<GameObject>(v->GetThis<Shell>()->GetSpeechballoon().lock()->GetThis<SpeechBalloon>()->m_UI[i].lock());
		//			}

		//			for (int j = 0; j < static_cast<int>(v->GetThis<BallonShell>()->GetBallonChild().size()); j++)
		//			{
		//				GetStage()->RemoveGameObject<GameObject>(v->GetThis<BallonShell>()->GetBallonChild()[j].lock());
		//			}
		//			GetStage()->RemoveGameObject<Shell>(v);
		//			auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
		//			group->Remove(v);
		//		}
		//		else
		//		{
		//			GetStage()->RemoveGameObject<SpeechBalloon>(v->GetThis<Shell>()->GetSpeechballoon().lock());
		//			for (int i = 0; i < 7; i++)
		//			{
		//				GetStage()->RemoveGameObject<GameObject>(v->GetThis<Shell>()->GetSpeechballoon().lock()->GetThis<SpeechBalloon>()->m_UI[i].lock());
		//			}
		//			GetStage()->RemoveGameObject<Shell>(v);
		//			auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
		//			group->Remove(v);
		//		}
		//	}
		//}
	}

	Pulley::Pulley(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale) :
		GameObject(StagePtr),m_Pos(StartPos),m_Scale(StartScale)
	{}
	float Pulley::Distance(float x, float z, float x2, float z2)
	{
		auto SP = ((x2 - x) * (x2 - x) + (z2 - z) * (z2 - z));
		return  SP;
	}

	void Pulley::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		//コリジョンを付ける
		auto PtrCol = AddComponent<CollisionObb>();
		//描画
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetDiffuse(Col4(1.0f, 1.0f, 0.0f, 1.0f));

		auto PtrAction = AddComponent<Action>();
		//PtrAction->AddMoveBy(5.0f, Vec3(0.0f, 3.0f, 0.0f));
		//PtrAction->AddMoveBy(5.0f, Vec3(0.0f, -3.0f, 0.0f));

		PtrAction->SetLooped(true);

		PtrAction->Run();

	}
	void Pulley::OnUpdate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos.x, GetComponent<Transform>()->GetPosition().y, m_Pos.z);
		//auto shad = GetStage()->GetSharedGameObject<Player>(L"Player");
		//auto obj = shad->GetObj();
	}
	void Pulley::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Player"))
			{
				auto TransPos = GetComponent<Transform>()->GetPosition();
				auto TransSle = GetComponent<Transform>()->GetScale();
				auto Shad = GetStage()->GetSharedGameObject<Player>(L"Player");
				auto transPos = Shad->GetComponent<Transform>()->GetPosition();
				auto transSle = Shad->GetComponent<Transform>()->GetScale();
				auto OBJ = Shad->GetMyShell();
				auto DisPos = Distance(TransPos.x, TransPos.z, transPos.x, transPos.z);

				if (OBJ.lock() != nullptr)
				{
					if (DisPos <= TransSle.x)
					{
						auto Weight = OBJ.lock()->GetThis<Shell>()->GetWeight();
						if (Weight >= Weight::m_Heavy)
						{
							if (DisPos >= TransSle.y)
							{
							m_Frag = true;
							}
						}
					}
				}
			}
		}
	}

	void Pulley::OnCollisionExit(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Player"))
			{
				auto TransPos = GetComponent<Transform>()->GetPosition();
				auto TransSle = GetComponent<Transform>()->GetScale();
				auto Shad = GetStage()->GetSharedGameObject<Player>(L"Player");
				auto transPos = Shad->GetComponent<Transform>()->GetPosition();
				auto transSle = Shad->GetComponent<Transform>()->GetScale();
				auto OBJ = Shad->GetMyShell();
				auto DisPos = Distance(TransPos.x, TransPos.z, transPos.x, transPos.z);

				if (DisPos >= transSle.x)
				{
					m_Frag = false;
				}
			}
		}
	}

	PulleyPoint::PulleyPoint(const shared_ptr<Stage>& StagePtr, const shared_ptr<Pulley>PulleyPointptr, const shared_ptr<Pulley>PulleyPointptr2) :
		GameObject(StagePtr), m_point(PulleyPointptr), m_point2(PulleyPointptr2)
	{}
	void PulleyPoint::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		auto PtrRigid = AddComponent<Rigidbody>();
		PtrTrans->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
		PtrTrans->SetScale(Vec3(0.0f, 0.0f, 0.0f));
		PtrTrans->SetRotation(Vec3(0.0f, 0.0f, 0.0f));

		//コリジョンを付ける
		auto PtrCol = AddComponent<CollisionObb>();

		//描画
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//PtrDraw->SetDiffuse(Col4(1.0f, 1.0f, 0.0f, 1.0f));

	}

	void PulleyPoint::OnUpdate()
	{
		if (m_point.lock()->m_Frag == true)
		{
			auto PtrAction = m_point.lock()->GetComponent<Action>();
			PtrAction->AddMoveBy(3.0f, Vec3(0.0f, -3.0f, 0.0f));
			PtrAction->Run();

			auto PtrAction2 = m_point2.lock()->GetComponent<Action>();
			PtrAction2->AddMoveBy(3.0f, Vec3(0.0f, 3.0f, 0.0f));
			PtrAction2->Run();
		}
		if (m_point2.lock()->m_Frag == true)
		{
			auto PtrAction = m_point.lock()->GetComponent<Action>();
			PtrAction->AddMoveBy(3.0f, Vec3(0.0f, 3.0f, 0.0f));
			PtrAction->Run();

			auto PtrAction2 = m_point2.lock()->GetComponent<Action>();
			PtrAction2->AddMoveBy(3.0f, Vec3(0.0f, -3.0f, 0.0f));
			PtrAction2->Run();
		}
	}


	//動く床
	MoveGround::MoveGround(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& Move, const float speed) :
			GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Move(Move), m_Speed(speed)
	{}
	void MoveGround::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(Qt);
		//コリジョンを付ける
		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetFixed(true);
		//描画
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"MOVEGROUND_TX");
		PtrDraw->SetOwnShadowActive(true);

		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(m_Speed, Vec3(m_Move));
		PtrAction->AddMoveBy(m_Speed, Vec3(m_Move.x * -1.0f, m_Move.y * -1.0f, m_Move.z * -1.0f));
		PtrAction->SetLooped(true);
		PtrAction->Run();

		AddTag(L"MoveGround");
		AddTag(L"Wall");
	}

	void MoveGround::OnUpdate()
	{

	}

	SpeechBalloon::SpeechBalloon(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale,const shared_ptr<GameObject>& Shell) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale),m_Obj(Shell)
	{}

	void SpeechBalloon::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(Vec3(1.6f, 0.9f, 1.0f) * m_Scale.x);
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);


		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SQUARE");
		PtrDraw->SetTextureResource(L"Speech balloon2_TX");
		PtrDraw->SetDepthStencilState(DepthStencilState::Read);

		SetAlphaActive(true);
		SetDrawActive(false);

		auto ratio = 1.3f;

		//文字
		//能力の文字
		m_UI[0] = GetStage()->AddGameObject<Word>(Vec3(m_Pos.x - 0.7f, m_Pos.y + 0.3f, m_Pos.z - 0.15f), Vec3(ratio), L"Ability_TX");
		m_UI[0].lock()->GetComponent<Transform>()->SetParent(GetThis<SpeechBalloon>());

		//コロン
		m_UI[1] = GetStage()->AddGameObject<Colon>(Vec3(m_Pos.x - 0.1f, m_Pos.y + 0.3f, m_Pos.z - 0.15f), Vec3(ratio), L"Colon_TX");
		m_UI[1].lock()->GetComponent<Transform>()->SetParent(GetThis<SpeechBalloon>());

		//重さの文字
		m_UI[2] = GetStage()->AddGameObject<Word>(Vec3(m_Pos.x - 0.65f, m_Pos.y - 0.1f, m_Pos.z - 0.25f), Vec3(ratio), L"Weight_TX");
		m_UI[2].lock()->GetComponent<Transform>()->SetParent(GetThis<SpeechBalloon>());

		//コロン
		m_UI[3] = GetStage()->AddGameObject<Colon>(Vec3(m_Pos.x - 0.1f, m_Pos.y - 0.1f, m_Pos.z - 0.15f), Vec3(ratio), L"Colon_TX");
		m_UI[3].lock()->GetComponent<Transform>()->SetParent(GetThis<SpeechBalloon>());

		//能力
		m_UI[4] = GetStage()->AddGameObject<TypeUI>(Vec3(m_Pos.x + 0.6f, m_Pos.y + 0.3f, m_Pos.z - 0.15f), Vec3(ratio), getObj().lock(), false);
		m_UI[4].lock()->GetComponent<Transform>()->SetParent(GetThis<SpeechBalloon>());

		//重さ
		m_UI[5] = GetStage()->AddGameObject<WeightUI>(Vec3(m_Pos.x + 0.6f, m_Pos.y - 0.1f, m_Pos.z - 0.25f), Vec3(ratio), getObj().lock(), false);
		m_UI[5].lock()->GetComponent<Transform>()->SetParent(GetThis<SpeechBalloon>());

		//Xボタン
		m_UI[6] = GetStage()->AddGameObject<PlayerSpeech>(Vec3(m_Pos.x, m_Pos.y + 0.75f, m_Pos.z - 0.25f), Vec3(0.6f));
		m_UI[6].lock()->GetComponent<Transform>()->SetParent(GetThis<SpeechBalloon>());
	}
	void SpeechBalloon::OnUpdate()
	{
		auto Transptr = GetComponent<Transform>();
		auto transPos = Transptr->GetPosition();
		auto transSle = Transptr->GetScale();
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto playerTransPos = player->GetComponent<Transform>()->GetPosition();
		auto playerTransSle = player->GetComponent<Transform>()->GetScale();
		auto DisPos = DistancePtr(transPos.x, transPos.z, playerTransPos.x, playerTransPos.z);

		float distance = 5.0f;

		if (DisPos <= distance && !m_Obj.lock()->GetThis<Shell>()->GetBurdened())
		{
			SetDrawActive(true);
			for (int i = 0; i < 7; i++)
			{
				m_UI[i].lock()->SetDrawActive(true);
			}
		}
		else if(DisPos >= distance && !m_Obj.lock()->GetThis<Shell>()->GetBurdened())
		{
			SetDrawActive(false);
			for (int i = 0; i < 7; i++)
			{
				m_UI[i].lock()->SetDrawActive(false);
			}
		}
		else if (m_Obj.lock()->GetThis<Shell>()->GetBurdened())
		{
			SetDrawActive(false);
			for (int i = 0; i < 7; i++)
			{
				m_UI[i].lock()->SetDrawActive(false);
			}
		}

		auto shellPos = m_Obj.lock()->GetComponent<Transform>()->GetPosition();
		GetComponent<Transform>()->SetPosition(shellPos.x, shellPos.y + 1.3f, shellPos.z);

		auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		auto pos = GetComponent<Transform>()->GetPosition();
		
		//角度の調整
		Quat qtXZ;
		qtXZ.rotation(camera->GetRadXZ(), Vec3(0.0f, 1.0f, 0.0f));
		GetComponent<Transform>()->SetQuaternion(qtXZ);
	}


	Word::Word(const shared_ptr<Stage>& Stageptr, const Vec3& StartPos, const Vec3& StartScale, const wstring texture) :
		GameObject(Stageptr), m_pos(StartPos), m_Scale(StartScale), m_texture(texture)
	{}
	void Word::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_pos);
		PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x, 354.0f / 1000.0f * m_Scale.x, 1.0f);
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SQUARE");
		PtrDraw->SetTextureResource(m_texture);
		PtrDraw->SetDepthStencilState(DepthStencilState::Read);

		SetAlphaActive(true);
		SetDrawActive(false);
	}

	void Word::OnUpdate()
	{

	}

	TypeUI::TypeUI(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale, const shared_ptr<GameObject>& shell, const bool flag) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_point(shell)
	{}
	void TypeUI::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SQUARE");
		PtrDraw->SetDepthStencilState(DepthStencilState::Read);

		if (m_point.lock() != nullptr)
		{
			auto PtrShell = m_point.lock()->GetThis<Shell>()->GetName();

			if (PtrShell == L"Dash")
			{
				PtrDraw->SetTextureResource(L"Dash_TX");
				PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x * 1.0f, 354.0f / 1000.0f * m_Scale.x * 1.0f, 1.0f);
			}
			else if (PtrShell == L"Temp")
			{
				PtrDraw->SetTextureResource(L"None_TX");
				PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x * 1.0f, 354.0f / 1000.0f * m_Scale.x * 1.0f, 1.0f);
			}
			else if (PtrShell == L"Spin")
			{
				PtrDraw->SetTextureResource(L"Hovering_TX");
				PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x * 0.8f, 354.0f / 1000.0f * m_Scale.x * 0.8f, 1.0f);
			}
			else if (PtrShell == L"Ballon")
			{
				PtrDraw->SetTextureResource(L"Maueuku_TX");
				PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x * 0.7f, 354.0f / 1000.0f * m_Scale.x * 0.7f, 1.0f);
			}
			else if (PtrShell == L"Iron")
			{
				PtrDraw->SetTextureResource(L"None_TX");
				PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x * 1.0f, 354.0f / 1000.0f * m_Scale.x * 1.0f, 1.0f);
			}
		}
		else
		{
			PtrDraw->SetTextureResource(L"None_TX");
		}

		SetAlphaActive(true);
		SetDrawActive(false);
	}

	void TypeUI::OnUpdate()
	{
		if (m_flag)
		{
			auto PtrDraw = GetComponent<PNTStaticDraw>();
			auto PtrTrans = GetComponent<Transform>();

			if (m_point.lock() != nullptr)
			{
				auto PtrShell = m_point.lock()->GetThis<Shell>()->GetName();

				if (PtrShell == L"Dash")
				{
					PtrDraw->SetTextureResource(L"Dash_TX");
					PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x * 1.0f, 354.0f / 1000.0f * m_Scale.x * 1.0f, 1.0f);
				}
				else if (PtrShell == L"Temp")
				{
					PtrDraw->SetTextureResource(L"None_TX");
					PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x * 1.0f, 354.0f / 1000.0f * m_Scale.x * 1.0f, 1.0f);
				}
				else if (PtrShell == L"Spin")
				{
					PtrDraw->SetTextureResource(L"Hovering_TX");
					PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x * 0.8f, 354.0f / 1000.0f * m_Scale.x * 0.8f, 1.0f);
				}
				else if (PtrShell == L"Ballon")
				{
					PtrDraw->SetTextureResource(L"Maueuku_TX");
					PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x * 0.7f, 354.0f / 1000.0f * m_Scale.x * 0.7f, 1.0f);
				}
				else if (PtrShell == L"Iron")
				{
					PtrDraw->SetTextureResource(L"None_TX");
					PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x * 1.0f, 354.0f / 1000.0f * m_Scale.x * 1.0f, 1.0f);
				}
			}
			else
			{
				PtrDraw->SetTextureResource(L"None_TX");
				PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x * 1.0f, 354.0f / 1000.0f * m_Scale.x * 1.0f, 1.0f);
			}
		}
	}

	WeightUI::WeightUI(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale, const shared_ptr<GameObject>& shell, const bool flag) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_point(shell)
	{}
	void WeightUI::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(1181.0f / 1000.0f * m_Scale.x, 354.0f / 1000.0f * m_Scale.x, 1.0f);
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SQUARE");
		PtrDraw->SetDepthStencilState(DepthStencilState::Read);

		if (m_point.lock() != nullptr)
		{
			auto PtrWeight = m_point.lock()->GetThis<Shell>()->GetWeight();

			switch (PtrWeight)
			{
			case basecross::Weight::m_VeryLight:
				break;
			case basecross::Weight::m_Light:
				PtrDraw->SetTextureResource(L"Weightlight_TX");
				break;
			case basecross::Weight::m_NomalWeight:
				PtrDraw->SetTextureResource(L"Weightnormal_TX");
				break;
			case basecross::Weight::m_Heavy:
				PtrDraw->SetTextureResource(L"Weightheavy_TX");
				break;
			case basecross::Weight::m_VeryHeavy:
				break;
			default:
				break;
			}
		}
		else
		{
			PtrDraw->SetTextureResource(L"None_TX");
		}

		SetAlphaActive(true);
		SetDrawActive(false);
	}

	void WeightUI::OnUpdate()
	{
		if (m_flag)
		{
			auto PtrDraw = GetComponent<PNTStaticDraw>();
			if (m_point.lock() != nullptr)
			{

				auto PtrWeight = m_point.lock()->GetThis<Shell>()->GetWeight();

				switch (PtrWeight)
				{
				case basecross::Weight::m_VeryLight:
					break;
				case basecross::Weight::m_Light:
					PtrDraw->SetTextureResource(L"Weightlight_TX");
					break;
				case basecross::Weight::m_NomalWeight:
					PtrDraw->SetTextureResource(L"Weightnormal_TX");
					break;
				case basecross::Weight::m_Heavy:
					PtrDraw->SetTextureResource(L"Weightheavy_TX");
					break;
				case basecross::Weight::m_VeryHeavy:
					break;
				default:
					break;
				}
			}
			else
			{
				PtrDraw->SetTextureResource(L"None_TX");
			}
		}
	}


	Colon::Colon(const shared_ptr<Stage>& Stageptr, const Vec3& StartPos, const Vec3& StartScale, const wstring Texture) :
		GameObject(Stageptr), m_Pos(StartPos), m_Scale(StartScale), m_Texture(Texture)
	{}
	void Colon::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(354.0f / 1000.0f * m_Scale.x, 354.0f / 1000.0f * m_Scale.x, 1.0f);
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SQUARE");
		PtrDraw->SetTextureResource(m_Texture);
		PtrDraw->SetDepthStencilState(DepthStencilState::Read);

		SetAlphaActive(true);
		SetDrawActive(false);
	}

	void Colon::OnUpdate()
	{
	}

	//透明な壁
	ClearWall::ClearWall(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{}
	void ClearWall::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_Pos);
		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetRotation(Vec3(0));

		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetFixed(true);

		AddTag(L"Wall");
	}
	void ClearWall::OnUpdate()
	{

	}


	PlayerSpeech::PlayerSpeech(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{}

	void PlayerSpeech::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(Vec3(1.0f * m_Scale.x, 1.0f * m_Scale.x, 1.0f));
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);


		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SQUARE");
		PtrDraw->SetTextureResource(L"XBUTTON_TX");
		PtrDraw->SetDepthStencilState(DepthStencilState::Read);
		SetAlphaActive(true);
		SetDrawActive(false);
	}
	void PlayerSpeech::OnUpdate()
	{

	}

	//スロープ
	Slope::Slope(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale, const Vec3 & StartRot):
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Rot(StartRot)
	{}

	void Slope::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_Pos);
		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetRotation(m_Rot);

		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetFixed(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetTextureResource(L"CONKURI_TX");

		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");

		//描画コンポーネントテクスチャの設定
		//DrawComp->SetTextureResource(L"BEACH_TX");


		AddTag(L"Slope");
		AddTag(L"Wall");
	}

	void Slope::OnUpdate()
	{
	}

	StaticObject::StaticObject(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale, const Vec3 & StartRot) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Rot(StartRot)
	{}

	void StaticObject::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_Pos);
		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetRotation(m_Rot);

		//auto ptrCol = AddComponent<CollisionObb>();
		//ptrCol->SetFixed(true);

		//auto ptrDraw = AddComponent<PNTStaticDraw>();
		//ptrDraw->SetTextureResource(L"CONKURI_TX");

		//ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void StaticObject::OnUpdate()
	{
	}


	//チェックポイント
	CheckPoint::CheckPoint(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{}

	void CheckPoint::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_Pos);
		ptrTrans->SetScale(m_Scale);
		
		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetIsHitAction(IsHitAction::None);
	}

	void CheckPoint::OnUpdate()
	{
	}

	void CheckPoint::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Player"))
			{
				auto scene = App::GetApp()->GetScene<Scene>();
				if (!scene->GetCheckPoint())
				{
					scene->SetCheckPoint(true);
					scene->SetPlayerPos(v->GetComponent<Transform>()->GetPosition());
				}
			}
		}
	}


	BackGround::BackGround(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale):
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{}

	void BackGround::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_Pos);
		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetRotation(Vec3(0));

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//ptrDraw->SetTextureResource(L"CONKURI_TX");
		ptrDraw->SetTextureResource(L"MOVEGROUND_TX");
		//ptrDraw->SetTextureResource(L"BlueWall_TX");
	}

	void BackGround::OnUpdate()
	{

	}
};
