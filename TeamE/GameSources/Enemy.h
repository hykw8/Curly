/*!
@file DebugStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	class Enemy : public HermitCrab
	//	�p�r : �G�̐e
	//-------------------------------------------------------------------------
	class Enemy : public HermitCrab 
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Rotation;
		float m_TotalTime;

		//Player�ɓ����������ǂ���
		bool m_PlaFlag;
		Vec3 m_Crash;

		weak_ptr<GameObject> m_Shell;
		weak_ptr<GameObject> m_Player;
		
	public:
		Enemy(const shared_ptr<Stage>& StagePtr): 
			HermitCrab(StagePtr) { AddTag(L"Enemy"); }
		virtual ~Enemy() {};

		weak_ptr<Shell> m_obj;

		virtual void OnCreate() {};

		virtual void OnUpdate() {};

		unique_ptr< StateMachine<Enemy>>  m_StateMachine;
		const unique_ptr<StateMachine<Enemy>>& GetStateMachine() {
			return m_StateMachine;
		}

		float GetTotalTime() { return m_TotalTime; }
		void SetTotalTime(float value) { m_TotalTime = value; }

		weak_ptr<GameObject> GetShell() { return m_Shell; }
		void SetShell(weak_ptr<GameObject>& Obj) { m_Shell = Obj; }

		weak_ptr<GameObject> GetPlayer() { return m_Player; }
		void SetPlayer(shared_ptr<GameObject>& Obj) { m_Player = Obj; }

		bool GetPlaFlag() { return m_PlaFlag; }
		void SetPlaFlag(bool flag) { m_PlaFlag = flag; }

		Vec3 GetCrash() { return m_Crash; }
		void SetCrash(Vec3 value) { m_Crash = value; }
	};
	//--------------------------------------------------------------------------------------------------------







	//-------------------------------------------------------------------------
	//	class NewHermitCrab : public Enemy
	//	�p�r : ���͈̔͂ɗ�����߂Â��Ă���
	//-------------------------------------------------------------------------
	class NewHermitCrab : public Enemy
	{

		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Rot;

		Vec3 firstPos;

		float m_TotalTime;
		float m_speed;
		float m_SarchDistance;
		float m_Direction;
		float m_Offset;

		weak_ptr<GameObject> m_ShellArea;

		enum hermitState
		{
			GoFormState,
			PosReturnState,
			ChasesState
		};

		enum hermitState Status;

	public:
		NewHermitCrab(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& StartRotaion);


		virtual ~NewHermitCrab();

		virtual void OnCreate() override;

		virtual void OnUpdate() override;

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

		float Length_EToP();

		//�v���C���[�ւ̒ǔ��̎�
		virtual void PlayerChase();

		//�v���C���[�ւ̒ǔ����Ă��Ȃ���
		virtual void PosReturn();

		//�s�����藈����̍s��
        virtual void GoFromBehavior();

		// �s���̐���
		virtual void StatusControl();

		void MyDirection();

		void ShellSet();
	};



	//�ʏ�X�e�[�g
	class EnemyDefaultState : public ObjState<Enemy>
	{
		EnemyDefaultState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(EnemyDefaultState)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;

	};
	//�v���C���[�ɓ��������X�e�[�g
	class PlayerHit : public ObjState<Enemy>
	{
		PlayerHit() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerHit)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};


	//-------------------------------------------------------------------------
	//	class ComeEnemy : public Enemy
	//	�p�r : �߂Â��Ă���
	//-------------------------------------------------------------------------
	class ComeEnemy : public Enemy {

		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Rot;
		float m_TotalTime;
		float m_speed;
		float m_Direction;
		float m_Offset;

		weak_ptr<GameObject> m_ShellArea;

	public:
		ComeEnemy(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& StartRotaion);

		virtual ~ComeEnemy();

		virtual void OnCreate() override;

		virtual void OnUpdate() override;

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

		//�v���C���[�ւ̒ǔ�
		virtual void PlayerChase();

		void MyDirection();
		void ShellSet();
	};
}
	