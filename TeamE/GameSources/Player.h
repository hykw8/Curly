/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	class Player : public HermitCrab
	//	�p�r : �v���C���[
	//-------------------------------------------------------------------------
	class  Player : public HermitCrab
	{
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Pos;
		float m_Totaltime;

		//����
		Vec3 m_Direction;
		//�_�b�V���t���O
		bool m_dashFlag;
		//�ő�X�s�[�h
		float m_MaxSpeed;
		//���݂̃X�s�[�h
		float m_Speed;
		//�d��
		Vec3 m_Gravity;
		//���ɏ���Ă��邩�ǂ���
		bool m_IsFloor;
		//�G�ɓ������Ă���t���O
		bool m_EneFlag;
		//�m�b�N�o�b�N��
		Vec3 m_Crash;
		//���Ƀt���O
		bool m_Dead;
		//�k�t���O
		bool m_ShellFlag;	
		//��Ԓl
		float m_Offset;			
		//�X�e�[�g�}�V��
		unique_ptr<LayeredStateMachine<Player>> m_StateMachine;
		//���ݒ�p
		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		//weak_ptr<MultiAudioObject>m_AudioObjectPtr;

	public:
		Player
		(
			const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Pos

		);

		virtual ~Player() {};

		//���̓n���h���[
		InputHandler<Player> m_InputHandler;
		//�k��w�����ꏊ
		weak_ptr<GameObject> m_ShellArea;
		weak_ptr<GameObject> m_StartUI;

		weak_ptr<GameObject> m_RemovableEff;

		//�@�G�t�F�N�g
		//shared_ptr<EfkEffect> m_EfkEffect;
		weak_ptr<EfkEffect> m_EfkEffect;
		// �G�t�F�N�g���s�I�u�W�F�N�g
		//shared_ptr<EfkPlay> m_EfkPlay;
		weak_ptr<EfkPlay> m_EfkPlay;

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);
		void OnCollisionExcute(vector<shared_ptr<GameObject>>& OtherVec);

		//�k���̂Ă�
		void ShellThrow();
		//�k��w����
		void ShellBurdened(shared_ptr<GameObject>& shell);
		//�X�s�[�h�ύX
		void ChangeSpeed();
		//���E���̏���
		void Boundary();
		//�m�b�N�o�b�N����Velocity�ݒ�
		void CrashVelo();
		//�ǂɓ����������̏���
		void HitWall(shared_ptr<GameObject>& obj);

		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �X�e�[�g�}�V���𓾂�
		@return �X�e�[�g�}�V��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr<LayeredStateMachine<Player>>& GetStateMachine()
		{
			return m_StateMachine;
		}

		float GetSpeed() { return m_Speed; }
		void SetSpeed(float value) { m_Speed = value; }

		Vec3 GetDire() { return m_Direction; }
		void SetDire(Vec3 value) 
		{
			if (value != Vec3(0))
			{
				m_Direction = value;
			}
		}

		bool GetDead() { return m_Dead; }
		void SetDead(bool flag) { m_Dead = flag; }

		bool GetDashFlag() { return m_dashFlag; }
		void SetDashFlag(bool flag) { m_dashFlag = flag; }

		float GetTotalTime() { return m_Totaltime; }
		void SetTotalTime(float value) { m_Totaltime = value; }

		void SetPosition(const Vec3& pos)
		{
			auto transComp = GetComponent<Transform>();
			transComp->SetPosition(pos);
		}

		void SetRot(const Vec3& rot)
		{
			m_Rotation = rot;
			auto transComp = GetComponent<Transform>();
			transComp->SetRotation(m_Rotation);
		}

		void SetVelocity(Vec3& velo)
		{
			auto rigit = GetComponent<Rigidbody>();
			rigit->SetVelocity(velo);
		}

		bool GetFloor() { return m_IsFloor; }

		bool GetBurdened() { return m_ShellFlag; }
		void SetBurdened(bool flag) { m_ShellFlag = flag; }
	};

	//�f�t�H���g�X�e�[�g
	class PlayerDefaultState : public ObjState<Player>
	{
		PlayerDefaultState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerDefaultState)
		virtual wstring GetStateName()const { return L"DefaultState"; }
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
	//�w�����Ă���Ƃ��̃X�e�[�g
	class PlayerBurdenedState : public ObjState<Player>
	{
		PlayerBurdenedState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerBurdenedState)
		virtual wstring GetStateName()const { return L"BurdenedState"; }
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Sleep(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
	//�S�[�����̃X�e�[�g
	class PlayerGoal : public ObjState<Player>
	{
		PlayerGoal() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerGoal)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
	//���ʎ��̃X�e�[�g
	class DeathState : public ObjState<Player>
	{
		DeathState() {}

		float m_TotalTime;
	public:
		DECLARE_SINGLETON_INSTANCE(DeathState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
	//--------------------------------------------------------------------------------------------------------
}
//end basecross

