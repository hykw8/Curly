#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	class Removable : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;

		Vec3 m_Dire;
		float m_UpValue;

	public:
		Removable(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~Removable() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void AngleAdjustment();

		

		Vec3 GetDire() { return m_Dire; }
		void SetDire(Vec3 value) { m_Dire = value; }

		float GetUpValue() { return m_UpValue; }
		void SetUpValue(float value) { m_UpValue = value; }
	};
}
