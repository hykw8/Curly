#pragma once
#include "stdafx.h"
#include "Project.h"
namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	class UnityCreate
	//	用途: UnityのTransform情報を.csvからとって来て配置する処理
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
