/*!
@file EfkInterface.h
@brief Effekseer�G�t�F�N�g�̃C���^�[�t�F�C�X
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
	//	�p�r : Effekseer�G�t�F�N�g�̃G�t�F�N�g
	//-------------------------------------------------------------------------
	class EfkEffect : public ObjectInterface {
		wstring m_FileName;
		Effekseer::Effect* m_Effect;
		weak_ptr<EfkInterface> m_EfkInterface;
		friend class EfkPlay;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X���g�N�^
		@param[in]	iface	�C���^�[�t�F�C�X
		@param[in]	filename	�G�t�F�N�g�t�@�C����
		*/
		//--------------------------------------------------------------------------------------
		EfkEffect(const shared_ptr<EfkInterface>& iface, const wstring& filename);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~EfkEffect();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
	};

	//-------------------------------------------------------------------------
	//	class EfkPlay : public ObjectInterface
	//	�p�r : Effekseer�G�t�F�N�g��Play�I�u�W�F�N�g
	//-------------------------------------------------------------------------
	class EfkPlay : public ObjectInterface {
		::Effekseer::Handle m_Handle;
		weak_ptr<EfkInterface> m_EfkInterface;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X���g�N�^
		@param[in]	effect	�G�t�F�N�g
		@param[in]	Emitter	�G�~�b�^�[
		*/
		//--------------------------------------------------------------------------------------
		EfkPlay(const shared_ptr<EfkEffect>& effect, const bsm::Vec3& Emitter);
		EfkPlay(const shared_ptr<EfkEffect>& effect, const bsm::Vec3& Emitter, const bsm::Vec3& Rotation);
		EfkPlay(const shared_ptr<EfkEffect>& effect, const bsm::Vec3& Emitter, float scale);
		virtual ~EfkPlay();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�������i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void OnCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�G�t�F�N�g���X�g�b�v������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void StopEffect();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�G�t�F�N�g���ړ�����
		@param[in]	Location	�ړ��x�N�g���i���Ε����j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddLocation(const bsm::Vec3& Location);
	};


	//-------------------------------------------------------------------------
	//	class EfkInterface : public ObjectInterface, public ShapeInterface 
	//	�p�r : Effekseer�G�t�F�N�g�̃C���^�[�t�F�[�X
	//-------------------------------------------------------------------------
	class EfkInterface : public ObjectInterface, public ShapeInterface {
		::Effekseer::Manager* m_Manager;
		::EffekseerRenderer::Renderer*	m_Renderer;
		friend class EfkPlay;
		friend class EfkEffect;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X���g�N�^
		*/
		//--------------------------------------------------------------------------------------
		EfkInterface();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		+		*/
		//--------------------------------------------------------------------------------------
		virtual ~EfkInterface();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�r���[�Ǝˉe�s���ݒ肷��
		@param[in]	view	�r���[�s��
		@param[in]	proj	�ˉe�s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetViewProj(const bsm::Mat4x4& view, const bsm::Mat4x4& proj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�揈���B
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//-------------------------------------------------------------------------
	//	class Effect : public GameObject
	//	�p�r : �܂Ƃ߂����
	//-------------------------------------------------------------------------
	class Effect : public GameObject
	{
	public:
		Effect(const shared_ptr<Stage>& StagePtr, wstring key) : GameObject(StagePtr), m_Key(key) {};
		virtual ~Effect() {};

		//�@�G�t�F�N�g
		shared_ptr<EfkEffect> m_EfkEffect;
		// �G�t�F�N�g���s�I�u�W�F�N�g
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
	//	�p�r : �S�[���p�̂��
	//-------------------------------------------------------------------------
	class GoalEffect : public GameObject
	{
	public:
		GoalEffect(const shared_ptr<Stage>& StagePtr) : GameObject(StagePtr) {};
		virtual ~GoalEffect() {};

		//�@�G�t�F�N�g
		shared_ptr<EfkEffect> m_EfkEffect;
		// �G�t�F�N�g���s�I�u�W�F�N�g
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

