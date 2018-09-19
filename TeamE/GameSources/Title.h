#pragma once
#include "stdafx.h"
#include "Project.h"

//-------------------------------------------------------------------------
//	class TitleStage : public Stage
//	用途 : タイトルステージ
//-------------------------------------------------------------------------
namespace basecross {
	class TitleStage : public Stage {
		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		void CreateResourses();
		//ビューの作成
		void CreateViewLight();
		//プレートの作成
		void CreateTitleSprite();

		void CreateTitleLogo();

	public:
		//構築と破棄
		TitleStage() :Stage() {}
		virtual ~TitleStage() {}
		//初期化
		virtual void OnCreate()override;

		virtual void OnUpdate()override;
	};

	

}
