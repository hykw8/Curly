#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	///	�v���C���[�̍s���N���X
	//--------------------------------------------------------------------------------------
	Vec3 PlayerBehavior::GetMoveVector() const
	{
		Vec3 angle(0, 0, 0);
		//�R���g���[���[�擾
		auto CntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntVec[0].bConnected)
		{
			if (CntVec[0].fThumbLX != 0.0f || CntVec[0].fThumbLY != 0.0f)
			{
				auto ptrTrans = GetGameObject()->GetComponent<Transform>();
				auto ptrCamera = GetGameObject()->OnGetDrawCamera();
				//�i�s�����̌������v�Z
				Vec3 front = ptrTrans->GetWorldMatrix().transInMatrix() - ptrCamera->GetEye();
				front.y = 0;
				front.normalize();//���K��
				ptrCamera->GetThis<MyCamera>()->SetFront(front);
				//�p�x�̎Z�o
				float frontAngle = atan2f(front.z, front.x);
				//�R���g���[���[�����v�Z
				float moveX = CntVec[0].fThumbLX;
				float moveZ = CntVec[0].fThumbLY;
				//�ړ��ʎZ�o
				Vec2 moveVec(moveX, moveZ);
				float moveSize = moveVec.length();
				//�R���g���[���[�̌�������p�x�v�Z
				float CntAngle = atan2f(-moveX, moveZ);
				//�g�[�^���̊p�x�Z�o
				float totalAngle = frontAngle + CntAngle;
				//�p�x����x�N�g���쐬
				angle = Vec3(cos(totalAngle), 0.0f, sin(totalAngle));
				//���K��
				angle.normalize();
				//�ړ��ʐݒ�
				angle *= moveSize;
				//Y���ω������Ȃ�
				angle.y = 0.0f;
				auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
				player->SetDire(angle);
			}
		}
		return angle;
	}
		
	void PlayerBehavior::Move()
	{
		float time = App::GetApp()->GetElapsedTime();
		Vec3 angle = GetMoveVector();
		auto ptrRigid = GetGameObject()->GetComponent<Rigidbody>();
		auto velo = ptrRigid->GetVelocity();
		//�R���g���[���[�𗣂�����
		if (angle.length() <= 0.0f && velo.y == 0.0f)
		{
			velo *= GetDecel();
			ptrRigid->SetVelocity(velo);
			return;
		}
		auto ptrTrans = GetGameObject()->GetComponent<Transform>();
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto target = angle * player->GetSpeed();
		auto force = target - velo;
		force.y = 0;
		//�����x
		auto accel = force / GetMass();
		//���Ԃ��|�������̂𑬓x�ɉ��Z
		velo += (accel * time);
		velo.y = 0.0f;
		//���x�̐ݒ�
		ptrRigid->SetVelocity(velo);
		//��]�̌v�Z
		if (angle.length() > 0.0f)
		{
			auto utilPtr = GetGameObject()->GetBehavior<UtilBehavior>();
			utilPtr->RotToHead(angle, 1.0f);
		}
	}

	int PlayerBehavior::Choose(int value, int buttonNum)
	{
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		int num = value;

		if (CntlVec[0].bConnected)
		{
			if (CntlVec[0].fThumbLX == 1.0f)
			{
				num += 1;
			}
			else if (CntlVec[0].fThumbLX == -1.0f) 
			{
				num -= 1;
			}
			else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)
			{
				num -= 1;
			}
			else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
			{
				num += 1;
			}
		}

		if (num > buttonNum)
		{
			num = buttonNum;
		}
		if (num < 0)
		{
			num = 0;
		}

		return num;
	}
	//--------------------------------------------------------------------------------------------------------
}
