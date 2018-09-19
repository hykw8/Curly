#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------
	
	//-------------------------------------------------------------------------
	//	class HermitCrab : public GameObject
	//	用途 : プレイヤーと敵の親クラス
	//-------------------------------------------------------------------------
	class HermitCrab : public GameObject
	{
		//プレイヤーの殻取得用
		weak_ptr<Shell> m_PlayerShell;
		//敵の殻取得用
		weak_ptr<Shell> m_EnemyShell;
		//ノックバック量
		Vec3 m_Crash;
	public:
		HermitCrab(const shared_ptr<Stage>& StagePtr);
		virtual ~HermitCrab() {};

		//ノックバック
		Vec3 Crash(Vec3 pos, shared_ptr<GameObject>& pair);
		
		weak_ptr<Shell> GetMyShell() { return m_PlayerShell; }
		void SetMyShell(shared_ptr<Shell>& shell) { m_PlayerShell = shell; }

		weak_ptr<Shell> GetPairShell() { return m_EnemyShell; }
		void SetPairShell(shared_ptr<Shell>& shell) { m_EnemyShell = shell; }

		Vec3 GetCrash() { return m_Crash; }
		void SetCrash(Vec3 crash) { m_Crash = crash; }
	};
	//--------------------------------------------------------------------------------------------------------
}
