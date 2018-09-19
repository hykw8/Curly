#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	class Fade : public GameObject
	//	�p�r : �t�F�[�h
	//-------------------------------------------------------------------------
	class Fade : public GameObject
	{
		Vec2 m_Pos;
		Vec2 m_Scale;

		float m_TotalTime;
		//ture��������In�Afalse��������Out
		bool m_Flag;

		vector<VertexPositionColor> m_BackupVertices;
	public:
		Fade(const shared_ptr<Stage>& StagePtr, const Vec2& StartPos, const Vec2& StartScale, const bool Flag);
		virtual ~Fade() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void Infade();
		void Outfade();

		bool GetFlag() { return m_Flag; }
		void SetFlag(bool flag) { m_Flag = flag; }
	};

	//-------------------------------------------------------------------------
	//	class NumberSprite : public GameObject
	//	�p�r : �����ϊ��p
	//-------------------------------------------------------------------------

	class NumberSprite : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		//���_�̔z����Ǘ�����z��
		vector<vector<VertexPositionColorTexture>> m_NumVertexVec;
		//���Ԗڂ����擾
		size_t m_Num;

	public:
		NumberSprite(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~NumberSprite() {};

		virtual void OnCreate();
		virtual void OnUpdate();

		//�A�N�Z�T
		void SetNum(size_t num) { m_Num = num; }
	};

	//-------------------------------------------------------------------------
	//	class GoalCount : public GameObject
	//	�p�r : �T���S�̐��̕\��
	//-------------------------------------------------------------------------
	class GoalCount : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Trace;

		int m_Count;
		int m_CollectionCount;

		vector<shared_ptr<NumberSprite>> NumberSpritePtr;

		vector<weak_ptr<GameObject>> m_UI;
	public:
		GoalCount(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~GoalCount() {};

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void Counter();
		void CreateUI();
	};

	//-------------------------------------------------------------------------
	//	class OnlyDisPlay : public GameObject
	//	�p�r : �̂���̕\���p
	//-------------------------------------------------------------------------
	class OnlyDisPlay : public GameObject	{
		
		bool m_Trace;
		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Rot;
		wstring m_TextureKey;
		//�o�b�N�A�b�v���_�f�[�^
		vector<VertexPositionColorTexture> m_BackupVertices;

	public:
		OnlyDisPlay(const shared_ptr<Stage>& StagePtr,const Vec3& Scale, const Vec3& Pos, wstring TexturePtr);
		OnlyDisPlay(const shared_ptr<Stage>& StagePtr, const Vec3& Scale, const Vec3& Pos, const Vec3& Rotation, wstring TexturePtr);

		virtual ~OnlyDisPlay() 
		{}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};

	//-------------------------------------------------------------------------
	//	class PressAnyButton : public GameObject
	//	�p�r : �^�C�g���̃v���X�{�^��
	//-------------------------------------------------------------------------
	class PressAnyButton : public GameObject {

		Vec2 m_Pos;
		Vec2 m_Scale;
		Col4 m_Color;
		float m_TotalTime;
		wstring m_Texture;

		vector<VertexPositionColorTexture> m_BackupVertices;
	public:
		PressAnyButton(const shared_ptr<Stage>& StagePtr, const Vec2& StartPos, const Vec2& StartScale);
		PressAnyButton(const shared_ptr<Stage>& StagePtr, const Vec2& StartPos, const Vec2& StartScale, Col4 color);
		PressAnyButton(const shared_ptr<Stage>& StagePtr, const Vec2& StartPos, const Vec2& StartScale, wstring TexturePtr);
		virtual ~PressAnyButton() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};

	//-------------------------------------------------------------------------
	//	class GoalUI : public GameObject
	//	�p�r : �S�[�����\��UI
	//-------------------------------------------------------------------------
	class GoalUI : public GameObject
	{
		Vec2 m_Pos;
		Vec2 m_Scale;
		wstring m_Texture;
	public:
		GoalUI(const shared_ptr<Stage>& StagePtr, const Vec2& StartPos, const Vec2& StartScale, wstring TexturePtr);
		virtual ~GoalUI() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};

	//-------------------------------------------------------------------------
	//	class GoalAllUI : public GameObject
	//	�p�r : �S�[��UI���܂Ƃ߂���
	//-------------------------------------------------------------------------
	class GoalAllUI : public GameObject
	{
		Vec2 m_Pos;
		Vec2 m_Scale;
		int m_Num;
		bool m_ViewFlag;

		weak_ptr<GoalUI> m_NextUI;
		weak_ptr<GoalUI> m_Select;
	public:
		GoalAllUI(const shared_ptr<Stage>& StagePtr, const Vec2& StartPos, const Vec2& StartScale);
		virtual ~GoalAllUI() {}

		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		//�@�G�t�F�N�g
		shared_ptr<EfkEffect> m_EfkEffect;
		// �G�t�F�N�g���s�I�u�W�F�N�g
		shared_ptr<EfkPlay> m_EfkPlay;

		void CreateResourses();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		bool GetViewFlag() { return m_ViewFlag; }
		void SetViewFlag(bool flag) { m_ViewFlag = flag; }
	};
	//--------------------------------------------------------------------------------------------------------






	//-------------------------------------------------------------------------
	//	class TitleSprite : public GameObject
	//	�p�r : �^�C�g���X�v���C�g
	//-------------------------------------------------------------------------
	class TitleSprite : public GameObject {

		bool m_Trace;
		Vec2 m_TitlePos;
		Vec2 m_TitleScale;
		wstring m_TextureKey;
	public:
		TitleSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace, const Vec2& StartPos, const Vec2& StartScale);
		virtual ~TitleSprite();
		virtual void OnCreate() override;
	};

	//-------------------------------------------------------------------------
	//	class TitleLogo : public GameObject
	//	�p�r : �^�C�g���̃��S�\��
	//-------------------------------------------------------------------------
	class TitleLogo : public GameObject {

		bool m_Trace;
		Vec2 m_TitlePos;
		Vec2 m_TitleScale;
		wstring m_TextureKey;
	public:
		TitleLogo(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace, const Vec2& StartPos, const Vec2& StartScale);
		virtual ~TitleLogo();
		virtual void OnCreate() override;
	};

	//-------------------------------------------------------------------------
	//	class StageSelectSprite : public GameObject
	//	�p�r : �Z���N�g�X�e�[�W��BG
	//-------------------------------------------------------------------------
	class StageSelectSprite : public GameObject {

		bool m_Trace;
		Vec2 m_StageSelectPos;
		Vec2 m_StageSelectScale;
		wstring m_StageSelectKey;
	public:
		StageSelectSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace, const Vec2& StartPos, const Vec2& StartScale);
		virtual ~StageSelectSprite();
		virtual void OnCreate() override;
	};


	//-------------------------------------------------------------------------
	//	class StageImage : public GameObject
	//	�p�r : �X�e�[�W�̔ԍ���\��
	//-------------------------------------------------------------------------
	class StageImage :public GameObject {
		bool m_Trace;
		Vec3 m_Pos;
		Vec2 m_Scale;
		int m_StageNum; // �X�e�[�W�ԍ�
		wstring m_TextureKey;

		//�o�b�N�A�b�v���_�f�[�^
		vector<VertexPositionColor> m_BackupVertices;

		vector<shared_ptr<NumberSprite>> NumberSpritePtr;
	public:
		StageImage(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace, const Vec3& StartPos, const Vec2& StartScale, const int& StartStage);
		virtual ~StageImage() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void StageImageCounter();

	};

	//-------------------------------------------------------------------------
	//	class SelectCursor : public GameObject
	//	�p�r : �Z���N�g�X�e�[�W�̃J�[�\��
	//-------------------------------------------------------------------------
	class SelectCursor : public GameObject {
		bool m_CursorMoveFlg;
		Vec3 m_Pos;
		Vec2 m_Scale;
		float m_CursorMove;
		float m_ToatalTime;
		float m_ScaleTime;
		float m_DefalutScaleTime;

		enum CurPos
		{
			pos0_0, pos1_0, pos2_0, pos3_0, pos4_0,
			pos0_1, pos1_1, pos2_1, pos3_1, pos4_1,
			//notGamePAD
		};
		enum  CurPos pos;

		//�o�b�N�A�b�v���_�f�[�^
		vector<VertexPositionColorTexture> m_BackupVertices;
	public:

		int CursorPos_X = 0;
		int CursorPos_Y = 0;
		int cursorXYPos[5][2] = { CursorPos_X,CursorPos_Y };

		int m_BeforeNum;
		InputHandler<SelectCursor> m_InputHandler;

		SelectCursor(const shared_ptr<Stage>& StagePtr, const bool& StartFlg, const Vec3& StartPos, const Vec2& StartScale, const float StartMove);
		virtual ~SelectCursor() {};

		void StageSelector();
		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		void CreateResourses();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//-------------------------------------------------------------------------
	//	class SkillGauge : public GameObject
	//	�p�r : �X�L���Q�[�W
	//-------------------------------------------------------------------------
	class SkillGauge : public GameObject {

		bool m_Gauge;
		Vec3 m_Pos;
		Vec2 m_Scale;
		float m_TotalTime;
		//�o�b�N�A�b�v���_�f�[�^
		vector<VertexPositionColor> m_BackupVertices;

	public:
		SkillGauge(
			const shared_ptr<Stage>& StagePtr,
			bool StartGauge,
			const Vec3& StartPos,
			const Vec2& StartScale
			);
		virtual ~SkillGauge();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void WaterGunGauge(float point1, float point2);
		void BalloonGauge(float point1);
	};

	//-------------------------------------------------------------------------
	//	class UIWord : public GameObject
	//	�p�r : �\�́A�d���̕����\���p
	//-------------------------------------------------------------------------
	class UIWord : public GameObject
	{
		Vec2 m_pos;
		Vec2 m_Scale;
		wstring m_texture;
	public:
		UIWord(const shared_ptr<Stage>& Stageptr, const Vec2& StartPos, const Vec2& StartScale, const wstring texture);
		virtual ~UIWord() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() {};
	};

	//-------------------------------------------------------------------------
	//	class UIType : public GameObject
	//	�p�r : �\�͕\���p
	//-------------------------------------------------------------------------
	class UIType : public GameObject
	{
		Vec2 m_Pos;
		Vec2 m_Scale;
		weak_ptr<GameObject> m_point;
	public:
		UIType(const shared_ptr<Stage>& StagePtr, const Vec2& Startpos, const Vec2& StartScale, const shared_ptr<GameObject>& shell);
		virtual ~UIType() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//-------------------------------------------------------------------------
	//	class UIWeight : public GameObject
	//	�p�r : �d���\���p
	//-------------------------------------------------------------------------
	class UIWeight : public GameObject
	{
		Vec2 m_Pos;
		Vec2 m_Scale;
		weak_ptr<GameObject> m_point;
	public:
		UIWeight(const shared_ptr<Stage>& StagePtr, const Vec2& Startpos, const Vec2& StartScale, const shared_ptr<GameObject>& shell);
		virtual ~UIWeight() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//-------------------------------------------------------------------------
	//	class hukidasi : public GameObject
	//	�p�r : �����o��
	//-------------------------------------------------------------------------
	class hukidasi : public GameObject
	{
		Vec2 m_Pos;
		Vec2 m_Scale;
		weak_ptr<GameObject> m_point;

		weak_ptr<GameObject> m_ui[6];
	public:
		hukidasi(const shared_ptr<Stage>& StagePtr, const Vec2& Startpos, const Vec2& StartScale);
		virtual ~hukidasi() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//-------------------------------------------------------------------------
	//	class colon : public GameObject
	//	�p�r : �R����
	//-------------------------------------------------------------------------
	class colon : public GameObject
	{
		Vec2 m_Pos;
		Vec2 m_Scale;
		weak_ptr<GameObject> m_point;
	public:
		colon(const shared_ptr<Stage>& StagePtr, const Vec2& Startpos, const Vec2& StartScale);
		virtual ~colon() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//-------------------------------------------------------------------------
	//	class SelectStageDisPlay : public GameObject
	//	�p�r : �X�e�[�W�̃T���l
	//-------------------------------------------------------------------------
	class SelectStageDisplay : public GameObject
	{
		Vec2 m_Pos;
		Vec2 m_Scale;

		vector<VertexPositionColorTexture> m_BackupVertices;
	public:
		SelectStageDisplay(const shared_ptr<Stage>& StagePtr, const Vec2& StartPos, const Vec2& StartScale);
		virtual ~SelectStageDisplay() {};

		int m_StageNum;

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void StageTextureChange();
	};

	//-------------------------------------------------------------------------
	//	class Pause : public GameObject
	//	�p�r : �|�[�Y
	//-------------------------------------------------------------------------
	class Pause : public GameObject
	{
		Vec3 m_Pos;
		Vec2 m_Scale;

		int m_CursorNum;
		bool m_OnPause;

		vector<weak_ptr<GameObject>> m_sprites;
	public:
		Pause(const shared_ptr<Stage> StagePtr, const Vec3& StartPos, const Vec2& StartScale);
		virtual ~Pause() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void PushStart();
		void ViewDraw();
		void Choose();
		void CursorSwich();
		void PushChoose(vector<CONTROLER_STATE> cntVec);
		void StopUpdate();

		bool GetPause() { return m_OnPause; }
		void SetPause(bool flag) { m_OnPause = flag; }
	};

	//-------------------------------------------------------------------------
	//	class P_Sprite : public GameObject
	//	�p�r : �|�[�Y�p�e�N�X�`��
	//-------------------------------------------------------------------------
	class P_Sprite : public GameObject
	{
		Vec3 m_Pos;
		Vec2 m_Scale;
		wstring m_Texture;
	public:
		P_Sprite(const shared_ptr<Stage> StagePtr, const Vec3& StartPos, const Vec2& StartScale, wstring TexturePtr);
		virtual ~P_Sprite() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}
