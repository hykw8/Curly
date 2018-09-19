#pragma once
#include "stdafx.h"

namespace basecross {
	//-------------------------------------------------------------------------
	//	class GameStage : public Stage
	//	�p�r : �Q�[���X�e�[�W
	//-------------------------------------------------------------------------
	class GameStage : public Stage {
		//�r���[�ƃ��C�g�̍쐬
		void CreateViewLight();
		void CreateGoalItem();

		// �G�t�F�N�g�̃C���^�[�t�F�[�X
		shared_ptr<EfkInterface> m_EfkInterface;

		float m_TotalTime;

	public:
		//�\�z�Ɣj��
		GameStage() :Stage(), m_TotalTime(0) {}
		virtual ~GameStage() {}
		
		//������
		virtual void OnCreate()override;

		//�G�t�F�N�g�̃C���^�t�F�[�X�̒ǉ�
		shared_ptr<EfkInterface> GetEfkInterface() const {
			return m_EfkInterface;
		}
		//�X�V
		virtual void OnUpdate()override;

		virtual void OnDraw()override;

	};
}

//end basecross
