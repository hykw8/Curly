#pragma once
#include "stdafx.h"

namespace basecross {
	//-------------------------------------------------------------------------
	//	class GameStage : public Stage
	//	用途 : ゲームステージ
	//-------------------------------------------------------------------------
	class GameStage : public Stage {
		//ビューとライトの作成
		void CreateViewLight();
		void CreateGoalItem();

		// エフェクトのインターフェース
		shared_ptr<EfkInterface> m_EfkInterface;

		float m_TotalTime;

	public:
		//構築と破棄
		GameStage() :Stage(), m_TotalTime(0) {}
		virtual ~GameStage() {}
		
		//初期化
		virtual void OnCreate()override;

		//エフェクトのインタフェースの追加
		shared_ptr<EfkInterface> GetEfkInterface() const {
			return m_EfkInterface;
		}
		//更新
		virtual void OnUpdate()override;

		virtual void OnDraw()override;

	};
}

//end basecross
