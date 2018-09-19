#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	class TempShell : public Shell
	//	用途 : ただの殻
	//-------------------------------------------------------------------------
	class TempShell : public Shell
	{
		//ポジション
		Vec3 m_Pos;
		//スケール
		Vec3 m_Scale;

		unique_ptr<LayeredStateMachine<Shell>> m_StateMachine;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr ステージ
		@param[in]　StartPos 初期ポジションの受け取り用　
		@param[in]　StartScale 初期スケールの受け取り用
		*/
		//--------------------------------------------------------------------------------------
		TempShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		~TempShell();

		//重力
		float m_Gravity = 1.0f;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	能力
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 Ability()override;

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

		unique_ptr<LayeredStateMachine<Shell>>& GetStateMachine()
		{
			return m_StateMachine;
		}
	};

	//-------------------------------------------------------------------------
	//	class SpinShell : public Shell
	//	用途 : 回るやつ
	//-------------------------------------------------------------------------
	class SpinShell : public Shell
	{
		Vec3 m_Pos;
		Vec3 m_Scale;

		float m_height;
		float m_TotalTime;
		bool flag;

		unique_ptr<LayeredStateMachine<SpinShell>> m_StateMachine;
	public:
		SpinShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~SpinShell();

		float m_Speed;

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual Vec3 Ability()override;

		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		void CreateResourses();

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

		//アクセサ

		unique_ptr<LayeredStateMachine<SpinShell>>& GetStateMachine()
		{
			return m_StateMachine;
		}

		void SetRot(Vec3 value)
		{
			auto PtrTrans = GetComponent<Transform>();
			PtrTrans->SetRotation(value);
		}
	};
	//--------------------------------------------------------------------------------------------------------




	//-------------------------------------------------------------------------
	//	class Shell_WaterGun : public Shell
	//	用途 : 水鉄砲
	//-------------------------------------------------------------------------
	class Shell_WaterGun : public Shell
	{
		//ポジション
		Vec3 m_Pos;
		//スケール
		Vec3 m_Scale;

		float m_Water;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr ステージ
		@param[in]　StartPos 初期ポジションの受け取り用　
		@param[in]　StartScale 初期スケールの受け取り用
		*/
		//--------------------------------------------------------------------------------------
		Shell_WaterGun(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const float& StartWater);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		~Shell_WaterGun() {};

		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	能力
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 Ability()override;
	};

	//-------------------------------------------------------------------------
	//	class BallonShell : public Shell
	//	用途 : 気球
	//-------------------------------------------------------------------------
	class BallonShell : public Shell
	{
		Vec3 m_Pos;
		Vec3 m_VecScale;
		float m_Scale;

		vector<weak_ptr<GameObject>> m_ballon;

	public:
		BallonShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const float StartScale);
		BallonShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);

		~BallonShell();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual Vec3 Ability()override;

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		void CreateResourses();
		void Limit();
		void NormalShellCreate();

		vector<weak_ptr<GameObject>> GetBallonChild() { return m_ballon; }
	};

	//-------------------------------------------------------------------------
	//	class IronShell : public Shell
	//	用途 : 鉄の殻
	//-------------------------------------------------------------------------
	class IronShell :public Shell
	{
		Vec3 m_Pos;
		Vec3 m_Scale;

	public:
		IronShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);

		~IronShell();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual Vec3 Ability() override;

		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);
	};
}
