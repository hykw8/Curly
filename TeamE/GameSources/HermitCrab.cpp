#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	’S“–”ÍˆÍ
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
		
		//©•ª‚ÌŠk‚ª‘Šè‚ÌŠk‚æ‚èd‚©‚Á‚½‚ç
		if (GetMyShell().lock()->GetWeight() > GetPairShell().lock()->GetWeight())
		{
			crash = -velo * 3.0f;
		}
		//©•ª‚ÌŠk‚ª‘Šè‚ÌŠk‚æ‚èŒy‚©‚Á‚½‚ç
		else if (GetMyShell().lock()->GetWeight() < GetPairShell().lock()->GetWeight())
		{
			crash = -velo * 10.0f;
		}
		//“¯‚¶d‚³‚¾‚Á‚½‚ç
		else
		{
			crash = -velo * 5.0f;
		}
		return crash;
	}
	//--------------------------------------------------------------------------------------------------------
}