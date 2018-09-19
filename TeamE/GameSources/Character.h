/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//-------------------------------------------------------------------------
	//	class ShellSphere : public GameObject
	//	用途 :　発射する弾
	//-------------------------------------------------------------------------
	class ShellSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
	public:
		//構築と破棄
		ShellSphere(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
			);
		virtual ~ShellSphere();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//後更新
		virtual void OnUpdate2() override;
		//物体を消す
		void Erase();
		//物体をリセットする
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
	};
}
//end basecross
