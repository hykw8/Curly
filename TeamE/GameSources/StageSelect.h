#pragma once
#include "stdafx.h"
#include "Project.h"

//-------------------------------------------------------------------------
//	class StageSelect : public Stage
//	�p�r : �X�e�[�W�Z���N�g
//-------------------------------------------------------------------------
namespace basecross {

	class StageSelect : public Stage {
		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		//�r���[�̍쐬
		void CreateViewLight();
		//�v���[�g�̍쐬
		void CreateBGSprite();
		void CreateStageImage();
		void CreateSelectCursor();

	public:
		//�\�z�Ɣj��
		StageSelect() :Stage() {}
		virtual ~StageSelect() {}

		//������
		virtual void OnCreate()override;

		virtual void OnUpdate()override;
	};



}