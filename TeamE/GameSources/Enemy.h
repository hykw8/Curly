/*!
@file DebugStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	class Enemy : public HermitCrab
	//	用途 : 敵の親
	//-------------------------------------------------------------------------
	class Enemy : public HermitCrab 
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Rotation;
		float m_TotalTime;

		//Playerに当たったかどうか
		bool m_PlaFlag;
		Vec3 m_Crash;

		weak_ptr<GameObject> m_Shell;
		weak_ptr<GameObject> m_Player;
		
	public:
		Enemy(const shared_ptr<Stage>& StagePtr): 
			HermitCrab(StagePtr) { AddTag(L"Enemy"); }
		virtual ~Enemy() {};

		weak_ptr<Shell> m_obj;

		virtual void OnCreate() {};

		virtual void OnUpdate() {};

		unique_ptr< StateMachine<Enemy>>  m_StateMachine;
		const unique_ptr<StateMachine<Enemy>>& GetStateMachine() {
			return m_StateMachine;
		}

		float GetTotalTime() { return m_TotalTime; }
		void SetTotalTime(float value) { m_TotalTime = value; }

		weak_ptr<GameObject> GetShell() { return m_Shell; }
		void SetShell(weak_ptr<GameObject>& Obj) { m_Shell = Obj; }

		weak_ptr<GameObject> GetPlayer() { return m_Player; }
		void SetPlayer(shared_ptr<GameObject>& Obj) { m_Player = Obj; }

		bool GetPlaFlag() { return m_PlaFlag; }
		void SetPlaFlag(bool flag) { m_PlaFlag = flag; }

		Vec3 GetCrash() { return m_Crash; }
		void SetCrash(Vec3 value) { m_Crash = value; }
	};
	//--------------------------------------------------------------------------------------------------------







	//-------------------------------------------------------------------------
	//	class NewHermitCrab : public Enemy
	//	用途 : 一定の範囲に来たら近づいてくる
	//-------------------------------------------------------------------------
	class NewHermitCrab : public Enemy
	{

		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Rot;

		Vec3 firstPos;

		float m_TotalTime;
		float m_speed;
		float m_SarchDistance;
		float m_Direction;
		float m_Offset;

		weak_ptr<GameObject> m_ShellArea;

		enum hermitState
		{
			GoFormState,
			PosReturnState,
			ChasesState
		};

		enum hermitState Status;

	public:
		NewHermitCrab(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& StartRotaion);


		virtual ~NewHermitCrab();

		virtual void OnCreate() override;

		virtual void OnUpdate() override;

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

		float Length_EToP();

		//プレイヤーへの追尾の時
		virtual void PlayerChase();

		//プレイヤーへの追尾していない時
		virtual void PosReturn();

		//行ったり来たりの行動
        virtual void GoFromBehavior();

		// 行動の制御
		virtual void StatusControl();

		void MyDirection();

		void ShellSet();
	};



	//通常ステート
	class EnemyDefaultState : public ObjState<Enemy>
	{
		EnemyDefaultState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(EnemyDefaultState)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;

	};
	//プレイヤーに当たったステート
	class PlayerHit : public ObjState<Enemy>
	{
		PlayerHit() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerHit)
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};


	//-------------------------------------------------------------------------
	//	class ComeEnemy : public Enemy
	//	用途 : 近づいてくる
	//-------------------------------------------------------------------------
	class ComeEnemy : public Enemy {

		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Rot;
		float m_TotalTime;
		float m_speed;
		float m_Direction;
		float m_Offset;

		weak_ptr<GameObject> m_ShellArea;

	public:
		ComeEnemy(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& StartRotaion);

		virtual ~ComeEnemy();

		virtual void OnCreate() override;

		virtual void OnUpdate() override;

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

		//プレイヤーへの追尾
		virtual void PlayerChase();

		void MyDirection();
		void ShellSet();
	};
}
	