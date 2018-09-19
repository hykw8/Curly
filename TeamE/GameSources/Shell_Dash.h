#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//-------------------------------------------------------------------------
	//	class DashShell : public Shell
	//	用途 : 加速する殻
	//-------------------------------------------------------------------------
	class DashShell : public Shell
	{
		//ポジション
		Vec3 m_Pos;
		//スケール
		Vec3 m_Scale;
		//スピード
		float m_speed;
		//時間
		float m_Time;
		//向き
		Vec3 m_Velo;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr ステージ
		@param[in]　StartPos 初期ポジションの受け取り用　
		@param[in]　StartScale 初期スケールの受け取り用
		*/
		//--------------------------------------------------------------------------------------
		DashShell(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		
		~DashShell();

		//--------------------------------------------------------------------------------------
		
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

		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		void CreateResourses();

		void SetVelocity(Vec3& velo)
		{
			auto rigit = GetComponent<Rigidbody>();
			rigit->SetVelocity(velo);
		}
	};


}
