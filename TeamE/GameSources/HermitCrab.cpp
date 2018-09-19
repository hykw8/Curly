#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	HermitCrab::HermitCrab(const shared_ptr<Stage>& StagePtr):
		GameObject(StagePtr)
	{}

	Vec3 HermitCrab::Crash(Vec3 pos, shared_ptr<GameObject>& pair)
	{
		Vec3 crash;
		Vec3 myPos = pos;
		Vec3 pairPos = pair->GetComponent<Transform>()->GetPosition();
		Vec3 velo = pairPos - myPos;
		
		//�����̊k������̊k���d��������
		if (GetMyShell().lock()->GetWeight() > GetPairShell().lock()->GetWeight())
		{
			crash = -velo * 3.0f;
		}
		//�����̊k������̊k���y��������
		else if (GetMyShell().lock()->GetWeight() < GetPairShell().lock()->GetWeight())
		{
			crash = -velo * 10.0f;
		}
		//�����d����������
		else
		{
			crash = -velo * 5.0f;
		}
		return crash;
	}
	//--------------------------------------------------------------------------------------------------------
}