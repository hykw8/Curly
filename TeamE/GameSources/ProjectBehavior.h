#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------

	template<typename T>

	//-------------------------------------------------------------------------
	//	struct InputHandler
	//	用途 : 入力のハンドラ
	//-------------------------------------------------------------------------
	struct InputHandler
	{
		void Push(const shared_ptr<T>& Obj)
		{
			auto CntVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntVec[0].bConnected)
			{
				if (Obj->GetStateMachine()->GetTopState() == PlayerBurdenedState::Instance())
				{
					if (CntVec[0].wButtons & XINPUT_GAMEPAD_A)
					{
						Vec3 velo = Obj->GetMyShell().lock()->GetThis<Shell>()->Ability();
						Obj->GetComponent<Rigidbody>()->SetVelocity(velo);
						Obj->GetMyShell().lock()->GetThis<Shell>()->SetNoPush(false);
					}

					if (CntVec[0].wReleasedButtons & XINPUT_GAMEPAD_A)
					{
						Obj->SetDashFlag(false);
						weak_ptr<GameObject> obj = Obj->GetMyShell();
						obj.lock()->GetThis<Shell>()->SetNoPush(true);
					}

					if (CntVec[0].wPressedButtons & XINPUT_GAMEPAD_X)
					{
						Obj->ShellThrow();
					}
				}
			}
		}

		Vec2 GetStick()
		{
			auto CntVec = App::GetApp()->GetInputDevice().GetControlerVec();
			return Vec2(CntVec[0].fThumbLX, CntVec[0].fThumbLY);
		}

		int SelectCursor(int nowNumber, int SelectNum)
		{
			auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			int num = nowNumber;

			if (CntlVec[0].bConnected)
			{
				//xが1.0fになったら
				if (CntlVec[0].fThumbLX >= 0.2f || (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))
				{
					num += 1;
				}
				//xが-1.0fになったら
				else if (CntlVec[0].fThumbLX <= -0.2f || (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
				{
					num -= 1;
				}
				//ｙが1.0fになったら
				else if (CntlVec[0].fThumbLY >= 0.4f || (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP))
				{
					num -= 5;
				}
				//ｙが-1.0fになったら
				else if (CntlVec[0].fThumbLY <= -0.4f || (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN))
				{
					num += 5;
				}
			}

			if (num > SelectNum)
			{
				num -= 10;
			}
			if (num < 0)
			{
				num += 10;
			}

			return num;
		}
	};

	//-------------------------------------------------------------------------
	//	class PlayerBehavior : public Behavior
	//	用途 : Player行動のクラス
	//-------------------------------------------------------------------------
	class PlayerBehavior : public Behavior
	{
		//減速率
		float m_Decel;
		//質量
		float m_Mass;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	コントローラから方向ベクトルを得る
		@return	方向ベクトル
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetMoveVector() const;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		PlayerBehavior(const shared_ptr<GameObject>& GameObjectPtr):
			Behavior(GameObjectPtr),
			m_Decel(0.01f),	//減速値
			m_Mass(0.1f)	//質量
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PlayerBehavior() {};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	減速値を得る
		@return	減速値
		*/
		//--------------------------------------------------------------------------------------
		float GetDecel() const {
			return m_Decel;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	質量を得る
		@return	質量
		*/
		//--------------------------------------------------------------------------------------
		float GetMass() const {
			return m_Mass;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Playerの動く処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Move();

		int Choose(int value, int buttonNum);
	};
	//--------------------------------------------------------------------------------------------------------
}
