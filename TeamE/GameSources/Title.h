#pragma once
#include "stdafx.h"
#include "Project.h"

//-------------------------------------------------------------------------
//	class TitleStage : public Stage
//	�p�r : �^�C�g���X�e�[�W
//-------------------------------------------------------------------------
namespace basecross {
	class TitleStage : public Stage {
		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		void CreateResourses();
		//�r���[�̍쐬
		void CreateViewLight();
		//�v���[�g�̍쐬
		void CreateTitleSprite();

		void CreateTitleLogo();

	public:
		//�\�z�Ɣj��
		TitleStage() :Stage() {}
		virtual ~TitleStage() {}
		//������
		virtual void OnCreate()override;

		virtual void OnUpdate()override;
	};

	

}
