/*!
@file Scene.h
@brief �V�[��
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//-------------------------------------------------------------------------
	//	class Scene : public SceneBase
	//	�p�r : �Q�[���̃V�[��
	//-------------------------------------------------------------------------
	class Scene : public SceneBase{
		shared_ptr<SoundItem> m_BGM;
		//�X�e�[�W�i���o�[
		int m_StageNum;
		//�X�e�[�W�̃T�C�Y
		float m_StageSize;
		//�X�e�[�W�̍���
		float m_StageHeight;
		//�`�F�b�N�|�C���g�̊Ǘ�
		bool m_CheckPoint = false;
		//�|�W�V�����ۑ�
		Vec3 m_PlayerPos;

		wstring m_StageName;

		shared_ptr<MultiAudioObject> m_AudioObjectPtr;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g�擾
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;

		//�X�e�[�W�̒��S
		Vec3 m_StageCenter;

		bool m_Dead = false;

		void Stop();

		void CreateResouce();

		int GetStageNum() { return m_StageNum;}
		void SetStageNum(int num) { m_StageNum = num; }

		float GetStageSize() { return m_StageSize; }
		void SetStageSize(float value) { m_StageSize = value; }

		float GetStageHeight() { return m_StageHeight; }
		void SetStageHeight(float value) { m_StageHeight = value; }

		bool GetCheckPoint() { return m_CheckPoint; }
		void SetCheckPoint(bool flag) { m_CheckPoint = flag; }

		Vec3 GetPlayerPos() { return m_PlayerPos; }
		void SetPlayerPos(Vec3 value) { m_PlayerPos = value; }

		wstring GetStageName() { return m_StageName; }
		void SetStageName(wstring name) { m_StageName = name; }
	};

}

//end basecross
