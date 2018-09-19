#pragma once
#include "stdafx.h"
#include "Project.h"
namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	’S“–”ÍˆÍ
	//--------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	class UnityCreate
	//	—p“r: Unity‚ÌTransformî•ñ‚ğ.csv‚©‚ç‚Æ‚Á‚Ä—ˆ‚Ä”z’u‚·‚éˆ—
	//--------------------------------------------------------------------------------------
	class UnityCreate
	{
		weak_ptr<Stage> m_Stage;
		wstring m_FileName;
		vector<wstring> RowStr;

		enum Tf
		{
			Pos, 
			Rot,
			Scale
		};

		Vec3 GetT(const Tf& t);

	public:
		UnityCreate(const shared_ptr<Stage>& StagePtr, const wstring FileName);
		virtual ~UnityCreate() {}

		void OnCreate();
	};
	//--------------------------------------------------------------------------------------------------------
}
