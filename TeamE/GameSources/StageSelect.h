#pragma once
#include "stdafx.h"
#include "Project.h"

//-------------------------------------------------------------------------
//	class StageSelect : public Stage
//	用途 : ステージセレクト
//-------------------------------------------------------------------------
namespace basecross {

	class StageSelect : public Stage {
		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		//ビューの作成
		void CreateViewLight();
		//プレートの作成
		void CreateBGSprite();
		void CreateStageImage();
		void CreateSelectCursor();

	public:
		//構築と破棄
		StageSelect() :Stage() {}
		virtual ~StageSelect() {}

		//初期化
		virtual void OnCreate()override;

		virtual void OnUpdate()override;
	};



}