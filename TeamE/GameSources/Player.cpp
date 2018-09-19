/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	Player::Player
	(
		const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Pos
	) :

		HermitCrab(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Pos(Pos),
		m_Totaltime(0),
		m_Speed(10.0f),
		m_Gravity(Vec3(0.0f, -9.8f, 0.0f)),
		m_Offset(0.2f),
		m_IsFloor(false)
	{
		m_MaxSpeed = m_Speed;
	}

	void Player::OnCreate()
	{
		auto scene = App::GetApp()->GetScene<Scene>();
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);

		//�X�e�[�W10�̎��`�F�b�N�|�C���g
		if (scene->GetCheckPoint() && scene->GetStageNum() == 10)
		{
			PtrTransform->SetPosition(scene->GetPlayerPos());
		}
		else
		{
			PtrTransform->SetPosition(m_Pos);
		}
		Mat4x4 mat;
		mat.affineTransformation
		(
			Vec3(0.3f, 0.3f, 0.3f),//scale
			Vec3(0.0f, 0.0f, 0.0f),//rotation�̌��_
			Vec3(0.0f, 180.0f * XM_PI / 180.0f, 0.0f),//rotation
			Vec3(0.0f, -0.4f, 0.3f)//position
		);

		//�R���W����
		auto PtrSphere = AddComponent<CollisionSphere>();

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		// ���C���[�̓���������
		SetAlphaActive(true);

		auto PtrRegid = AddComponent<Rigidbody>();

		auto action = AddComponent<Action>();

		auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
		PtrDraw->SetMeshResource(L"Curly");
		PtrDraw->SetMeshToTransformMatrix(mat);

		m_StateMachine.reset(new LayeredStateMachine<Player>(GetThis<Player>()));
		m_StateMachine->Reset(PlayerDefaultState::Instance());

		auto PtrCamera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (PtrCamera)
		{
			PtrCamera->SetTargetObject(GetThis<Player>());
			PtrCamera->SetTargetToAt(Vec3(0.0f, 1.5f, 0.0f));
		}
		GetStage()->SetSharedGameObject(L"Player", GetThis<Player>());
		m_ShellArea = GetStage()->AddGameObject<InvisibleShell>(Vec3(PtrTransform->GetPosition().x, PtrTransform->GetPosition().y + m_Scale.y / 2.0f, PtrTransform->GetPosition().z - m_Scale.z / 2.0f - m_Offset), Vec3(1.0f, 1.0f, 1.0f), Vec3(-45.0f * XM_PI / 180.0f, 0.0f, 0.0f));
		m_ShellArea.lock()->GetComponent<Transform>()->SetParent(GetThis<Player>());

		GetStage()->AddGameObject<PlayerSpeech>(Vec3(PtrTransform->GetPosition().x, PtrTransform->GetPosition().y + 1.0f, PtrTransform->GetPosition().z), Vec3(3.0f, 1.0f, 1.0f));
		m_StartUI = GetStage()->AddGameObject<PressAnyButton>(Vec2(0.0f, 0.0f), Vec2(0.5f, 0.5f), L"PushBottom2W_TX");

		m_RemovableEff = GetStage()->AddGameObject<Removable>(Vec3(m_Pos), Vec3(20.0f, 3.0f, 20.0f));
		m_RemovableEff.lock()->SetDrawActive(false);

		AddTag(L"Player");

		// �G�t�F�N�g�̏�����
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring TestEffectStr = DataDir + L"Effect\\SangoEffect.efk";
		auto ShEfkInterface = GetTypeStage<GameStage>()->GetEfkInterface();
		m_EfkEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, TestEffectStr);
	}

	void Player::OnUpdate()
	{
		m_StateMachine->Update();
		m_InputHandler.Push(GetThis<Player>());

		CrashVelo();
		ChangeSpeed();
	}

	void Player::OnUpdate2()
	{
		Boundary();
	}

	void Player::ShellThrow()
	{
		//�k��w�����Ă���X�e�[�g��������
		if (GetStateMachine()->GetTopState() == PlayerBurdenedState::Instance())
		{
			auto playerTrans = GetComponent<Transform>();
			auto playerScale = playerTrans->GetScale();
			auto playerPos = playerTrans->GetPosition();
			auto velo = m_Direction;
			velo.normalize();

			auto trans = GetMyShell().lock()->GetComponent<Transform>();
			trans->ResetPosition(Vec3(trans->GetPosition().x, trans->GetPosition().y + 5.0f, trans->GetPosition().z));
			trans->SetRotation(Vec3(0));
			GetMyShell().lock()->SetTakePos(GetComponent<Transform>()->GetPosition());

			//�����蔻�������
			auto shellCol = GetMyShell().lock()->GetComponent<CollisionObb>();
			for (auto v : GetMyShell().lock()->GetThis<Shell>()->GetTagSet())
			{
				if (v != L"Shell")
				{
					GetComponent<CollisionSphere>()->RemoveExcludeCollisionTag(v);
				}
			}

			//�k�Ƒ��̂�̓����蔻�������
			shellCol->RemoveExcludeCollisionTag(L"Shell");
			shellCol->RemoveExcludeCollisionTag(L"Wall");
			shellCol->RemoveExcludeCollisionTag(L"Floor");
			GetMyShell().lock()->GetComponent<Rigidbody>()->SetVelocity(-velo.x, 5.0f, -velo.z);
			GetMyShell().lock()->GetComponent<Rigidbody>()->SetGravityVelocity(0.0f, -9.8f, 0.0f);

			//���D�������ꍇ
			if (!GetMyShell().lock()->FindTag(L"Ballon"))
			{
				GetMyShell().lock()->GetComponent<Transform>()->SetScale(GetMyShell().lock()->GetThis<Shell>()->GetStartScale() * GetMyShell().lock()->GetThis<Shell>()->GetScaleMagni());
			}

			//�e�q�֌W���O��
			GetMyShell().lock()->GetComponent<Transform>()->SetParent(nullptr);
			GetMyShell().lock()->GetThis<Shell>()->SetBurdened(false);
			GetMyShell().lock().reset();

			m_StateMachine->Reset(PlayerDefaultState::Instance());
		}
	}

	void Player::ShellBurdened(shared_ptr<GameObject>& shell)
	{
		m_StateMachine->Reset(PlayerBurdenedState::Instance());
		auto shellTrans = shell->GetThis<Shell>()->GetComponent<Transform>();

		//�k�Ɗk�𓖂���Ȃ��悤�ɂ���
		shell->GetThis<Shell>()->GetComponent<CollisionObb>()->AddExcludeCollisionTag(L"Shell");
		shell->GetThis<Shell>()->GetComponent<CollisionObb>()->AddExcludeCollisionTag(L"Wall");
		shell->GetThis<Shell>()->GetComponent<CollisionObb>()->AddExcludeCollisionTag(L"Floor");

		//�R���W�����𖳌�
		shell->GetThis<Shell>()->GetComponent<CollisionObb>()->SetIsHitAction(IsHitAction::None);
		shell->GetComponent<Rigidbody>()->SetGravityVelocityZero();
		shell->GetComponent<Rigidbody>()->SetVelocityZero();

		m_IsFloor = true;

		//�ʏ�̑傫����
		shell->GetComponent<Transform>()->SetScale(shell->GetThis<Shell>()->GetStartScale());

		shell->GetThis<Shell>()->SetBurdened(true);

		//�w���ɃZ�b�g
		shellTrans->SetPosition(m_ShellArea.lock()->GetComponent<Transform>()->GetWorldPosition());
		shellTrans->SetParent(GetThis<Player>());
		shellTrans->SetRotation(m_ShellArea.lock()->GetComponent<Transform>()->GetRotation().x, 0.0f, 0.0f);
		SetMyShell(shell->GetThis<Shell>());

		auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());

		m_RemovableEff.lock()->GetComponent<Transform>()->SetPosition(GetComponent<Transform>()->GetPosition());
		m_RemovableEff.lock()->GetComponent<Transform>()->SetScale(Vec3(3));
		m_RemovableEff.lock()->SetDrawActive(true);

		for (auto v : shell->GetThis<Shell>()->GetTagSet())
		{
			if (v != L"Shell")
			{
				//�v���C���[�Ɣw�����Ă���k�𓖂���Ȃ��悤�ɂ���
				GetComponent<CollisionSphere>()->AddExcludeCollisionTag(v);
			}
		}

		if (!camera->GetStartFlag())
		{
			m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
			m_AudioObjectPtr->AddAudioResource(L"Change");
			m_AudioObjectPtr->Start(L"Change", 0, 0.5f);
		}
	}

	void Player::ChangeSpeed()
	{
		//�k��w�����Ă�����
		if (GetMyShell().lock() != nullptr)
		{
			if (GetMyShell().lock()->GetThis<Shell>()->GetWeight() == Weight::m_Heavy)
			{
				m_Speed = 7.5f;
			}
			else
			{
				m_Speed = 10.0f;
			}
		}
		else
		{
			m_Speed = 10.0f;
		}
	}

	void Player::Boundary()
	{
		//���E������
		if (m_IsFloor && GetComponent<CollisionSphere>()->GetHitObjectVec().empty())
		{
			GetComponent<CollisionSphere>()->SetIsHitAction(IsHitAction::Auto);
			m_IsFloor = false;
		}
		else if (m_IsFloor && !GetComponent<CollisionSphere>()->GetHitObjectVec().empty())
		{
			for (auto v : GetComponent<CollisionSphere>()->GetHitObjectVec())
			{
				//WallTag�ɓ���������
				if (v->FindTag(L"Wall"))
				{
					HitWall(v);
				}
			}
		}
	}

	void Player::CrashVelo()
	{
		if (m_EneFlag)
		{
			auto time = App::GetApp()->GetElapsedTime();
			SetTotalTime(GetTotalTime() + time);
			if (GetTotalTime() <= 0.5f)
			{
				GetComponent<Rigidbody>()->SetVelocity(m_Crash.x * 0.9f, 0.0f, m_Crash.z * 0.9f);
			}
			else
			{
				SetTotalTime(0.0f);
				m_EneFlag = false;
			}
		}
	}

	void Player::HitWall(shared_ptr<GameObject>& obj)
	{
		auto ptrTrans = GetComponent<Transform>();
		auto ptrRigid = GetComponent<Rigidbody>();
		auto velo = ptrRigid->GetVelocity();
		SPHERE sp;
		sp.m_Center = ptrTrans->GetPosition();
		sp.m_Radius = 0.5f;
		float time = App::GetApp()->GetElapsedTime();

		auto ptrWallTrans = obj->GetComponent<Transform>();
		OBB obb(Vec3(1.0f, 1.0f, 1.0f), ptrWallTrans->GetWorldMatrix());
		float hitTime;
		if (HitTest::CollisionTestSphereObb(sp, velo, obb, 0, time, hitTime))
		{
			auto beforePos = ptrTrans->GetBeforePosition();
			auto hitPos = beforePos + velo * hitTime;
			sp.m_Center = hitPos;
			Vec3 ret;
			HitTest::SPHERE_OBB(sp, obb, ret);
			Vec3 normal = sp.m_Center - ret;
			normal.normalize();
			Vec3 newPos = ret + normal * sp.m_Radius;
			float otherTime = time - hitTime;
			Vec3 slideVelo;
			if (otherTime > 0.0f)
			{
				float len = dot(velo, normal);
				Vec3 contact = normal * len;
				slideVelo = velo - contact;
				newPos += slideVelo * otherTime;
			}
			ptrTrans->ResetPosition(newPos);
			velo = XMVector3Reflect(velo, normal);
			ptrRigid->SetVelocity(velo.x, 0.0f, velo.z);
		}
	}

	void Player::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto& v : OtherVec)
		{
			//�k�ɓ���������
			if (v->FindNumTag(1))
			{
				if (GetStateMachine()->GetTopState() == PlayerDefaultState::Instance())
				{
					ShellBurdened(v);
				}
			}

			//�G
			if (v->FindTag(L"Enemy"))
			{
				GetComponent<CollisionSphere>()->SetIsHitAction(IsHitAction::None);
				m_IsFloor = true;

				m_EneFlag = true;
				SetPairShell(v->GetThis<Enemy>()->GetMyShell().lock());
				m_Crash = Crash(GetComponent<Transform>()->GetPosition(), v);
			}

			//���k
			auto CntVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (GetMyShell().lock() != nullptr)
			{
				if (GetMyShell().lock()->GetThis<Shell>()->GetName() == L"Spin")
				{
					if (GetMyShell().lock()->GetThis<Shell>()->GetNoPush() && GetMyShell().lock()->GetThis<Shell>()->GetTotalTime() >= 1.0f)
					{
						GetMyShell().lock()->GetThis<Shell>()->SetTotalTime(0.0f);
					}
				}
			}

			//��
			if (v->FindTag(L"Floor"))
			{
				auto ptrCol = GetComponent<CollisionSphere>();
				auto ptrFloorCol = v->GetComponent<CollisionObb>();
				Vec3 retVec;
				ptrCol->GetHitNormal(ptrFloorCol, retVec);
				retVec.normalize();
				Vec3 angle(XMVector3AngleBetweenVectors(retVec, Vec3(0.0f, -1.0f, 0.0f)));
				if (angle.x <= 0.01f)
				{
					GetComponent<CollisionSphere>()->SetIsHitAction(IsHitAction::None);
					m_IsFloor = true;
					return;
				}
			}
		}
	}

	void Player::OnCollisionExcute(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Cube") || v->FindTag(L"MoveGround"))
			{
				if (GetMyShell().lock() != nullptr)
				{
					if (GetMyShell().lock()->GetThis<Shell>()->GetName() == L"Spin")
					{
						if (GetMyShell().lock()->GetThis<Shell>()->GetNoPush() && GetMyShell().lock()->GetThis<Shell>()->GetTotalTime() >= 1.0f)
						{
							GetMyShell().lock()->GetThis<Shell>()->SetTotalTime(0.0f);
						}
					}
				}
			}

			if (v->FindNumTag(1))
			{
				auto CntVec = App::GetApp()->GetInputDevice().GetControlerVec();
				if (CntVec[0].bConnected)
				{
					if (CntVec[0].wPressedButtons & XINPUT_GAMEPAD_X)
					{
						if (GetStateMachine()->GetTopState() == PlayerDefaultState::Instance())
						{
							ShellBurdened(v);
						}
						else
						{
							ShellThrow();
							ShellBurdened(v);
						}
					}
				}
			}
		}
	}

	//�f�t�H���g�X�e�[�g
	IMPLEMENT_SINGLETON_INSTANCE(PlayerDefaultState)
	void PlayerDefaultState::Enter(const shared_ptr<Player>& Obj)
	{
	}
	void PlayerDefaultState::Execute(const shared_ptr<Player>& Obj)
	{
		auto camera = dynamic_pointer_cast<MyCamera>(Obj->OnGetDrawCamera());
		//�X�^�[�g���o���ł͂Ȃ�������
		if (!camera->GetStartFlag())
		{
			auto pause = Obj->GetStage()->GetSharedGameObject<Pause>(L"Pause");
			auto ptrBehavior = Obj->GetBehavior<PlayerBehavior>();
			if (!pause->GetPause())
			{
				ptrBehavior->Move();
			}
			auto ptrGrav = Obj->GetBehavior<Gravity>();

			if (!Obj->GetFloor())
			{
				ptrGrav->SetGravity(0.0f, -9.8f * 2.0f, 0.0f);
				ptrGrav->Execute();
			}
			else
			{
				ptrGrav->SetGravity(Vec3(0));
				ptrGrav->Execute();
			}
		}
		else
		{
			auto CntVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntVec[0].bConnected)
			{
				if (CntVec[0].wPressedButtons)
				{
					auto pause = Obj->GetStage()->GetSharedGameObject<Pause>(L"Pause");

					auto scene = App::GetApp()->GetScene<Scene>();
					scene->m_Dead = false;

					camera->SetStartFlag(false);
					camera->SetRadXZ(0.0f);
					Obj->GetStage()->RemoveGameObject<GameObject>(Obj->m_StartUI.lock());
					Obj->m_RemovableEff.lock()->SetDrawActive(true);
					pause->SetPause(false);
				}
			}
		}

		if (camera->GetGoalFlag())
		{
			Obj->GetStateMachine()->Push(PlayerGoal::Instance());
		}
	}
	void PlayerDefaultState::Exit(const shared_ptr<Player>& Obj)
	{

	}

	//�w�����Ă���Ƃ��̃X�e�[�g
	IMPLEMENT_SINGLETON_INSTANCE(PlayerBurdenedState)
	void PlayerBurdenedState::Enter(const shared_ptr<Player>& Obj)
	{
		Obj->SetBurdened(true);
	}
	void PlayerBurdenedState::Execute(const shared_ptr<Player>& Obj)
	{
		auto camera = dynamic_pointer_cast<MyCamera>(Obj->OnGetDrawCamera());
		if (!camera->GetStartFlag())
		{
			auto pause = Obj->GetStage()->GetSharedGameObject<Pause>(L"Pause");
			auto ptrBehavior = Obj->GetBehavior<PlayerBehavior>();
			if (!pause->GetPause())
			{
				ptrBehavior->Move();
			}

			auto ptrGrav = Obj->GetBehavior<Gravity>();

			//���ɓ������ĂȂ�������
			if (!Obj->GetFloor())
			{
				ptrGrav->SetGravity(0.0f, -9.8f * 2.0f, 0.0f);
				ptrGrav->Execute();
			}
			else
			{
				ptrGrav->SetGravity(Vec3(0));
				ptrGrav->Execute();
			}

			if (camera->GetGoalFlag())
			{
				Obj->GetStateMachine()->Push(PlayerGoal::Instance());
			}
		}
		else
		{
			Obj->m_RemovableEff.lock()->GetComponent<Transform>()->SetScale(Vec3(20.0f, 2.0f, 20.0f));

			auto CntVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntVec[0].bConnected)
			{
				if (CntVec[0].wPressedButtons)
				{
					auto pause = Obj->GetStage()->GetSharedGameObject<Pause>(L"Pause");

					auto scene = App::GetApp()->GetScene<Scene>();
					scene->m_Dead = false;

					camera->SetStartFlag(false);
					Obj->GetStage()->RemoveGameObject<GameObject>(Obj->m_StartUI.lock());
					Obj->m_RemovableEff.lock()->SetDrawActive(true);
					pause->SetPause(false);
				}
			}
		}
	}
	void PlayerBurdenedState::Sleep(const shared_ptr<Player>& Obj)
	{
		Obj->SetBurdened(true);
	}
	void PlayerBurdenedState::Exit(const shared_ptr<Player>& Obj)
	{
		Obj->SetBurdened(false);
	}

	//�S�[�����̃X�e�[�g
	IMPLEMENT_SINGLETON_INSTANCE(PlayerGoal)
	void PlayerGoal::Enter(const shared_ptr<Player>& Obj)
	{
		auto ef = Obj->GetStage()->GetSharedGameObject<GoalEffect>(L"GoalEffect");
		
		ef->Play
		(
			Vec3(Obj->GetComponent<Transform>()->GetPosition().x,
				Obj->GetComponent<Transform>()->GetPosition().y,
				Obj->GetComponent<Transform>()->GetPosition().z + 5.0f)
		);

		Obj->GetComponent<Rigidbody>()->SetUpdateActive(false);
		Obj->SetTotalTime(0.0f);

	}

	void PlayerGoal::Execute(const shared_ptr<Player>& Obj)
	{

	}
	void PlayerGoal::Exit(const shared_ptr<Player>& Obj)
	{
	}

	//���ʂƂ��̃X�e�[�g
	IMPLEMENT_SINGLETON_INSTANCE(DeathState)
	void DeathState::Enter(const shared_ptr<Player>& Obj)
	{
		auto col = Obj->GetComponent<CollisionSphere>();
		col->SetIsHitAction(IsHitAction::None);
		auto rigid = Obj->GetComponent<Rigidbody>();
		rigid->SetUpdateActive(false);

		auto action = Obj->GetComponent<Action>();
		action->AddRotateBy(2.0f, Vec3(0.0f, 720.0f, 0.0f));
		action->AddMoveBy(2.0f, Vec3(0.0f, -1.0f, 0.0f));
		action->Run();

		m_TotalTime = 0.0f;
	}
	void DeathState::Execute(const shared_ptr<Player>& Obj)
	{
		auto action = Obj->GetComponent<Action>();
		if (action->GetArrived())
		{
			//Obj->GetStage()->AddGameObject<Fade>
			//	(
			//		Vec2(0.0f, 0.0f),
			//		Vec2(1280.0f, 800.0f),
			//		true
			//		);

			auto scene = App::GetApp()->GetScene<Scene>();
			Obj->PostEvent(1.0f, Obj->GetThis<ObjectInterface>(), scene, L"ToGameStage");
			scene->m_Dead = true;
			Obj->GetStage()->RemoveGameObject<Player>(Obj->GetThis<Player>());
			auto group = Obj->GetStage()->GetSharedObjectGroup(L"AllObject");
			group->Remove(Obj);
			m_TotalTime += App::GetApp()->GetElapsedTime();
		}

		if (m_TotalTime == 1.0f)
		{
			//Obj->GetStage()->RemoveGameObject<Player>(Obj->GetThis<Player>());
		}
	}
	void DeathState::Exit(const shared_ptr<Player>& Obj)
	{

	}
	//--------------------------------------------------------------------------------------------------------
}
//end basecross

