/*!
@file MyCamera.h
@brief カメラ
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	class MyCamera : public Camera
	//	用途: カメラ
	//--------------------------------------------------------------------------------------
	class MyCamera : public Camera
	{
	public :
		explicit MyCamera();
		virtual ~MyCamera();

		//目標オブジェクトを得る
		shared_ptr<GameObject> GetTargetObjct() const;
		//目標オブジェクトを設定
		void SetTargetObject(const shared_ptr<GameObject>& Obj);
		//ゴールUIを得る
		shared_ptr<GameObject> GetGoalUI() const;
		//ゴールUIの設定
		void SetGoalUI(const shared_ptr<GameObject>& Obj);
		//カメラのスピードを得る
		float GetSpeed();
		//カメラのスピードを設定
		void SetSpeed(float value);
		//カメラの視点を設定
		void SetAt(Vec3 value);
		//ターゲットからAtへの調整ベクトルを得る
		Vec3 GetTargetToAt()const;
		//ターゲットからAtへの調整ベクトルを設定する
		void SetTargetToAt(const Vec3& v);
		//高さを得る
		float GetHeight();
		//高さの設定
		void SetHeight(float value);
		//腕の長さを得る
		float GetArmLength();
		//腕の長さを設定
		void SetArmLength(float length);
		//進行方向を得る
		Vec3 GetFront();
		//進行方向の設定
		void SetFront(Vec3 value);
		
		float GetRadXZ();
		void SetRadXZ(float value);
		//ゴール時のフラグ
		bool GetGoalFlag();
		void SetGoalFlag(bool flag);
		//スタート時のフラグ
		bool GetStartFlag();
		void SetStartFlag(bool flag);
		//時間の取得
		float GetTimer();
		void SetTimer(float time);

		bool GetUpDateActive();
		void SetUpDateActive(bool flag);

		virtual void OnUpdate() override;

		void UpdateArmLength();

		//通常時の動き
		void Move();
		//スタート時の動き
		void StartMove();

	private:
		struct Impl;
		unique_ptr<Impl> pImpl;
	};
	//--------------------------------------------------------------------------------------------------------
}
