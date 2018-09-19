/*!
@file MyCamera.h
@brief �J����
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	class MyCamera : public Camera
	//	�p�r: �J����
	//--------------------------------------------------------------------------------------
	class MyCamera : public Camera
	{
	public :
		explicit MyCamera();
		virtual ~MyCamera();

		//�ڕW�I�u�W�F�N�g�𓾂�
		shared_ptr<GameObject> GetTargetObjct() const;
		//�ڕW�I�u�W�F�N�g��ݒ�
		void SetTargetObject(const shared_ptr<GameObject>& Obj);
		//�S�[��UI�𓾂�
		shared_ptr<GameObject> GetGoalUI() const;
		//�S�[��UI�̐ݒ�
		void SetGoalUI(const shared_ptr<GameObject>& Obj);
		//�J�����̃X�s�[�h�𓾂�
		float GetSpeed();
		//�J�����̃X�s�[�h��ݒ�
		void SetSpeed(float value);
		//�J�����̎��_��ݒ�
		void SetAt(Vec3 value);
		//�^�[�Q�b�g����At�ւ̒����x�N�g���𓾂�
		Vec3 GetTargetToAt()const;
		//�^�[�Q�b�g����At�ւ̒����x�N�g����ݒ肷��
		void SetTargetToAt(const Vec3& v);
		//�����𓾂�
		float GetHeight();
		//�����̐ݒ�
		void SetHeight(float value);
		//�r�̒����𓾂�
		float GetArmLength();
		//�r�̒�����ݒ�
		void SetArmLength(float length);
		//�i�s�����𓾂�
		Vec3 GetFront();
		//�i�s�����̐ݒ�
		void SetFront(Vec3 value);
		
		float GetRadXZ();
		void SetRadXZ(float value);
		//�S�[�����̃t���O
		bool GetGoalFlag();
		void SetGoalFlag(bool flag);
		//�X�^�[�g���̃t���O
		bool GetStartFlag();
		void SetStartFlag(bool flag);
		//���Ԃ̎擾
		float GetTimer();
		void SetTimer(float time);

		bool GetUpDateActive();
		void SetUpDateActive(bool flag);

		virtual void OnUpdate() override;

		void UpdateArmLength();

		//�ʏ펞�̓���
		void Move();
		//�X�^�[�g���̓���
		void StartMove();

	private:
		struct Impl;
		unique_ptr<Impl> pImpl;
	};
	//--------------------------------------------------------------------------------------------------------
}
