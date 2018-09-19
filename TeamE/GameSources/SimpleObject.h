#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	class SangoSpark;


	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	class GoalItem : public GameObject
	//	�p�r : �S�[�����邽�߂̃A�C�e��
	//-------------------------------------------------------------------------
	class GoalItem : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		//�@�G�t�F�N�g
		shared_ptr<EfkEffect> m_EfkEffect;
		// �G�t�F�N�g���s�I�u�W�F�N�g
		shared_ptr<EfkPlay> m_EfkPlay;

		weak_ptr<GameObject> m_Ef;
	public:
		GoalItem(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~GoalItem() {};

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

		void StartMove();

		shared_ptr<MultiAudioObject>m_AudioObjectPtr;
		void CreateResourses();

	};

	//-------------------------------------------------------------------------
	//	class DeadZone : public GameObject
	//	�p�r : ���Ƀ]�[��
	//-------------------------------------------------------------------------
	class DeadZone : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;

	public:
		DeadZone(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~DeadZone() {};

		virtual void OnCreate()override;
		virtual void OnUpdate() {};

		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);
	};
	//--------------------------------------------------------------------------------------------------------




	//-------------------------------------------------------------------------
	//	class Pulley : public GameObject
	//	�p�r : ����
	//-------------------------------------------------------------------------
	class Pulley : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
	public:
		Pulley(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~Pulley() {};
		bool m_Frag;
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);
		void OnCollisionExit(vector<shared_ptr<GameObject>>& OtherVec);
		float Distance(float x, float z, float x2, float z2);
	};

	//-------------------------------------------------------------------------
	//	class SangoSpark : public MultiParticle
	//	�p�r : �T���S���o
	//-------------------------------------------------------------------------
	class SangoSpark : public MultiParticle {
	public :

		SangoSpark(shared_ptr<Stage>& StagePtr);
		virtual ~SangoSpark();

		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
	};

	//-------------------------------------------------------------------------
	//	class Water : public GameObject
	//	�p�r : ����
	//-------------------------------------------------------------------------
	class Water : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Velocity;
		wstring m_Mesh;
		weak_ptr<ObjState<Water>> m_State;

		Vec3 m_Gravity;

		//�X�e�[�g�}�V��
		unique_ptr<StateMachine<Water>> m_StateMachine;
	public:
		Water
		(
			const shared_ptr<Stage>& StagePtr,
			const Vec3& StartPos, 
			const Vec3& StartScale,
			const Vec3& Velocity,
			wstring StrMesh,
			shared_ptr<ObjState<Water>> StateMachine
		);
		virtual ~Water() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

		//���̂̕\���Ə���
		void Active(bool flag);
		//���̃��Z�b�g
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
		//�d��
		void Gravity();

		unique_ptr<StateMachine<Water>>& GetStateMachine()
		{
			return m_StateMachine;
		}
	};

	//--------------------------------------------------------------------------------------
	///	�ʏ�X�e�[�g
	//--------------------------------------------------------------------------------------
	class WaterDefalutState : public ObjState<Water>
	{
		WaterDefalutState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(WaterDefalutState)
		virtual void Enter(const shared_ptr<Water>& Obj)override;
		virtual void Execute(const shared_ptr<Water>& Obj)override;
		virtual void Exit(const shared_ptr<Water>& Obj)override;
	};

	class WaterShotState : public ObjState<Water>
	{
		WaterShotState() {}
	public:
		//�X�e�[�g�̃C���X�^���X�擾
		DECLARE_SINGLETON_INSTANCE(WaterShotState)
		virtual void Enter(const shared_ptr<Water>& Obj)override;
		virtual void Execute(const shared_ptr<Water>& Obj)override;
		virtual void Execute2(const shared_ptr<Water>& Obj)override;
		virtual void Exit(const shared_ptr<Water>& Obj)override;
	};

	//-------------------------------------------------------------------------
	//	class Floor : public GameObject
	//	�p�r : ��
	//-------------------------------------------------------------------------
	class Floor : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Rot;
	public:
		Floor(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& StartRot);
		virtual ~Floor() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);
	};

	//-------------------------------------------------------------------------
	//	class Wall : public GameObject
	//	�p�r : ��
	//-------------------------------------------------------------------------
	class Wall : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		int m_Num;
		vector<vector<VertexPositionColorTexture>> m_NumVertexVec;
	public:
		Wall(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~Wall() {};

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);

	};
	
	//-------------------------------------------------------------------------
	//	class PulleyPoint : public GameObject
	//	�p�r : ���Ԃ̎x�_
	//-------------------------------------------------------------------------
	class PulleyPoint : public GameObject
	{
		weak_ptr<Pulley> m_point;
		weak_ptr<Pulley> m_point2;
	public:
			PulleyPoint(const shared_ptr<Stage>& StagePtr, const shared_ptr<Pulley>PulleyPointptr, const shared_ptr<Pulley>PulleyPointptr2);
			virtual ~PulleyPoint() {};
			virtual void OnCreate()override;
			virtual void OnUpdate()override;
	};

	//-------------------------------------------------------------------------
	//	class MoveGround : public GameObject
	//	�p�r : ������
	//-------------------------------------------------------------------------
	class MoveGround : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;

		float m_Speed;
		Vec3 m_Move;
	public:
		MoveGround(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& Move, const float speed);
		virtual ~MoveGround() {};
		bool m_Frag;
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//-------------------------------------------------------------------------
	//	class SpeechBallon : public GameObject
	//	�p�r : �k�����o��
	//-------------------------------------------------------------------------
	class SpeechBalloon : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		weak_ptr<GameObject> m_Obj;

		bool m_Active;
	public :
		SpeechBalloon(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale,const shared_ptr<GameObject>& Shell);
		virtual ~SpeechBalloon() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		weak_ptr<GameObject> m_UI[7];

		float DistancePtr(float x, float z, float x2, float z2)
		{
			auto SP = ((x2 - x) * (x2 - x) + (z2 - z) * (z2 - z));
			return  SP;
		}

		weak_ptr<GameObject> getObj() { return m_Obj; }
		void SetObj(weak_ptr<GameObject> Obj) { m_Obj = Obj; }
	};
	
	//-------------------------------------------------------------------------
	//	class Word : public GameObject
	//	�p�r : �d���A�\�͕����\���p
	//-------------------------------------------------------------------------
	class Word : public GameObject
	{
		Vec3 m_pos;
		Vec3 m_Scale;
		wstring m_texture;
	public:
		Word(const shared_ptr<Stage>& Stageptr, const Vec3& StartPos, const Vec3& StaetScale, const wstring texture);
		virtual ~Word() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//-------------------------------------------------------------------------
	//	class TypeUI : public GameObject
	//	�p�r : �\�͕\���p
	//-------------------------------------------------------------------------
	class TypeUI : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		weak_ptr<GameObject> m_point;
		bool m_flag;
	public:
		TypeUI(const shared_ptr<Stage>& StagePtr, const Vec3& Startpos, const Vec3& StartScale, const shared_ptr<GameObject>& shell, const bool flag);
		virtual ~TypeUI() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		weak_ptr<GameObject> GetShell() { return m_point; }
		void SetShell(weak_ptr<GameObject>& obj) { m_point = obj; }
	};

	//-------------------------------------------------------------------------
	//	class WeightUI : public GameObject
	//	�p�r : �d���\���p
	//-------------------------------------------------------------------------
	class WeightUI : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		weak_ptr<GameObject> m_point;
		bool m_flag;
	public:
		WeightUI(const shared_ptr<Stage>& StagePtr, const Vec3& Startpos, const Vec3& StartScale, const shared_ptr<GameObject>& shell, const bool flag);
		virtual ~WeightUI() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		weak_ptr<GameObject> GetShell() { return m_point; }
		void SetShell(weak_ptr<GameObject>& obj) { m_point = obj; }
	};

	//-------------------------------------------------------------------------
	//	class Colon : public GameObject
	//	�p�r : �R����
	//-------------------------------------------------------------------------
	class Colon : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		wstring m_Texture;
	public:
		Colon(const shared_ptr<Stage>& Stageptr, const Vec3& StartPos, const Vec3& StaetScale, const wstring Texture);
		virtual ~Colon() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//-------------------------------------------------------------------------
	//	class ClearWall : public GameObject
	//	�p�r : �����ȕ�
	//-------------------------------------------------------------------------
	class ClearWall : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;

	public:
		ClearWall(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		~ClearWall() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//-------------------------------------------------------------------------
	//	class PlayerSpeech : public GameObject
	//	�p�r : Player�����o��
	//-------------------------------------------------------------------------
	class PlayerSpeech : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		weak_ptr<GameObject> m_Obj;

		bool m_Active;
	public:
		PlayerSpeech(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~PlayerSpeech() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		weak_ptr<GameObject> m_UI;

		float DistancePtr(float x, float z, float x2, float z2)
		{
			auto SP = ((x2 - x) * (x2 - x) + (z2 - z) * (z2 - z));
			return  SP;
		}

		weak_ptr<GameObject> getObj() { return m_Obj; }
		void SetObj(weak_ptr<GameObject> Obj) { m_Obj = Obj; }
	};

	//-------------------------------------------------------------------------
	//	class Slope : public GameObject
	//	�p�r : �X���[�v
	//-------------------------------------------------------------------------
	class Slope : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Rot;
	public:
		Slope(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& StartRot);
		virtual ~Slope() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//-------------------------------------------------------------------------
	//	class CheckPoint : public GameObject
	//	�p�r : �`�F�b�N�|�C���g
	//-------------------------------------------------------------------------
	class CheckPoint : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;

	public:
		CheckPoint(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~CheckPoint() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec);
	};

	//-------------------------------------------------------------------------
	//	class StaticObject : public GameObject
	//	�p�r : �����̃I�u�W�F�N�g
	//-------------------------------------------------------------------------
	class StaticObject : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
		Vec3 m_Rot;
	public:
		StaticObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale, const Vec3& StartRot);
		virtual ~StaticObject() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	class BackGround : public GameObject
	{
		Vec3 m_Pos;
		Vec3 m_Scale;
	public:
		BackGround(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& StartScale);
		virtual ~BackGround() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
};