#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
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
		
		//自分の殻が相手の殻より重かったら
		if (GetMyShell().lock()->GetWeight() > GetPairShell().lock()->GetWeight())
		{
			crash = -velo * 3.0f;
		}
		//自分の殻が相手の殻より軽かったら
		else if (GetMyShell().lock()->GetWeight() < GetPairShell().lock()->GetWeight())
		{
			crash = -velo * 10.0f;
		}
		//同じ重さだったら
		else
		{
			crash = -velo * 5.0f;
		}
		return crash;
	}
	//--------------------------------------------------------------------------------------------------------
}