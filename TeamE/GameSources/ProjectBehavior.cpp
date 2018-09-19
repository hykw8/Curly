#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	///	プレイヤーの行動クラス
	//--------------------------------------------------------------------------------------
	Vec3 PlayerBehavior::GetMoveVector() const
	{
		Vec3 angle(0, 0, 0);
		//コントローラー取得
		auto CntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntVec[0].bConnected)
		{
			if (CntVec[0].fThumbLX != 0.0f || CntVec[0].fThumbLY != 0.0f)
			{
				auto ptrTrans = GetGameObject()->GetComponent<Transform>();
				auto ptrCamera = GetGameObject()->OnGetDrawCamera();
				//進行方向の向きを計算
				Vec3 front = ptrTrans->GetWorldMatrix().transInMatrix() - ptrCamera->GetEye();
				front.y = 0;
				front.normalize();//正規化
				ptrCamera->GetThis<MyCamera>()->SetFront(front);
				//角度の算出
				float frontAngle = atan2f(front.z, front.x);
				//コントローラー向き計算
				float moveX = CntVec[0].fThumbLX;
				float moveZ = CntVec[0].fThumbLY;
				//移動量算出
				Vec2 moveVec(moveX, moveZ);
				float moveSize = moveVec.length();
				//コントローラーの向きから角度計算
				float CntAngle = atan2f(-moveX, moveZ);
				//トータルの角度算出
				float totalAngle = frontAngle + CntAngle;
				//角度からベクトル作成
				angle = Vec3(cos(totalAngle), 0.0f, sin(totalAngle));
				//正規化
				angle.normalize();
				//移動量設定
				angle *= moveSize;
				//Y軸変化させない
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
		//コントローラーを離したら
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
		//加速度
		auto accel = force / GetMass();
		//時間を掛けたものを速度に加算
		velo += (accel * time);
		velo.y = 0.0f;
		//速度の設定
		ptrRigid->SetVelocity(velo);
		//回転の計算
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
