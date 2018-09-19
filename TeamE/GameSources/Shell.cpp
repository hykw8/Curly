#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------

	InvisibleShell::InvisibleShell(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale, const Vec3 & StartRot) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Rot(StartRot)
	{}

	void InvisibleShell::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(m_Rot);
	}

	void InvisibleShell::OnUpdate()
	{

	}

	//デフォルトステート
	IMPLEMENT_SINGLETON_INSTANCE(DefaultShell)
	void DefaultShell::Enter(const shared_ptr<Shell>& Obj)
	{
	}
	void DefaultShell::Execute(const shared_ptr<Shell>& Obj)
	{
		auto ptrGrav = Obj->GetBehavior<Gravity>();
		ptrGrav->SetGravity(0.0f, -9.8f * 2.0f, 0.0f);
		ptrGrav->Execute();
	}
	void DefaultShell::Exit(const shared_ptr<Shell>& Obj)
	{

	}
	//--------------------------------------------------------------------------------------------------------
};
