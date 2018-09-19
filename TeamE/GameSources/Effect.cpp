#include "stdafx.h"
#include "Project.h"
#include "Effect.h"

namespace basecross
{
	Removable::Removable(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{
		m_UpValue = -1.0f;
	}

	void Removable::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_Pos);
		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetRotation(Vec3(0));

		//auto ptrDraw = AddComponent<PNTStaticDraw>();
		//ptrDraw->SetMeshResource(L"DEFAULT_SQUARE");
		//ptrDraw->SetTextureResource(L"STAR_TX");
		//ptrDraw->SetDepthStencilState(DepthStencilState::Read);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetTextureResource(L"WAVE_TX");
		ptrDraw->SetDepthStencilState(DepthStencilState::Read);

		//auto ptrRigid = AddComponent<Rigidbody>();
		//ptrRigid->SetGravityVelocity(0.0f, -9.8f / 2.0f, 0.0f);

		SetAlphaActive(true);
	}

	void Removable::OnUpdate()
	{
		AngleAdjustment();

		auto ptrTrans = GetComponent<Transform>();
		//auto ptrRigid = GetComponent<Rigidbody>();
		auto time = App::GetApp()->GetElapsedTime();
		//ptrTrans->SetPosition(ptrTrans->GetPosition().x * (GetDire().x * GetUpValue()), ptrTrans->GetPosition().y - (9.8f / 2.0f) * time, ptrTrans->GetPosition().z * (GetDire().z * GetUpValue()));
		//ptrRigid->SetVelocity(GetDire().x * GetUpValue(),ptrRigid->GetVelocity().y * -GetUpValue(), GetDire().z);
		//ptrRigid->SetVelocity(GetDire().x * GetUpValue(), 1.0f * GetUpValue(), GetDire().z);
		//ptrRigid->SetVelocity(GetDire().x, -4.9f * -GetUpValue(), GetDire().z);
		//ptrRigid->SetGravityVelocity(0.0f, -4.9f * -GetUpValue(), GetDire().z);

		//if (GetUpValue() <= -1.0f)
		//{
		//	SetUpValue(-1.0f);
		//}
		//else
		//{
		//	SetUpValue(GetUpValue() * time);
		//}

		if (ptrTrans->GetScale().x >= 20.0f)
		{
			ptrTrans->SetScale(Vec3(m_Scale));
			SetDrawActive(false);
		}
		else
		{
			ptrTrans->SetScale(Vec3(ptrTrans->GetScale().x + 20.0f * time, 3.0f, ptrTrans->GetScale().z + 20.0f * time));
		}
	}

	void Removable::AngleAdjustment()
	{
		auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		Quat qtXZ;
		qtXZ.rotation(camera->GetRadXZ(), Vec3(0.0f, 1.0f, 0.0f));
		GetComponent<Transform>()->SetQuaternion(qtXZ);
	}
}