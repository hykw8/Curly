#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------
	
	//-------------------------------------------------------------------------
	//	class HermitCrab : public GameObject
	//	�p�r : �v���C���[�ƓG�̐e�N���X
	//-------------------------------------------------------------------------
	class HermitCrab : public GameObject
	{
		//�v���C���[�̊k�擾�p
		weak_ptr<Shell> m_PlayerShell;
		//�G�̊k�擾�p
		weak_ptr<Shell> m_EnemyShell;
		//�m�b�N�o�b�N��
		Vec3 m_Crash;
	public:
		HermitCrab(const shared_ptr<Stage>& StagePtr);
		virtual ~HermitCrab() {};

		//�m�b�N�o�b�N
		Vec3 Crash(Vec3 pos, shared_ptr<GameObject>& pair);
		
		weak_ptr<Shell> GetMyShell() { return m_PlayerShell; }
		void SetMyShell(shared_ptr<Shell>& shell) { m_PlayerShell = shell; }

		weak_ptr<Shell> GetPairShell() { return m_EnemyShell; }
		void SetPairShell(shared_ptr<Shell>& shell) { m_EnemyShell = shell; }

		Vec3 GetCrash() { return m_Crash; }
		void SetCrash(Vec3 crash) { m_Crash = crash; }
	};
	//--------------------------------------------------------------------------------------------------------
}
