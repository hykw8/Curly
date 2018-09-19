#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	class BreakWall : public GameObject
	//	用途 : 壊れる壁
	//-------------------------------------------------------------------------
	class BreakWall : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;


		//　エフェクト
		//shared_ptr<EfkEffect> m_EfkEffect;
		weak_ptr<EfkEffect> m_EfkEffect;
		// エフェクト実行オブジェクト
		//shared_ptr<EfkPlay> m_EfkPlay;
		weak_ptr<EfkPlay> m_EfkPlay;

		weak_ptr<GameObject> m_Ef;

	public:
		BreakWall(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~BreakWall() {};

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);
		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
	};
	//--------------------------------------------------------------------------------------------------------





	class Door;

	//-------------------------------------------------------------------------
	//	class WaterWheel : public GameObject
	//	用途 : 水車
	//-------------------------------------------------------------------------
	class WaterWheel : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Dir;
	public:
		WaterWheel(
			const shared_ptr<Stage>& StagePtr,
			const Vec3& StartPos,
			const Vec3& StartScale,
			const Vec3& StartRotation,
			const Vec3& StartDir);
		virtual ~WaterWheel() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);
	};

	//-------------------------------------------------------------------------
	//	class WaterWheellChild : public GameObject
	//	用途 : 水車の足場
	//-------------------------------------------------------------------------
	class WaterWheelChild : public GameObject {
		weak_ptr<WaterWheel> m_Parent;
		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Dir;

	public:
		WaterWheelChild(
			const shared_ptr<Stage>& StagePtr,
			const shared_ptr<WaterWheel> WaterWheelptr,
			const Vec3& StartPos,
			const Vec3& StartScale,
			const Vec3& StartRotation,
			const Vec3& StartDir);

		virtual ~WaterWheelChild() {}
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//衝突時
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
	};

	//-------------------------------------------------------------------------
	//	class Swich : public GameObject
	//	用途 : スイッチ
	//-------------------------------------------------------------------------
	class Switch : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		Col4 m_Color;
	public:
		Switch(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Col4& color);
		virtual ~Switch() {};

		bool m_Flag = false;

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		float Distance(float x, float z, float x2, float z2);

		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		void CreateResourses();

		//色ごとに分ける
		void ColorLimit();
		//Playerが範囲内かどうか
		bool InRange();
		void OnDoor(shared_ptr<Door>& door);
		void OffDoor(shared_ptr<Door>& door);
	};

	//-------------------------------------------------------------------------
	//	class Cannon : public GameObject
	//	用途 : 大砲
	//-------------------------------------------------------------------------
	class Cannon : public GameObject
	{
		//ステートマシーン
		unique_ptr< StateMachine<Cannon> >  m_StateMachine;
		Vec3 m_Pos;
		Vec3 m_Scale;
		float m_TotalTime;

	public:
		Cannon(const shared_ptr<Stage>&StagePtr, const Vec3&StartPos, const Vec3& StartSacle);
		virtual ~Cannon() {};

		// 初期化処理
		virtual void OnCreate()override;

		// 更新処理
		void OnUpdate();

		//ステートマシンを得る
		const unique_ptr<StateMachine<Cannon>>& GetStateMachine() {
			return m_StateMachine;
		}
		//プレイヤーとの距離を得る
		float GetToPlayerLen() const;
		//砲弾発射行動
		void FirCannonBehavior(int div);

		virtual void OnUpdate2()override;
	};

	class CannonAttack : public ObjState<Cannon>
	{
		CannonAttack() {}
	public:
		static shared_ptr<CannonAttack> Instance();
		virtual void Enter(const shared_ptr<Cannon>& Obj)override;
		virtual void Execute(const shared_ptr<Cannon>& Obj)override;
		virtual void Exit(const shared_ptr<Cannon>& Obj)override;
	};

	//-------------------------------------------------------------------------
	//	class CannonBall : public GameObject
	//	用途 : 大砲の弾
	//-------------------------------------------------------------------------
	class CannonBall : public GameObject

	{
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
	public:
		//構築と破棄
		CannonBall(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
		);
		virtual ~CannonBall();
		//初期化r
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//後更新
		virtual void OnUpdate2() override;
		//物体を消す
		void Erase();
		//物体をリセットする
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
	};

	//-------------------------------------------------------------------------
	//	class Door : public GameObject
	//	用途 : ドア
	//-------------------------------------------------------------------------
	class Door : public GameObject
	{
		weak_ptr<Switch> m_Parent;
		Vec3 m_Pos;
		Vec3 m_Pos2;
		Vec3 m_Scale;
		Vec3 m_Rot;
		Vec3 m_move;
		Vec3 m_moveMin;

		Col4 m_Color;

		float m_height;
		float m_TotalTime;
		bool Switchflag;
		bool m_PushFlag;

		unique_ptr<LayeredStateMachine<Door>> m_StateMachine;
	public:
		Door(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& StartRot, const Col4 color);
		virtual ~Door() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;


		unique_ptr<LayeredStateMachine<Door>>& GetStateMachine()
		{
			return m_StateMachine;
		}

		bool GetSwichFlag() { return Switchflag; }
		void SetSwichFlag(bool flag) { Switchflag = flag; }

		bool GetPushFlag() { return m_PushFlag; }
		void SetPushFlag(bool flag) { m_PushFlag = flag; }

		Vec3 GetMove() { return m_move; }
		void SetMove(Vec3 value) { m_move = value; }
	};

	class DoorDefalut : public ObjState<Door>
	{
		DoorDefalut() {}
	public:
		DECLARE_SINGLETON_INSTANCE(DoorDefalut)
		virtual void Enter(const shared_ptr<Door>& Obj)override;
		virtual void Execute(const shared_ptr<Door>& Obj)override;
		virtual void Exit(const shared_ptr<Door>& Obj)override;
	};

	class DoorMove : public ObjState<Door>
	{
		DoorMove() {}
	public:
		DECLARE_SINGLETON_INSTANCE(DoorMove)
		virtual void Enter(const shared_ptr<Door>& Obj)override;
		virtual void Execute(const shared_ptr<Door>& Obj)override;
		virtual void Exit(const shared_ptr<Door>& Obj)override;
		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		void CreateResourses();
	};
}