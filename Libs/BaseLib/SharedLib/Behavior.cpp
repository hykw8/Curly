/*!
@file Behavior.cpp
@brief �s���N���X����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct Behavior::Impl;
	//--------------------------------------------------------------------------------------
	struct Behavior::Impl {
		weak_ptr<GameObject> m_GameObject;
		explicit Impl(const shared_ptr<GameObject>& GameObjectPtr) :
			m_GameObject(GameObjectPtr)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	///	�s���N���X�̐e�N���X
	//--------------------------------------------------------------------------------------
	Behavior::Behavior(const shared_ptr<GameObject>& GameObjectPtr) :
		pImpl(new Impl(GameObjectPtr))
	{}
	Behavior::~Behavior() {}
	shared_ptr<GameObject> Behavior::GetGameObject() const {
		auto shptr = pImpl->m_GameObject.lock();
		if (!shptr) {
			throw BaseException(
				L"GameObject�͗L���ł͂���܂���",
				L"if (!shptr)",
				L"Behavior::GetGameObject()"
			);
		}
		else {
			return shptr;
		}
	}
	shared_ptr<Stage> Behavior::GetStage() const {
		return GetGameObject()->GetStage();
	}


	//--------------------------------------------------------------------------------------
	///	�s�����[�e�B���e�B�N���X
	//--------------------------------------------------------------------------------------
	//�i�s�����������悤�ɂ���
	void UtilBehavior::RotToHead(float LerpFact) {
		if (LerpFact <= 0.0f) {
			//��ԌW����0�ȉ��Ȃ牽�����Ȃ�
			return;
		}
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		//����Ői�s�����������悤�ɂȂ�
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 Velocity = PtrTransform->GetVelocity();
		if (Velocity.length() > 0.0f) {
			bsm::Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			bsm::Quat Qt;
			Qt.rotationRollPitchYawFromVector(bsm::Vec3(0, ToAngle, 0));
			Qt.normalize();
			//���݂̉�]���擾
			bsm::Quat NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW����
			//���݂ƖڕW����
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				NowQt = XMQuaternionSlerp(NowQt, Qt, LerpFact);
			}
			PtrTransform->SetQuaternion(NowQt);
		}
	}

	void UtilBehavior::RotToHead(const bsm::Vec3& Velocity, float LerpFact) {
		if (LerpFact <= 0.0f) {
			//��ԌW����0�ȉ��Ȃ牽�����Ȃ�
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//��]�̍X�V
		if (Velocity.length() > 0.0f) {
			bsm::Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			bsm::Quat Qt;
			Qt.rotationRollPitchYawFromVector(bsm::Vec3(0, ToAngle, 0));
			Qt.normalize();
			//���݂̉�]���擾
			bsm::Quat NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW����
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				NowQt = XMQuaternionSlerp(NowQt, Qt, LerpFact);
			}
			PtrTransform->SetQuaternion(NowQt);
		}

	}


	//--------------------------------------------------------------------------------------
	///	�������Ȃ��s���N���X
	//--------------------------------------------------------------------------------------
	float WaitBehavior::Execute(const bsm::Vec3& TargetPos) {
		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>();
		auto Velo = PtrRigid->GetVelocity();
		//����
		Velo *= 0.95f;
		PtrRigid->SetVelocity(Velo);
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
		return bsm::length(Pos - TargetPos);
	}

	float WaitBehavior::Execute(const wstring& TargetKey) {
		auto TargetPtr = GetStage()->GetSharedObject(TargetKey);
		auto TargetPos = TargetPtr->GetComponent<Transform>()->GetWorldPosition();
		return Execute(TargetPos);
	}


	//--------------------------------------------------------------------------------------
	//	struct GravityBehavior::Impl;
	//--------------------------------------------------------------------------------------
	struct Gravity::Impl {
		//���݂̏d�͉����x
		bsm::Vec3 m_Gravity;
	public:
		Impl() :
			m_Gravity(0, -9.8f, 0)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	Gravity�s���N���X
	//--------------------------------------------------------------------------------------
	Gravity::Gravity(const shared_ptr<GameObject>& GameObjectPtr) :
		Behavior(GameObjectPtr),
		pImpl(new Impl())
	{}
	Gravity::~Gravity() {}

	const bsm::Vec3& Gravity::GetGravity() const {
		return pImpl->m_Gravity;
	}
	void Gravity::SetGravity(const bsm::Vec3& gravity) {
		pImpl->m_Gravity = gravity;
	}
	void Gravity::SetGravity(float x, float y, float z) {
		pImpl->m_Gravity = bsm::Vec3(x, y, z);
	}

	void Gravity::StartJump(const bsm::Vec3& StartVec, float EscapeSpan) {
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>();
		auto Velo = PtrRigid->GetGravityVelocity();
		Velo += StartVec;
		PtrRigid->SetGravityVelocity(Velo);
		bsm::Vec3 EscapeVec = StartVec;
		//�W�����v���Đe�I�u�W�F�N�g�{�����[������E�o�ł��Ȃ��Ƃ��Ή�
		EscapeVec *= EscapeSpan;
		auto Pos = PtrTransform->GetWorldPosition();
		Pos += EscapeVec;
		PtrTransform->ResetWorldPosition(Pos);
	}
	void Gravity::StartJump(float x, float y, float z, float EscapeSpan) {
		StartJump(bsm::Vec3(x, y, z), EscapeSpan);
	}

	void Gravity::UpdateFromTime(float CalcTime) {
		auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>();
		auto Velo = PtrRigid->GetGravityVelocity();
		Velo += pImpl->m_Gravity * CalcTime;
		PtrRigid->SetGravityVelocity(Velo);
	}

	void Gravity::Execute() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateFromTime(ElapsedTime);
	}


}
//end basecross
