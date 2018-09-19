#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	template<typename T>

	//-------------------------------------------------------------------------
	//	struct InputHandler
	//	�p�r : ���͂̃n���h��
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
				//x��1.0f�ɂȂ�����
				if (CntlVec[0].fThumbLX >= 0.2f || (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))
				{
					num += 1;
				}
				//x��-1.0f�ɂȂ�����
				else if (CntlVec[0].fThumbLX <= -0.2f || (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
				{
					num -= 1;
				}
				//����1.0f�ɂȂ�����
				else if (CntlVec[0].fThumbLY >= 0.4f || (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP))
				{
					num -= 5;
				}
				//����-1.0f�ɂȂ�����
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
	//	�p�r : Player�s���̃N���X
	//-------------------------------------------------------------------------
	class PlayerBehavior : public Behavior
	{
		//������
		float m_Decel;
		//����
		float m_Mass;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���g���[����������x�N�g���𓾂�
		@return	�����x�N�g��
		*/
		//--------------------------------------------------------------------------------------
		Vec3 GetMoveVector() const;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		PlayerBehavior(const shared_ptr<GameObject>& GameObjectPtr):
			Behavior(GameObjectPtr),
			m_Decel(0.01f),	//�����l
			m_Mass(0.1f)	//����
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PlayerBehavior() {};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����l�𓾂�
		@return	�����l
		*/
		//--------------------------------------------------------------------------------------
		float GetDecel() const {
			return m_Decel;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���ʂ𓾂�
		@return	����
		*/
		//--------------------------------------------------------------------------------------
		float GetMass() const {
			return m_Mass;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Player�̓�������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Move();

		int Choose(int value, int buttonNum);
	};
	//--------------------------------------------------------------------------------------------------------
}
