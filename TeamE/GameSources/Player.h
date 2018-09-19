/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	class Player : public HermitCrab
	//	用途 : プレイヤー
	//-------------------------------------------------------------------------
	class  Player : public HermitCrab
	{
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Pos;
		float m_Totaltime;

		//方向
		Vec3 m_Direction;
		//ダッシュフラグ
		bool m_dashFlag;
		//最大スピード
		float m_MaxSpeed;
		//現在のスピード
		float m_Speed;
		//重力
		Vec3 m_Gravity;
		//床に乗っているかどうか
		bool m_IsFloor;
		//敵に当たっているフラグ
		bool m_EneFlag;
		//ノックバック量
		Vec3 m_Crash;
		//死にフラグ
		bool m_Dead;
		//殻フラグ
		bool m_ShellFlag;	
		//補間値
		float m_Offset;			
		//ステートマシン
		unique_ptr<LayeredStateMachine<Player>> m_StateMachine;
		//音設定用
		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		//weak_ptr<MultiAudioObject>m_AudioObjectPtr;

	public:
		Player
		(
			const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Pos

		);

		virtual ~Player() {};

		//入力ハンドラー
		InputHandler<Player> m_InputHandler;
		//殻を背負う場所
		weak_ptr<GameObject> m_ShellArea;
		weak_ptr<GameObject> m_StartUI;

		weak_ptr<GameObject> m_RemovableEff;

		//　エフェクト
		//shared_ptr<EfkEffect> m_EfkEffect;
		weak_ptr<EfkEffect> m_EfkEffect;
		// エフェクト実行オブジェクト
		//shared_ptr<EfkPlay> m_EfkPlay;
		weak_ptr<EfkPlay> m_EfkPlay;

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);
		void OnCollisionExcute(vector<shared_ptr<GameObject>>& OtherVec);

		//殻を捨てる
		void ShellThrow();
		//殻を背負う
		void ShellBurdened(shared_ptr<GameObject>& shell);
		//スピード変更
		void ChangeSpeed();
		//境界線の処理
		void Boundary();
		//ノックバック時のVelocity設定
		void CrashVelo();
		//壁に当たった時の処理
		void HitWall(shared_ptr<GameObject>& obj);

		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief  ステートマシンを得る
		@return ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr<LayeredStateMachine<Player>>& GetStateMachine()
		{
			return m_StateMachine;
		}

		float GetSpeed() { return m_Speed; }
		void SetSpeed(float value) { m_Speed = value; }

		Vec3 GetDire() { return m_Direction; }
		void SetDire(Vec3 value) 
		{
			if (value != Vec3(0))
			{
				m_Direction = value;
			}
		}

		bool GetDead() { return m_Dead; }
		void SetDead(bool flag) { m_Dead = flag; }

		bool GetDashFlag() { return m_dashFlag; }
		void SetDashFlag(bool flag) { m_dashFlag = flag; }

		float GetTotalTime() { return m_Totaltime; }
		void SetTotalTime(float value) { m_Totaltime = value; }

		void SetPosition(const Vec3& pos)
		{
			auto transComp = GetComponent<Transform>();
			transComp->SetPosition(pos);
		}

		void SetRot(const Vec3& rot)
		{
			m_Rotation = rot;
			auto transComp = GetComponent<Transform>();
			transComp->SetRotation(m_Rotation);
		}

		void SetVelocity(Vec3& velo)
		{
			auto rigit = GetComponent<Rigidbody>();
			rigit->SetVelocity(velo);
		}

		bool GetFloor() { return m_IsFloor; }

		bool GetBurdened() { return m_ShellFlag; }
		void SetBurdened(bool flag) { m_ShellFlag = flag; }
	};

	//デフォルトステート
	class PlayerDefaultState : public ObjState<Player>
	{
		PlayerDefaultState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerDefaultState)
		virtual wstring GetStateName()const { return L"DefaultState"; }
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
	//背負っているときのステート
	class PlayerBurdenedState : public ObjState<Player>
	{
		PlayerBurdenedState() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerBurdenedState)
		virtual wstring GetStateName()const { return L"BurdenedState"; }
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Sleep(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
	//ゴール時のステート
	class PlayerGoal : public ObjState<Player>
	{
		PlayerGoal() {}
	public:
		DECLARE_SINGLETON_INSTANCE(PlayerGoal)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
	//死ぬ時のステート
	class DeathState : public ObjState<Player>
	{
		DeathState() {}

		float m_TotalTime;
	public:
		DECLARE_SINGLETON_INSTANCE(DeathState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};
	//--------------------------------------------------------------------------------------------------------
}
//end basecross

