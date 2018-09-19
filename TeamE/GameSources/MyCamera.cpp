#include "stdafx.h"
#include "Project.h"
#include "MyCamera.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	struct MyCamera::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct MyCamera::Impl
	{
		//�ڕW�̃I�u�W�F�N�g
		weak_ptr<GameObject> m_TargetObject;
		//�S�[��UI
		weak_ptr<GameObject> m_GoalUI;
		//�ڕW�I�u�W�F�N�g��Position
		Vec3 m_TargetPos;
		//�ڕW���王�_�𒲐�����ʒu�x�N�g��
		Vec3 m_TargetToAt;
		//�J�����̃X�s�[�h
		float m_Speed;
		//�㉺�̃X�s�[�h
		float m_UpDownSpeed;
		//�J�����̉�]
		float m_RadXZ;
		//�㉺
		float m_RadY;
		//�J������Y�����
		float m_CameraUnderRot;
		//�J�����̍���
		float m_Height;
		//�A�[���̒���
		float m_ArmLength;
		//�i�s����
		Vec3 m_Front;
		//�X�^�[�g�p���W�A��
		float m_StartRadXZ;

		float m_MaxArm;
		float m_MinArm;
		bool m_StartFlag;
		bool m_GoalFlag;
		float m_Timer;
		bool m_RotFlag;

		bool m_UpdateActive;


		Impl() :
			m_TargetPos(0,0,0),
			m_TargetToAt(0,0,0),
			m_Speed(2.0f),
			m_RadXZ(0.0f),
			m_CameraUnderRot(0.1f),
			m_ArmLength(7.0f),
			m_MaxArm(20.0f),
			m_MinArm(2.0f),
			m_Height(0.0f),
			m_StartFlag(true),
			m_GoalFlag(false),
			m_Timer(0.0f),
			m_RotFlag(false),
			m_StartRadXZ(0.0f),
			m_RadY(0.0f),
			m_UpDownSpeed(1.0f),
			m_UpdateActive(true)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class MyCamera : public Camera
	//	�p�r: �J����
	//--------------------------------------------------------------------------------------
	MyCamera::MyCamera() :
		Camera(),
		pImpl(new Impl())
	{}

	MyCamera::~MyCamera() {}

	//�ڕW�I�u�W�F�N�g�𓾂�
	shared_ptr<GameObject> MyCamera::GetTargetObjct() const
	{
		if (!pImpl->m_TargetObject.expired())
		{
			return pImpl->m_TargetObject.lock();
		}

		return nullptr;
	}

	//�ڕW�I�u�W�F�N�g��ݒ�
	void MyCamera::SetTargetObject(const shared_ptr<GameObject>& Obj)
	{
		pImpl->m_TargetObject = Obj;
		pImpl->m_TargetPos = Obj->GetComponent<Transform>()->GetPosition();
	}

	shared_ptr<GameObject> MyCamera::GetGoalUI() const
	{
		if (!pImpl->m_GoalUI.expired())
		{
			return pImpl->m_GoalUI.lock();
		}

		return nullptr;
	}

	void MyCamera::SetGoalUI(const shared_ptr<GameObject>& Obj)
	{
		pImpl->m_GoalUI = Obj;
	}

	//�J�����̃X�s�[�h�𓾂�
	float MyCamera::GetSpeed()
	{
		return pImpl->m_Speed;
	}

	//�J�����̃X�s�[�h��ݒ�
	void MyCamera::SetSpeed(float value)
	{
		pImpl->m_Speed = value;
	}

	void MyCamera::SetAt(Vec3 value)
	{
		if (GetStartFlag())
		{
			auto scene = App::GetApp()->GetScene<Scene>();
			Camera::SetAt(value);
			Vec3 armVec = GetEye() - GetAt();
			armVec.normalize();
			armVec *= scene->GetStageSize() / 2;
			Vec3 newVec = GetAt() + armVec;
			SetEye(newVec.x, scene->GetStageHeight(), newVec.z);
		}
		else
		{
			Camera::SetAt(value);
			Vec3 armVec = GetEye() - GetAt();
			armVec.normalize();
			armVec *= pImpl->m_ArmLength;
			Vec3 newVec = GetAt() + armVec;
			SetEye(newVec.x, pImpl->m_Height + pImpl->m_TargetPos.y, newVec.z);
		}
	}

	Vec3 MyCamera::GetTargetToAt() const
	{
		return pImpl->m_TargetToAt;
	}

	void MyCamera::SetTargetToAt(const Vec3 & v)
	{
		pImpl->m_TargetToAt = v;
	}

	float MyCamera::GetHeight()
	{
		return pImpl->m_Height;
	}

	void MyCamera::SetHeight(float value)
	{
		pImpl->m_Height = value;
	}

	float MyCamera::GetArmLength()
	{
		return pImpl->m_ArmLength;
	}

	void MyCamera::SetArmLength(float length)
	{
		pImpl->m_ArmLength = length;
	}

	Vec3 MyCamera::GetFront()
	{
		return pImpl->m_Front;
	}

	void MyCamera::SetFront(Vec3 value)
	{
		pImpl->m_Front = value;
	}

	float MyCamera::GetRadXZ()
	{
		return pImpl->m_RadXZ;
	}

	void MyCamera::SetRadXZ(float value)
	{
		pImpl->m_RadXZ = value;
	}

	bool MyCamera::GetGoalFlag()
	{
		return pImpl->m_GoalFlag;
	}

	void MyCamera::SetGoalFlag(bool flag)
	{
		pImpl->m_GoalFlag = flag;
	}

	bool MyCamera::GetStartFlag()
	{
		return pImpl->m_StartFlag;
	}

	void MyCamera::SetStartFlag(bool flag)
	{
		pImpl->m_StartFlag = flag;
	}

	float MyCamera::GetTimer()
	{
		return pImpl->m_Timer;
	}

	void MyCamera::SetTimer(float time)
	{
		pImpl->m_Timer = time;
	}

	bool MyCamera::GetUpDateActive()
	{
		return pImpl->m_UpdateActive;
	}

	void MyCamera::SetUpDateActive(bool flag)
	{
		pImpl->m_UpdateActive = flag;
	}

	void MyCamera::OnUpdate()
	{
		if (GetUpDateActive())
		{
			if (GetStartFlag())
			{
				StartMove();
			}
			else
			{
				Move();
			}

			Camera::OnUpdate();
		}
	}

	void MyCamera::UpdateArmLength()
	{
		auto vec = GetEye() - GetAt();
		pImpl->m_ArmLength = length(vec);
		if (pImpl->m_ArmLength >= pImpl->m_MaxArm)
		{
			pImpl->m_ArmLength = pImpl->m_MaxArm;
		}

		if (pImpl->m_ArmLength <= pImpl->m_MinArm)
		{
			pImpl->m_ArmLength = pImpl->m_MinArm;
		}
	}

	void MyCamera::Move()
	{
		auto cntVec = App::GetApp()->GetInputDevice().GetControlerVec();

		auto time = App::GetApp()->GetElapsedTime();
		auto newEye = GetEye();
		auto newAt = GetAt();
		//�r�̊p�x
		auto armVec = newEye - newAt;
		armVec.normalize();
		if (!GetGoalFlag())
		{
			if (cntVec[0].bConnected)
			{
				if (cntVec[0].fThumbRX != 0.0f)
				{
					SetRadXZ(GetRadXZ() + cntVec[0].fThumbRX * time *  GetSpeed());
				}

				if (cntVec[0].fThumbRY >= 0.1f)
				{
					pImpl->m_RadY += pImpl->m_UpDownSpeed * time;
				}
				else if (cntVec[0].fThumbRY <= -0.1f)
				{
					pImpl->m_RadY -= pImpl->m_UpDownSpeed * time;
				}
			} 
		}
		else
		{
			GetGoalUI()->GetThis<GoalAllUI>()->SetViewFlag(true);
			pImpl->m_ArmLength = 5.0f;
			SetRadXZ(0.0f);
		}

		if (pImpl->m_RadY > XM_PI * 4 / 9.0f)
		{
			pImpl->m_RadY = XM_PI * 4 / 9.0f;
		}
		else if (pImpl->m_RadY <= 0.1f)
		{
			pImpl->m_RadY = 0.1f;
		}
		armVec.y = sin(pImpl->m_RadY);
		
		if (abs(GetRadXZ() >= XM_2PI) && !GetGoalFlag())
		{
			//�P���������0�ɂ���
			SetRadXZ(0.0f);
		}
		else if (abs(GetRadXZ() >= XM_2PI) && GetGoalFlag())
		{
			pImpl->m_RotFlag = true;
		}

		//�N�I�[�^�j�I���̌v�Z
		Quat qtXZ;						
		qtXZ.rotation(GetRadXZ(), Vec3(0.0f, 1.0f, 0.0f));
		qtXZ.normalize();
		//�s��v�Z��XZ�̒l���Z�o
		Mat4x4 Mat;
		Mat.strTransformation
		(
			Vec3(1.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,-1.0f),
			qtXZ
		);

		Vec3 posXZ = Mat.transInMatrix();
		armVec.x = posXZ.x;
		armVec.z = posXZ.z;
		armVec.normalize();

		auto targetPtr = GetTargetObjct();
		if (targetPtr)
		{
			Vec3 toAt = targetPtr->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			toAt += pImpl->m_TargetToAt;
			newAt = Lerp::CalculateLerp(GetAt(), toAt, 0.0f, 1.0f, 1.0f, Lerp::Linear);
		}

		Vec3 toEye = newAt + armVec * pImpl->m_ArmLength;
		newEye = Lerp::CalculateLerp(GetEye(), toEye, 0.0f, 1.0f, 1.0f, Lerp::Linear);

		pImpl->m_TargetPos = GetTargetObjct()->GetComponent<Transform>()->GetPosition();

		auto front = GetEye() - pImpl->m_TargetPos;
		SetFront(front);

		if (!GetGoalFlag())
		{
			//SetEye(newEye.x, pImpl->m_Height + pImpl->m_TargetPos.y, newEye.z);
			SetEye(newEye);
		}
		else
		{
			SetEye(newEye.x, pImpl->m_TargetPos.y, newEye.z);
		}

		SetAt(newAt);
	}

	void MyCamera::StartMove()
	{
		auto time = App::GetApp()->GetElapsedTime();
		auto newEye = GetEye();
		auto newAt = GetAt();
		auto scene = App::GetApp()->GetScene<Scene>();
		//�r�̊p�x
		auto armVec = newEye - newAt;
		armVec.normalize();

		pImpl->m_StartRadXZ += time / 3;

		if (abs(pImpl->m_StartRadXZ >= XM_2PI))
		{
			//�P���������0�ɂ���
			pImpl->m_StartRadXZ = 0;
		}

		//�N�I�[�^�j�I���̌v�Z
		Quat qtXZ;
		qtXZ.rotation(pImpl->m_StartRadXZ, Vec3(0.0f, 1.0f, 0.0f));
		qtXZ.normalize();
		//�s��v�Z��XZ�̒l���Z�o
		Mat4x4 Mat;
		Mat.strTransformation
		(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, -1.0f),
			qtXZ
		);

		Vec3 posXZ = Mat.transInMatrix();
		armVec.x = posXZ.x;
		armVec.z = posXZ.z;
		armVec.normalize();

		Vec3 toEye = newAt + armVec * scene->GetStageSize() / 2;
		newEye = Lerp::CalculateLerp(GetEye(), toEye, 0.0f, 1.0f, 1.0f, Lerp::Linear);

		SetEye(newEye.x, scene->GetStageHeight(), newEye.z);
		SetAt(scene->m_StageCenter);
	}
	//--------------------------------------------------------------------------------------------------------
}