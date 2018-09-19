#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	class TempShell : public Shell
	//	�p�r : �����̊k
	//-------------------------------------------------------------------------
	class TempShell : public Shell
	{
		//�|�W�V����
		Vec3 m_Pos;
		//�X�P�[��
		Vec3 m_Scale;

		unique_ptr<LayeredStateMachine<Shell>> m_StateMachine;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr �X�e�[�W
		@param[in]�@StartPos �����|�W�V�����̎󂯎��p�@
		@param[in]�@StartScale �����X�P�[���̎󂯎��p
		*/
		//--------------------------------------------------------------------------------------
		TempShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		~TempShell();

		//�d��
		float m_Gravity = 1.0f;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�\��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 Ability()override;

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

		unique_ptr<LayeredStateMachine<Shell>>& GetStateMachine()
		{
			return m_StateMachine;
		}
	};

	//-------------------------------------------------------------------------
	//	class SpinShell : public Shell
	//	�p�r : �����
	//-------------------------------------------------------------------------
	class SpinShell : public Shell
	{
		Vec3 m_Pos;
		Vec3 m_Scale;

		float m_height;
		float m_TotalTime;
		bool flag;

		unique_ptr<LayeredStateMachine<SpinShell>> m_StateMachine;
	public:
		SpinShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~SpinShell();

		float m_Speed;

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual Vec3 Ability()override;

		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		void CreateResourses();

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

		//�A�N�Z�T

		unique_ptr<LayeredStateMachine<SpinShell>>& GetStateMachine()
		{
			return m_StateMachine;
		}

		void SetRot(Vec3 value)
		{
			auto PtrTrans = GetComponent<Transform>();
			PtrTrans->SetRotation(value);
		}
	};
	//--------------------------------------------------------------------------------------------------------




	//-------------------------------------------------------------------------
	//	class Shell_WaterGun : public Shell
	//	�p�r : ���S�C
	//-------------------------------------------------------------------------
	class Shell_WaterGun : public Shell
	{
		//�|�W�V����
		Vec3 m_Pos;
		//�X�P�[��
		Vec3 m_Scale;

		float m_Water;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr �X�e�[�W
		@param[in]�@StartPos �����|�W�V�����̎󂯎��p�@
		@param[in]�@StartScale �����X�P�[���̎󂯎��p
		*/
		//--------------------------------------------------------------------------------------
		Shell_WaterGun(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const float& StartWater);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		~Shell_WaterGun() {};

		//--------------------------------------------------------------------------------------
		/*!
		@brief	������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�\��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 Ability()override;
	};

	//-------------------------------------------------------------------------
	//	class BallonShell : public Shell
	//	�p�r : �C��
	//-------------------------------------------------------------------------
	class BallonShell : public Shell
	{
		Vec3 m_Pos;
		Vec3 m_VecScale;
		float m_Scale;

		vector<weak_ptr<GameObject>> m_ballon;

	public:
		BallonShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const float StartScale);
		BallonShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);

		~BallonShell();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual Vec3 Ability()override;

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		void CreateResourses();
		void Limit();
		void NormalShellCreate();

		vector<weak_ptr<GameObject>> GetBallonChild() { return m_ballon; }
	};

	//-------------------------------------------------------------------------
	//	class IronShell : public Shell
	//	�p�r : �S�̊k
	//-------------------------------------------------------------------------
	class IronShell :public Shell
	{
		Vec3 m_Pos;
		Vec3 m_Scale;

	public:
		IronShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);

		~IronShell();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual Vec3 Ability() override;

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);
	};
}
