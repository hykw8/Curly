#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//-------------------------------------------------------------------------
	//	class LicenseStage : public Stage
	//	用途 : 著作権表示
	//-------------------------------------------------------------------------
	class LicenseStage : public Stage
	{
	public:
		LicenseStage() : Stage() {}
		virtual ~LicenseStage() {}

		virtual void OnCreate() override;
	};
}