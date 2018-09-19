/*!
@file EfkInterface.h
@brief Effekseerエフェクトのインターフェイス
*/

#pragma once

#include "stdafx.h"

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>


#pragma comment(lib, "Effekseer.lib" )
#pragma comment(lib, "EffekseerRendererDX11.lib" )
#pragma comment(lib, "EffekseerSoundXAudio2.lib" )


namespace basecross {

	class EfkInterface;

	//-------------------------------------------------------------------------
	//	class EfkEffect : public ObjectInterface
	//	用途 : Effekseerエフェクトのエフェクト
	//-------------------------------------------------------------------------
	class EfkEffect : public ObjectInterface {
		wstring m_FileName;
		Effekseer::Effect* m_Effect;
		weak_ptr<EfkInterface> m_EfkInterface;
		friend class EfkPlay;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスラトクタ
		@param[in]	iface	インターフェイス
		@param[in]	filename	エフェクトファイル名
		*/
		//--------------------------------------------------------------------------------------
		EfkEffect(const shared_ptr<EfkInterface>& iface, const wstring& filename);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~EfkEffect();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
	};

	//-------------------------------------------------------------------------
	//	class EfkPlay : public ObjectInterface
	//	用途 : EffekseerエフェクトのPlayオブジェクト
	//-------------------------------------------------------------------------
	class EfkPlay : public ObjectInterface {
		::Effekseer::Handle m_Handle;
		weak_ptr<EfkInterface> m_EfkInterface;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスラトクタ
		@param[in]	effect	エフェクト
		@param[in]	Emitter	エミッター
		*/
		//--------------------------------------------------------------------------------------
		EfkPlay(const shared_ptr<EfkEffect>& effect, const bsm::Vec3& Emitter);
		EfkPlay(const shared_ptr<EfkEffect>& effect, const bsm::Vec3& Emitter, const bsm::Vec3& Rotation);
		EfkPlay(const shared_ptr<EfkEffect>& effect, const bsm::Vec3& Emitter, float scale);
		virtual ~EfkPlay();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void OnCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エフェクトをストップさせる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void StopEffect();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エフェクトを移動する
		@param[in]	Location	移動ベクトル（相対方向）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddLocation(const bsm::Vec3& Location);
	};


	//-------------------------------------------------------------------------
	//	class EfkInterface : public ObjectInterface, public ShapeInterface 
	//	用途 : Effekseerエフェクトのインターフェース
	//-------------------------------------------------------------------------
	class EfkInterface : public ObjectInterface, public ShapeInterface {
		::Effekseer::Manager* m_Manager;
		::EffekseerRenderer::Renderer*	m_Renderer;
		friend class EfkPlay;
		friend class EfkEffect;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスラトクタ
		*/
		//--------------------------------------------------------------------------------------
		EfkInterface();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		+		*/
		//--------------------------------------------------------------------------------------
		virtual ~EfkInterface();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ビューと射影行列を設定する
		@param[in]	view	ビュー行列
		@param[in]	proj	射影行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetViewProj(const bsm::Mat4x4& view, const bsm::Mat4x4& proj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//-------------------------------------------------------------------------
	//	class Effect : public GameObject
	//	用途 : まとめたやつ
	//-------------------------------------------------------------------------
	class Effect : public GameObject
	{
	public:
		Effect(const shared_ptr<Stage>& StagePtr, wstring key) : GameObject(StagePtr), m_Key(key) {};
		virtual ~Effect() {};

		//　エフェクト
		shared_ptr<EfkEffect> m_EfkEffect;
		// エフェクト実行オブジェクト
		shared_ptr<EfkPlay> m_EfkPlay;

		wstring m_Key;

		virtual void OnCreate() override;

		void Play(Vec3& pos)
		{
			m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkEffect, pos);
		};

		void Play(Vec3& pos, Vec3& rot)
		{
			m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkEffect, pos, rot);
		};

		void Play(Vec3& pos, float scale)
		{
			m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkEffect, pos, scale);
		}
	};

	//-------------------------------------------------------------------------
	//	class GoalEffect : public GameObject
	//	用途 : ゴール用のやつ
	//-------------------------------------------------------------------------
	class GoalEffect : public GameObject
	{
	public:
		GoalEffect(const shared_ptr<Stage>& StagePtr) : GameObject(StagePtr) {};
		virtual ~GoalEffect() {};

		//　エフェクト
		shared_ptr<EfkEffect> m_EfkEffect;
		// エフェクト実行オブジェクト
		shared_ptr<EfkPlay> m_EfkPlay;

		wstring m_Key;

		virtual void OnCreate() override;
		virtual void OnUpdate() {};

		void Play(Vec3& pos)
		{
			m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkEffect, pos);
		};

		void Play(Vec3& pos, Vec3& rot)
		{
			m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkEffect, pos, rot);
		};

		void Play(Vec3& pos, float scale)
		{
			m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_EfkEffect, pos, scale);
		}
	};
}
//end basecross

