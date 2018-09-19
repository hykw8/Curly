/*!
@file DebugStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"
#include "Enemy.h"

namespace basecross {

	// ComeEnemy
	ComeEnemy::ComeEnemy(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& StartRotaion) :
		Enemy(StagePtr),
		m_Pos(StartPos),
		m_Scale(StartScale),
		m_Rot(StartRotaion),
		m_speed(1.0f),
		m_TotalTime(0.0f),
		m_Offset(0.2f)
	{}

	ComeEnemy::~ComeEnemy() {}

	void ComeEnemy::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(m_Rot);

		Mat4x4 mat;
		mat.affineTransformation
		(
			Vec3(0.3f, 0.3f, 0.3f),//scale
			Vec3(0.0f, 0.0f, 0.0f),//rotationの原点
			Vec3(0.0f, 0.0f, 0.0f),//rotation
								   //Vec3(0.0f, 0.0f, 0.0f),//rotation
			Vec3(0.0f, -0.4f, -0.3f)//position
		);

		//衝突判定
		auto PtrCol = AddComponent<CollisionObb>();

		//Fixdタイプ
		AddComponent<Rigidbody>();
		auto PtrRigid = AddComponent<Rigidbody>();
		PtrRigid->SetGravityVelocity(Vec3(0.0f, -3.8f, 0.0f));
		//影
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CONE");

		//描画コンポーネント
		auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
		//メッシュ
		PtrDraw->SetMeshResource(L"Curly");
		PtrDraw->SetMeshToTransformMatrix(mat);
		//透明処理
		SetAlphaActive(true);

		////ステートマシン 使ってます
		m_StateMachine.reset(new StateMachine<Enemy>(GetThis<ComeEnemy>()));
		////最初のステートをEnemyDefaultStateに設定
		m_StateMachine->ChangeState(EnemyDefaultState::Instance());
		
		ShellSet();
	}


	void ComeEnemy::OnUpdate()
	{
		m_StateMachine->Update();

		auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (!camera->GetStartFlag())
		{
			ComeEnemy::PlayerChase();
			if (GetPlaFlag())
			{
				auto time = App::GetApp()->GetElapsedTime();
				SetTotalTime(GetTotalTime() + time);
				if (GetTotalTime() <= 0.5f)
				{
					GetComponent<Rigidbody>()->SetVelocity(GetCrash() * 0.9f);
				}
				else
				{
					SetTotalTime(0.0f);
					GetPlayer().reset();
					SetPlaFlag(false);
				}
			}

			MyDirection();
		}
	}

	void ComeEnemy::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Player"))
			{
				//ノックバック
				SetPlayer(v);
				SetPlaFlag(true);
				SetPairShell(v->GetThis<Player>()->GetMyShell().lock());
				SetCrash(Crash(GetComponent<Transform>()->GetPosition(), v));
			}
			else if (v->FindTag(L"Enemy"))
			{
				//ノックバック
				SetPlayer(v);
				SetPlaFlag(true);
				SetPairShell(v->GetThis<Enemy>()->GetMyShell().lock());
				SetCrash(Crash(GetComponent<Transform>()->GetPosition(), v));
			}
		}
	}

	void ComeEnemy::PlayerChase()
	{
		auto PtrTrans = GetComponent<Transform>();
		Vec3 MyPos = PtrTrans->GetPosition();
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		Vec3 PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();

		Vec3 velo = PlayerPos - MyPos;
		velo.normalize();
		auto PtrRigid = GetComponent<Rigidbody>();
		PtrRigid->SetVelocity(velo * m_speed);

	}

	void ComeEnemy::MyDirection()
	{
		auto trans = GetComponent<Transform>();
		auto pos = trans->GetPosition();
		auto playerPos = GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
		m_Direction = -atan2f(pos.z - playerPos.z, pos.x - playerPos.x);
		trans->SetRotation(0.0f, m_Direction + 90.0f * XM_PI / 180.0f, 0.0f);
	}

	void ComeEnemy::ShellSet()
	{
		auto PtrTrans = GetComponent<Transform>();
		m_ShellArea = GetStage()->AddGameObject<InvisibleShell>(Vec3(PtrTrans->GetPosition().x, PtrTrans->GetPosition().y + m_Scale.y / 2.0f, PtrTrans->GetPosition().z + m_Scale.z / 2.0f + m_Offset), Vec3(1.0f, 1.0f, 1.0f), Vec3(45.0f * XM_PI / 180.0f, 0.0f, 0.0f));
		m_ShellArea.lock()->GetComponent<Transform>()->SetParent(GetThis<ComeEnemy>());

		m_obj = GetStage()->AddGameObject<TempShell>(Vec3(m_ShellArea.lock()->GetComponent<Transform>()->GetPosition()), Vec3(1.0f, 1.0f, 1.0f));
		m_obj.lock()->GetComponent<CollisionObb>()->AddExcludeCollisionTag(L"Shell");
		m_obj.lock()->GetComponent<CollisionObb>()->AddExcludeCollisionTag(L"Wall");
		m_obj.lock()->GetComponent<CollisionObb>()->AddExcludeCollisionTag(L"Floor");
		m_obj.lock()->GetComponent<CollisionObb>()->AddExcludeCollisionTag(L"Enemy");
		m_obj.lock()->GetComponent<CollisionObb>()->SetIsHitAction(IsHitAction::None);
		m_obj.lock()->GetComponent<Rigidbody>()->SetGravityVelocityZero();
		m_obj.lock()->GetComponent<Rigidbody>()->SetVelocityZero();
		m_obj.lock()->GetComponent<Transform>()->SetPosition(m_ShellArea.lock()->GetComponent<Transform>()->GetWorldPosition());
		m_obj.lock()->GetComponent<Transform>()->SetRotation(m_ShellArea.lock()->GetComponent<Transform>()->GetRotation().x, 0.0f, 0.0f);
		m_obj.lock()->GetComponent<Transform>()->SetScale(m_obj.lock()->GetThis<Shell>()->GetStartScale());
		m_obj.lock()->GetComponent<Transform>()->SetParent(GetThis<ComeEnemy>());
		m_obj.lock()->SetEnemy(true);

		SetMyShell(m_obj.lock()->GetThis<Shell>());
	}


	IMPLEMENT_SINGLETON_INSTANCE(EnemyDefaultState)
		void EnemyDefaultState::Enter(const shared_ptr<Enemy>& Obj)
	{
	}
	void EnemyDefaultState::Execute(const shared_ptr<Enemy>& Obj)
	{
	}
	void EnemyDefaultState::Exit(const shared_ptr<Enemy>& Obj)
	{

	}


	// 新しいヤドカリ
	NewHermitCrab::NewHermitCrab(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& StartRotaion) :
		Enemy(StagePtr),
		m_Pos(StartPos),
		m_Scale(StartScale),
		m_Rot(StartRotaion),
		m_speed(5.0f),
		m_TotalTime(0.0f),
		m_Offset(0.2f)

	{
		m_SarchDistance = 10.0f;
	}

	NewHermitCrab::~NewHermitCrab() {}

	void NewHermitCrab::OnCreate()
	{
		firstPos = m_Pos;

		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(m_Rot);

		Mat4x4 mat;
		mat.affineTransformation
		(
			Vec3(0.3f, 0.3f, 0.3f),//scale
			Vec3(0.0f, 0.0f, 0.0f),//rotationの原点
			Vec3(0.0f, 0.0f, 0.0f),//rotation
			Vec3(0.0f, -0.4f, -0.3f)//position
		);

		//衝突判定
		auto PtrCol = AddComponent<CollisionObb>();
		//PtrCol->SetFixed(true);

		//Fixdタイプ
		AddComponent<Rigidbody>();
		//PtrCol->SetFixed(true);
		auto PtrRigid = AddComponent<Rigidbody>();
		PtrRigid->SetGravityVelocity(Vec3(0.0f, -3.8f, 0.0f));
		//影
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネント
		auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
		//メッシュ
		PtrDraw->SetMeshResource(L"Curly");
		PtrDraw->SetMeshToTransformMatrix(mat);
		PtrDraw->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));
		//テクスチャ
		//PtrDraw->SetTextureResource(L"PLAYER_TX");
		//透明処理
		SetAlphaActive(true);

		////ステートマシン 使ってます
		m_StateMachine.reset(new StateMachine<Enemy>(GetThis<NewHermitCrab>()));
		////最初のステートをEnemyDefaultStateに設定
		m_StateMachine->ChangeState(EnemyDefaultState::Instance());
		ShellSet();
		//m_obj = GetStage()->AddGameObject<TempShell>(Vec3(m_Pos.x, m_Pos.y + 1.0f + 0.1f, m_Pos.z), Vec3(1.0f, 1.0f, 1.0f));
		//m_obj.lock()->GetComponent<Transform>()->SetParent(GetThis<NewHermitCrab>());
		//m_obj.lock()->SetEnemy(true);

		//SetMyShell(m_obj.lock()->GetThis<Shell>());
	}


	void NewHermitCrab::OnUpdate()
	{
		//PlayerChase();
		//GoFromBehavior();
		m_StateMachine->Update();

		auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (!camera->GetStartFlag())
		{
			StatusControl();
			if (GetPlaFlag())
			{
				auto time = App::GetApp()->GetElapsedTime();
				SetTotalTime(GetTotalTime() + time);
				if (GetTotalTime() <= 0.5f)
				{
					GetComponent<Rigidbody>()->SetVelocity(GetCrash() * 0.9f);
				}
				else
				{
					SetTotalTime(0.0f);
					GetPlayer().reset();
					SetPlaFlag(false);
				}
			}

			MyDirection();
		}
	}

	float NewHermitCrab::Length_EToP() {
		auto PtrTrans = GetComponent<Transform>();
		Vec3 MyPos = PtrTrans->GetPosition();
		Vec3 PlayerPos = GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
		return length(MyPos - PlayerPos);
	}




	void NewHermitCrab::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Player"))
			{
				SetPlayer(v);
				SetPlaFlag(true);
				SetPairShell(v->GetThis<Player>()->GetMyShell().lock());
				SetCrash(Crash(GetComponent<Transform>()->GetPosition(), v));
			}
			else if (v->FindTag(L"Enemy"))
			{
				//ノックバック
				SetPlayer(v);
				SetPlaFlag(true);
				SetPairShell(v->GetThis<Enemy>()->GetMyShell().lock());
				SetCrash(Crash(GetComponent<Transform>()->GetPosition(), v));
			}
		}
	}

	// 徘徊する
	void NewHermitCrab::GoFromBehavior()
	{
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0.0f;
		}
		Vec3 Pos = GetComponent<Transform>()->GetPosition();
		Pos.x = sin(m_TotalTime);
		Pos.z = cos(m_TotalTime);
		GetComponent<Rigidbody>()->SetVelocity(Pos.x, 0.0f, Pos.z);

	}

	// 元の位置に戻る
	void NewHermitCrab::PosReturn() {
		auto PtrTrans = GetComponent<Transform>();
		Vec3 HermitPos = PtrTrans->GetPosition();
		Vec3 velo = firstPos - HermitPos;
		velo.normalize();
		auto PtrRigid = GetComponent<Rigidbody>();
		PtrRigid->SetVelocity(velo * m_speed);
	}

	// プレイヤーを追いかける
	void NewHermitCrab::PlayerChase()
	{
		auto PtrTrans = GetComponent<Transform>();
		Vec3 HermitPos = PtrTrans->GetPosition();
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		Vec3 PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		Vec3 velo = PlayerPos - HermitPos;
		velo.normalize();
		auto PtrRigid = GetComponent<Rigidbody>();
		PtrRigid->SetVelocity(velo * m_speed);

		if (Length_EToP() >= m_SarchDistance) {
			PosReturn();
		}
	}

	void NewHermitCrab::StatusControl()
	{

		if (Length_EToP() < m_SarchDistance) {
			Status = ChasesState;
		}
		else
			if ((m_Pos.x <= firstPos.x + 3.0f) && (m_Pos.x >= firstPos.x - 3.0f) &&
				(m_Pos.z <= firstPos.z + 3.0f) && (m_Pos.z >= firstPos.z - 3.0f)&&
				(m_Pos.y < firstPos.y))
			{
				Status = GoFormState;

			}
	
		switch (Status)
		{
		case GoFormState:			
			GoFromBehavior();
			break;
		case ChasesState:
			PlayerChase();
			break;
		case PosReturnState:
			PosReturn();
			break;
		}
	}

	void NewHermitCrab::MyDirection()
	{
		auto trans = GetComponent<Transform>();
		auto pos = trans->GetPosition();
		auto playerPos = GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
		m_Direction = -atan2f(pos.z - playerPos.z, pos.x - playerPos.x);
		trans->SetRotation(0.0f, m_Direction + 90.0f * XM_PI / 180.0f, 0.0f);
	}

	void NewHermitCrab::ShellSet()
	{
		auto PtrTrans = GetComponent<Transform>();
		m_ShellArea = GetStage()->AddGameObject<InvisibleShell>(Vec3(PtrTrans->GetPosition().x, PtrTrans->GetPosition().y + m_Scale.y / 2.0f, PtrTrans->GetPosition().z + m_Scale.z / 2.0f + m_Offset), Vec3(1.0f, 1.0f, 1.0f), Vec3(45.0f * XM_PI / 180.0f, 0.0f, 0.0f));
		m_ShellArea.lock()->GetComponent<Transform>()->SetParent(GetThis<NewHermitCrab>());

		m_obj = GetStage()->AddGameObject<TempShell>(Vec3(m_ShellArea.lock()->GetComponent<Transform>()->GetPosition()), Vec3(1.0f, 1.0f, 1.0f));
		m_obj.lock()->GetComponent<CollisionObb>()->AddExcludeCollisionTag(L"Shell");
		m_obj.lock()->GetComponent<CollisionObb>()->AddExcludeCollisionTag(L"Wall");
		m_obj.lock()->GetComponent<CollisionObb>()->AddExcludeCollisionTag(L"Floor");
		m_obj.lock()->GetComponent<CollisionObb>()->AddExcludeCollisionTag(L"Enemy");
		m_obj.lock()->GetComponent<CollisionObb>()->SetIsHitAction(IsHitAction::None);
		m_obj.lock()->GetComponent<Rigidbody>()->SetGravityVelocityZero();
		m_obj.lock()->GetComponent<Rigidbody>()->SetVelocityZero();
		m_obj.lock()->GetComponent<Transform>()->SetPosition(m_ShellArea.lock()->GetComponent<Transform>()->GetWorldPosition());
		m_obj.lock()->GetComponent<Transform>()->SetRotation(m_ShellArea.lock()->GetComponent<Transform>()->GetRotation().x, 0.0f, 0.0f);
		m_obj.lock()->GetComponent<Transform>()->SetScale(m_obj.lock()->GetThis<Shell>()->GetStartScale());
		m_obj.lock()->GetComponent<Transform>()->SetParent(GetThis<NewHermitCrab>());
		m_obj.lock()->SetEnemy(true);

		SetMyShell(m_obj.lock()->GetThis<Shell>());
	}
}


	
