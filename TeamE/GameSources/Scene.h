/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//-------------------------------------------------------------------------
	//	class Scene : public SceneBase
	//	用途 : ゲームのシーン
	//-------------------------------------------------------------------------
	class Scene : public SceneBase{
		shared_ptr<SoundItem> m_BGM;
		//ステージナンバー
		int m_StageNum;
		//ステージのサイズ
		float m_StageSize;
		//ステージの高さ
		float m_StageHeight;
		//チェックポイントの管理
		bool m_CheckPoint = false;
		//ポジション保存
		Vec3 m_PlayerPos;

		wstring m_StageName;

		shared_ptr<MultiAudioObject> m_AudioObjectPtr;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief イベント取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;

		//ステージの中心
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
