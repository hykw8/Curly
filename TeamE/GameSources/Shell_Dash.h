#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//-------------------------------------------------------------------------
	//	class DashShell : public Shell
	//	�p�r : ��������k
	//-------------------------------------------------------------------------
	class DashShell : public Shell
	{
		//�|�W�V����
		Vec3 m_Pos;
		//�X�P�[��
		Vec3 m_Scale;
		//�X�s�[�h
		float m_speed;
		//����
		float m_Time;
		//����
		Vec3 m_Velo;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr �X�e�[�W
		@param[in]�@StartPos �����|�W�V�����̎󂯎��p�@
		@param[in]�@StartScale �����X�P�[���̎󂯎��p
		*/
		//--------------------------------------------------------------------------------------
		DashShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		
		~DashShell();

		//--------------------------------------------------------------------------------------
		
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

		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		void CreateResourses();

		void SetVelocity(Vec3& velo)
		{
			auto rigit = GetComponent<Rigidbody>();
			rigit->SetVelocity(velo);
		}
	};


}
