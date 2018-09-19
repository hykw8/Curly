#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//-------------------------------------------------------------------------
	//	class LicenseStage : public Stage
	//	ópìr : íòçÏå†ï\é¶
	//-------------------------------------------------------------------------
	class LicenseStage : public Stage
	{
	public:
		LicenseStage() : Stage() {}
		virtual ~LicenseStage() {}

		virtual void OnCreate() override;
	};
}