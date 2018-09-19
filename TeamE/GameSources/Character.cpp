/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	///	���˂��鋅��
	//--------------------------------------------------------------------------------------
	ShellSphere::ShellSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity) :
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity),
		m_Scale(0.5f)
	{}
	ShellSphere::~ShellSphere() {}


	//������
	void ShellSphere::OnCreate() {
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellGroup");
		//�O���[�v�ɒǉ�
		Group->IntoGroup(GetThis<GameObject>());

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Emitter);

		//Rigidbody������
		auto PtrRedid = AddComponent<Rigidbody>();
		PtrRedid->SetVelocity(m_Velocity);
		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionSphere>();
		PtrCol->SetIsHitAction(IsHitAction::None);


		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//PtrDraw->SetTextureResource(L"TEXTURE01_TX");

	}

	void ShellSphere::OnUpdate() {
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
	}

	void ShellSphere::OnUpdate2() {
		auto PtrTransform = GetComponent<Transform>();
		if (PtrTransform->GetPosition().y < -0.5f) {
			Erase();
		}
		if (GetComponent<Collision>()->GetHitObjectVec().size() > 0) {
			for (auto& v : GetComponent<Collision>()->GetHitObjectVec()) {
				auto& ptr = dynamic_pointer_cast<Player>(v);
				if (ptr) {
					//GetStage()->RemoveGameObject<Player>(ptr);
				}
			}
		}
	}


	void ShellSphere::Erase() {
		SetUpdateActive(false);
		SetDrawActive(false);
	}

	void ShellSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		SetUpdateActive(true);
		SetDrawActive(true);

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->ResetPosition(Emitter);
		auto PtrRedid = GetComponent<Rigidbody>();
		//�d�͂�0�ɂ���
		PtrRedid->SetGravityVelocityZero();
		PtrRedid->SetVelocity(Velocity);
	}

	//--------------------------------------------------------------------------------------
}
//end basecross
