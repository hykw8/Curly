#pragma once
#include "stdafx.h"
#include "Project.h"
namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	class UnityCreate
	//	�p�r: Unity��Transform����.csv����Ƃ��ė��Ĕz�u���鏈��
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
