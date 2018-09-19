/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//-------------------------------------------------------------------------
	//	class ShellSphere : public GameObject
	//	�p�r :�@���˂���e
	//-------------------------------------------------------------------------
	class ShellSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
	public:
		//�\�z�Ɣj��
		ShellSphere(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
			);
		virtual ~ShellSphere();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//��X�V
		virtual void OnUpdate2() override;
		//���̂�����
		void Erase();
		//���̂����Z�b�g����
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
	};
}
//end basecross
