#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------

	//重さクラス
	enum class Weight
	{
		m_VeryLight,
		m_Light,
		m_NomalWeight,
		m_Heavy,
		m_VeryHeavy
	};

	//-------------------------------------------------------------------------
	//	class Shell : public GameObject
	//	用途 : 殻の親クラス
	//-------------------------------------------------------------------------
	class Shell : public GameObject
	{
		Vec3 m_Scale;
		//重さ
		Weight m_Weight;
		//時間
		float m_TotalTime;
		// 名前
		wstring m_Name;
		//背負われているか
		bool m_Burdened;
		//押されていないか
		bool m_NoPush;
		//大きさの倍率
		float m_ScaleMagni;
		//今何ステージにいるか
		int m_StageNum;
		//脱いだ時のポジション
		Vec3 m_TakePos;
		//敵についている殻かどうか
		bool m_Enemy;

		weak_ptr<GameObject>m_Speechballoon;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		Shell(const shared_ptr<Stage>& StagePtr) : GameObject(StagePtr) 
		{ 
			m_Burdened = false;
			m_ScaleMagni = 0.5f;
			m_Enemy = false;
			AddTag(L"Shell"); 
			AddNumTag(1);
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Shell() {};

		//--------------------------------------------------------------------------------------
		/*!
		@brief	能力
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 Ability() { return Vec3(0); };

		float DistancePtr(float x, float z, float x2, float z2)
		{
			auto SP = ((x2 - x) * (x2 - x) + (z2 - z) * (z2 - z));
			return  SP;
		}

		bool HitFloor(shared_ptr<GameObject>& floor, Vec3& shellPos)
		{
			bool hit = false;
			float area = floor->GetComponent<Transform>()->GetPosition().y + floor->GetComponent<Transform>()->GetScale().y;
			if (shellPos.y > area)
			{
				hit = true;
			}
			return hit;
		}


		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	重さを得る
		@return	重さ
		*/
		//--------------------------------------------------------------------------------------
		Weight GetWeight() { return m_Weight; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	重さを設定する
		@return	重さ
		*/
		//--------------------------------------------------------------------------------------
		void SetWeight(Weight weight) { m_Weight = weight; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	時間を得る
		@return	時間
		*/
		//--------------------------------------------------------------------------------------
		float GetTotalTime() { return m_TotalTime; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	時間を設定
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTotalTime(float value) { m_TotalTime = value; }

		wstring GetName() { return m_Name; }
		void SetName(wstring Name) { m_Name = Name; }

		bool GetBurdened() { return m_Burdened; }
		void SetBurdened(bool flag) { m_Burdened = flag; }

		bool GetNoPush() { return m_NoPush; }
		void SetNoPush(bool flag) { m_NoPush = flag; }

		weak_ptr<GameObject> GetSpeechballoon() { return m_Speechballoon; }
		void SetSpeechballoon(shared_ptr<GameObject> obj) { m_Speechballoon = obj; }

		float GetScaleMagni() { return m_ScaleMagni; }
		void SetScaleMagni(float value) { m_ScaleMagni = value; }

		Vec3 GetStartScale() { return m_Scale; }
		void SetStartScale(Vec3 value) { m_Scale = value; }

		int GetStageNum() { return m_StageNum; }
		void SetStageNum(int value) { m_StageNum = value; }

		Vec3 GetTakePos() { return m_TakePos; }
		void SetTakePos(Vec3 value) { m_TakePos = value; }

		bool GetEnemy() { return m_Enemy; }
		void SetEnemy(bool flag) { m_Enemy = flag; }
	};

	//-------------------------------------------------------------------------
	//	class InvisibleShell : public GameObject
	//	用途 : 透明な殻
	//-------------------------------------------------------------------------
	class InvisibleShell : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale; 
		Vec3 m_Rot;

	public:
		InvisibleShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& StartRot);
		virtual ~InvisibleShell() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	class DefaultShell : public ObjState<Shell>
	{
		DefaultShell() {}
	public:
		DECLARE_SINGLETON_INSTANCE(DefaultShell)
		virtual void Enter(const shared_ptr<Shell>& Obj)override;
		virtual void Execute(const shared_ptr<Shell>& Obj)override;
		virtual void Exit(const shared_ptr<Shell>& Obj)override;
	};
	//--------------------------------------------------------------------------------------------------------
}