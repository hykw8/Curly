#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	//�d���N���X
	enum class Weight
	{
		m_VeryLight,
		m_Light,
		m_NomalWeight,
		m_Heavy,
		m_VeryHeavy
	};

	//-------------------------------------------------------------------------
	//	class Shell : public GameObject
	//	�p�r : �k�̐e�N���X
	//-------------------------------------------------------------------------
	class Shell : public GameObject
	{
		Vec3 m_Scale;
		//�d��
		Weight m_Weight;
		//����
		float m_TotalTime;
		// ���O
		wstring m_Name;
		//�w�����Ă��邩
		bool m_Burdened;
		//������Ă��Ȃ���
		bool m_NoPush;
		//�傫���̔{��
		float m_ScaleMagni;
		//�����X�e�[�W�ɂ��邩
		int m_StageNum;
		//�E�������̃|�W�V����
		Vec3 m_TakePos;
		//�G�ɂ��Ă���k���ǂ���
		bool m_Enemy;

		weak_ptr<GameObject>m_Speechballoon;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		Shell(const shared_ptr<Stage>& StagePtr) : GameObject(StagePtr) 
		{ 
			m_Burdened = false;
			m_ScaleMagni = 0.5f;
			m_Enemy = false;
			AddTag(L"Shell"); 
			AddNumTag(1);
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Shell() {};

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�\��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 Ability() { return Vec3(0); };

		float DistancePtr(float x, float z, float x2, float z2)
		{
			auto SP = ((x2 - x) * (x2 - x) + (z2 - z) * (z2 - z));
			return  SP;
		}

		bool HitFloor(shared_ptr<GameObject>& floor, Vec3& shellPos)
		{
			bool hit = false;
			float area = floor->GetComponent<Transform>()->GetPosition().y + floor->GetComponent<Transform>()->GetScale().y;
			if (shellPos.y > area)
			{
				hit = true;
			}
			return hit;
		}


		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�d���𓾂�
		@return	�d��
		*/
		//--------------------------------------------------------------------------------------
		Weight GetWeight() { return m_Weight; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�d����ݒ肷��
		@return	�d��
		*/
		//--------------------------------------------------------------------------------------
		void SetWeight(Weight weight) { m_Weight = weight; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Ԃ𓾂�
		@return	����
		*/
		//--------------------------------------------------------------------------------------
		float GetTotalTime() { return m_TotalTime; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Ԃ�ݒ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTotalTime(float value) { m_TotalTime = value; }

		wstring GetName() { return m_Name; }
		void SetName(wstring Name) { m_Name = Name; }

		bool GetBurdened() { return m_Burdened; }
		void SetBurdened(bool flag) { m_Burdened = flag; }

		bool GetNoPush() { return m_NoPush; }
		void SetNoPush(bool flag) { m_NoPush = flag; }

		weak_ptr<GameObject> GetSpeechballoon() { return m_Speechballoon; }
		void SetSpeechballoon(shared_ptr<GameObject> obj) { m_Speechballoon = obj; }

		float GetScaleMagni() { return m_ScaleMagni; }
		void SetScaleMagni(float value) { m_ScaleMagni = value; }

		Vec3 GetStartScale() { return m_Scale; }
		void SetStartScale(Vec3 value) { m_Scale = value; }

		int GetStageNum() { return m_StageNum; }
		void SetStageNum(int value) { m_StageNum = value; }

		Vec3 GetTakePos() { return m_TakePos; }
		void SetTakePos(Vec3 value) { m_TakePos = value; }

		bool GetEnemy() { return m_Enemy; }
		void SetEnemy(bool flag) { m_Enemy = flag; }
	};

	//-------------------------------------------------------------------------
	//	class InvisibleShell : public GameObject
	//	�p�r : �����Ȋk
	//-------------------------------------------------------------------------
	class InvisibleShell : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale; 
		Vec3 m_Rot;

	public:
		InvisibleShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& StartRot);
		virtual ~InvisibleShell() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	class DefaultShell : public ObjState<Shell>
	{
		DefaultShell() {}
	public:
		DECLARE_SINGLETON_INSTANCE(DefaultShell)
		virtual void Enter(const shared_ptr<Shell>& Obj)override;
		virtual void Execute(const shared_ptr<Shell>& Obj)override;
		virtual void Exit(const shared_ptr<Shell>& Obj)override;
	};
	//--------------------------------------------------------------------------------------------------------
}