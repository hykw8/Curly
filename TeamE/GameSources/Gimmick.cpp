#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	�S���͈�
	//--------------------------------------------------------------------------------------------------------
	
	//------------------------------------------------------------------
	///�����
	//------------------------------------------------------------------
	BreakWall::BreakWall(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)

	{}

	void BreakWall::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(Vec3(0.0f, 0.0f, 0.0f));

		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetIsHitAction(IsHitAction::None);

		auto PtrDraw = AddComponent<BcPNTnTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_PNTnT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"BrakeWall_TX");
		PtrDraw->SetNormalMapTextureResource(L"BrakeWallhousen_TX");
		AddTag(L"Wall");
	}

	void BreakWall::OnUpdate()
	{

	}

	void BreakWall::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{
		for (auto v : OtherVec)
		{
			if (v->FindTag(L"Player"))
			{
				if (v->GetThis<Player>()->GetDashFlag())
				{
					m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
					m_AudioObjectPtr->AddAudioResource(L"Break");
					m_AudioObjectPtr->Start(L"Break", 0, 0.5f);

					auto Ptr = GetComponent<Transform>();
					auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
					auto front = camera->GetFront().normalize() * 5.0f;
					auto playerPos = v->GetComponent<Transform>()->GetPosition();
					auto ef = GetStage()->GetSharedGameObject<Effect>(L"BreakWall");
					ef->Play(Vec3(playerPos.x + front.x, playerPos.y, playerPos.z + front.z), 3.0f);
					GetStage()->RemoveGameObject<BreakWall>(GetThis<BreakWall>());
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------




	//------------------------------------------------------------------
	///���� ���R
	//------------------------------------------------------------------
	WaterWheel::WaterWheel(
		const shared_ptr<Stage>&StagePtr,
		const Vec3 & StartPos,
		const Vec3 & StartScale,
		const Vec3 & StartRotation,
		const Vec3 & StartDir) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Rotation(StartRotation), m_Dir(StartDir)
	{}

	void WaterWheel::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(m_Rotation);

		auto PtrCol = AddComponent<CollisionObb>();

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void WaterWheel::OnUpdate()
	{
		m_Rotation.x += m_Dir.x;
		m_Rotation.y += m_Dir.y;
		m_Rotation.z += m_Dir.z;
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetRotation(m_Rotation);
		//if (m_Rotation.z > 360.0f) {
		//	m_Rotation.z = 0.0f;
		//}
	}

	void WaterWheel::OnCollision(vector<shared_ptr<GameObject>>& OtherVec)
	{

	}

	// ���Ԃ̑���

	WaterWheelChild::WaterWheelChild(
		const shared_ptr<Stage>& StagePtr,
		const shared_ptr<WaterWheel> WaterWheelptr,
		const Vec3 & StartPos,
		const Vec3 & StartScale,
		const Vec3 & StartRotation,
		const Vec3 & StartDir
	) :
		GameObject(StagePtr),
		m_Parent(WaterWheelptr),
		m_Pos(StartPos),
		m_Scale(StartScale),
		m_Rotation(StartRotation),
		m_Dir(StartDir)
	{
	}

	//������
	void WaterWheelChild::OnCreate() {

		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = AddComponent<Transform>();

		auto PtrCol = AddComponent<CollisionObb>();

		auto WaterWheelptr = m_Parent.lock();
		if (!WaterWheelptr) {
			throw BaseException(
				L"���Ԃ�������܂���",
				L"if (!WaterWheelptr)",
				L"WaterWheelChild::OnCreate()"
			);
		}

		auto Pos = WaterWheelptr->GetComponent<Transform>()->GetPosition();
		Pos += m_Pos;
		Ptr->SetPosition(Pos);

		auto Scale = WaterWheelptr->GetComponent<Transform>()->GetScale();
		Scale += m_Scale;
		Ptr->SetScale(Scale);

		//�e�q�֌W�ɂ���
		GetComponent<Transform>()->SetParent(WaterWheelptr);

		//�Փ˔��������
		auto WaterWheelCol = AddComponent<CollisionObb>();
		//�v���C���[�Ƃ͏Փ˂��Ȃ�
		WaterWheelCol->AddExcludeCollisionTag(L"WaterWheel");
		WaterWheelCol->SetDrawActive(true);
		//�q�b�g���̃A�N�V�����͎����ŋL�q
		//WaterWheelCol->SetIsHitAction(IsHitAction::None);
		WaterWheelCol->SetFixed(true);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//�`�悷��e�N�X�`����ݒ�
		//PtrDraw->SetTextureResource(L"GREEN_TX");
	}

	void WaterWheelChild::OnUpdate() {
		m_Rotation.x += m_Dir.x;
		m_Rotation.y += m_Dir.y;
		m_Rotation.z += m_Dir.z;
		auto parent_PtrTrans = GetComponent<Transform>();
		parent_PtrTrans->SetRotation(m_Rotation);
	}

	void WaterWheelChild::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
		//�v���C���[�`���C���h�������ɓ�������
		//�v���C���[�Ƃ͏Փ˂��Ȃ��̂ŁA�v���C���[�ȊO�̉���

	}

	//------------------------------------------------------------------
	///�X�C�b�`
	//------------------------------------------------------------------
	Switch::Switch(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale, const Col4& color) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Color(color)
	{}

	void Switch::CreateResourses()
	{
		wstring datadir;
		auto& app = App::GetApp();
		//�T�E���h
		app->GetDataDirectory(datadir);
		datadir += L"SE\\";
		auto SE = datadir + L"Switch.wav";
		app->RegisterWav(L"Switch", SE);
	}

	float Switch::Distance(float x, float z, float x2, float z2)
	{
		auto SP = ((x2 - x) * (x2 - x) + (z2 - z) * (z2 - z));
		return  SP;
	}

	void Switch::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale.x, 0.5f, m_Scale.z);
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);

		//auto PtrCol = AddComponent<CollisionObb>();

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CYLINDER");

		//���ߏ���
		SetAlphaActive(true);
	}

	void Switch::OnUpdate()
	{
		ColorLimit();
	}

	void Switch::ColorLimit()
	{
		//��
		if (m_Color == Col4(1.0f, 0.0f, 0.0f, 1.0f))
		{
			auto PtrDraw = GetComponent<PNTStaticDraw>();
			PtrDraw->SetTextureResource(L"BOTTUNRED_TX");

			auto groupRed = GetStage()->GetSharedObjectGroup(L"DoorRed");
			for (auto& v : groupRed->GetGroupVector())
			{
				auto shPtr = v.lock();
				if (shPtr)
				{
					auto door = dynamic_pointer_cast<Door>(shPtr);
					if (door)
					{
						if (InRange())
						{
							OnDoor(door);
						}
						else
						{
							OffDoor(door);
						}
					}
				}
			}
		}
		//��
		else if (m_Color == Col4(0.0f, 0.0f, 1.0f, 1.0f))
		{
			auto PtrDraw = GetComponent<PNTStaticDraw>();
			PtrDraw->SetTextureResource(L"BOTTUN_BLUE_TX");

			auto groupRed = GetStage()->GetSharedObjectGroup(L"DoorBule");
			for (auto& v : groupRed->GetGroupVector())
			{
				auto shPtr = v.lock();
				if (shPtr)
				{
					auto door = dynamic_pointer_cast<Door>(shPtr);
					if (door)
					{
						if (InRange())
						{
							OnDoor(door);
						}
						else
						{
							OffDoor(door);
						}
					}
				}
			}
		}
		//���F
		else if (m_Color == Col4(1.0f, 1.0f, 0.0f, 1.0f))
		{
			auto PtrDraw = GetComponent<PNTStaticDraw>();
			PtrDraw->SetTextureResource(L"BOTTUN_YELLOW_TX");

			auto groupRed = GetStage()->GetSharedObjectGroup(L"DoorYellow");
			for (auto& v : groupRed->GetGroupVector())
			{
				auto shPtr = v.lock();
				if (shPtr)
				{
					auto door = dynamic_pointer_cast<Door>(shPtr);
					if (door)
					{
						if (InRange())
						{
							OnDoor(door);
						}
						else
						{
							OffDoor(door);
						}
					}
				}
			}
		}
		else
		{
			return;
		}
	}

	bool  Switch::InRange()
	{
		bool flag = false;
		//������
		auto TransPos = GetComponent<Transform>()->GetPosition();
		auto TransSle = GetComponent<Transform>()->GetScale();
		//Player��
		auto Shad = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto transPos = Shad->GetComponent<Transform>()->GetPosition();

		auto OBJ = Shad->GetMyShell();
		//�����v�Z
		auto DisPos = Distance(TransPos.x, TransPos.z, transPos.x, transPos.z);
		float discol = Distance(TransPos.x, TransPos.y, transPos.x, transPos.y);

		//�k��w�����Ă��邩
		if (Shad->GetStateMachine()->GetTopState() == PlayerBurdenedState::Instance())
		{
			auto Weight = OBJ.lock()->GetThis<Shell>()->GetWeight();
			if (DisPos <= TransSle.x && discol <= TransSle.y && (Weight >= Weight::m_Heavy))
			{
				flag = true;
			}
			else if (DisPos >= TransSle.x && discol >= TransSle.y)
			{
				flag = false;
			}

			if (m_Flag == true)
			{
				auto PtrDraw = GetComponent<PNTStaticDraw>();
				PtrDraw->SetDiffuse(Col4(0.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				auto PtrDraw = GetComponent<PNTStaticDraw>();
				PtrDraw->SetDiffuse(Col4(m_Color));
			}
		}
		return flag;
	}

	void Switch::OnDoor(shared_ptr<Door>& door)
	{
		if (!door->GetSwichFlag())
		{
			door->SetPushFlag(true);
			door->GetStateMachine()->Push(DoorMove::Instance());

			m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
			m_AudioObjectPtr->AddAudioResource(L"Switch");
			m_AudioObjectPtr->Start(L"Switch", 0, 1.0f);
		}
	}

	void Switch::OffDoor(shared_ptr<Door>& door)
	{
		door->SetPushFlag(false);
	}

	//------------------------------------------------------------------
	///��C
	//------------------------------------------------------------------
	Cannon::Cannon(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{
	}


	//// y�{�^���Œe������
	//void Cannon::OnPushY() {
	//	auto Ptr = GetComponent<Transform>();
	//	Vec3 Pos = Ptr->GetPosition();
	//	Pos.y += 0.4f;
	//	Quat Qt = Ptr->GetQuaternion();
	//	Vec3 Rot = Qt.toRotVec();
	//	float RotY = Rot.y;
	//	Vec3 velo(sin(RotY), 0.27f, cos(RotY));
	//	velo.normalize();
	//	velo *= 15.0f;
	//	auto Group = GetStage()->GetSharedObjectGroup(L"ShellGroup");
	//	for (size_t i = 0; i < Group->size(); i++) {
	//		auto shptr = dynamic_pointer_cast<ShellSphere>(Group->at(i));
	//		if (shptr && !shptr->IsUpdateActive()) {
	//			//�󂫂���������
	//			shptr->Reset(Pos, velo);
	//			return;
	//		}
	//	}
	//	//�����܂ŗ��Ă�΋󂫂��Ȃ�
	//	GetStage()->AddGameObject<ShellSphere>(Pos, velo);

	//}

	void Cannon::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(Vec3(0.0f, 0.0f, 0.0f));
		//�R���W������t����
		auto PtrCol = AddComponent<CollisionObb>();


		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");

		//�X�e�[�g�}�V��
		m_StateMachine.reset(new StateMachine<Cannon>(GetThis<Cannon>()));
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		//m_StateMachine->ChangeState(BoxDefaultState::Instance());

	}

	void Cannon::OnUpdate()
	{
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}
	//�v���C���[�Ƃ̋����𓾂�
	float Cannon::GetToPlayerLen() const {
		auto Ptr = GetComponent<Transform>();
		Vec3 Pos = Ptr->GetPosition();
		auto PlayerPos
			= GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
		return length(Pos - PlayerPos);
	}

	//�C�e���ˍs��
	void Cannon::FirCannonBehavior(int div) {
		if (Util::DivProbability(div)) {
			//200����1�̊m���Ŕ���
			auto Ptr = GetComponent<Transform>();
			Vec3 Pos = Ptr->GetPosition();
			Pos.y += 0.5f;
			auto PlayerPos
				= GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
			auto velo = PlayerPos;
			velo -= Pos;
			velo.y = 1.0f;
			velo.normalize();
			velo *= 20.0f;
			auto Group = GetStage()->GetSharedObjectGroup(L"ShellGroup");
			for (size_t i = 0; i < Group->size(); i++) {
				auto shptr = dynamic_pointer_cast<CannonBall>(Group->at(i));
				if (shptr && !shptr->IsUpdateActive()) {
					//�󂫂���������n2f2f
					shptr->Reset(Pos, velo);
					return;
				}
			}
			//�����܂ŗ��Ă�΋󂫂��Ȃ�
			GetStage()->AddGameObject<CannonBall>(Pos, velo);
		}
	}




	//float Cannon::GetToPlayerLen() const
	//{
	//	return 0.0f;
	//}

	//


	shared_ptr<CannonAttack> CannonAttack::Instance() {
		static shared_ptr<CannonAttack> instance(new CannonAttack);
		return instance;
	}
	void CannonAttack::Enter(const shared_ptr<Cannon>& Obj) {
	}
	void CannonAttack::Execute(const shared_ptr<Cannon>& Obj) {
		//Obj->SeekPlayerBehavior();
		Obj->FirCannonBehavior(120);
		float len = Obj->GetToPlayerLen();
		//if (len >= 10.0f) {
		//		Obj->GetStateMachine()->ChangeState(BoxDefaultState::Instance());
		//	}
		//	else if (len < 5.0f) {
		//		Obj->GetStateMachine()->ChangeState(BoxSeekAttackState::Instance());
		//	}
	}
	void CannonAttack::Exit(const shared_ptr<Cannon>& Obj) {
	}




	//auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
	//if (CntlVec[0].bConnected) {
	//	//Y�{�^��
	//	if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_Y) {
	//		OnPushY();
	//		}
	//	}
	//}

	void Cannon::OnUpdate2() {
		/*
		if (GetComponent<Collision>()->GetHitObjectVec().size() > 0) {
		for (auto& v : GetComponent<Collision>()->GetHitObjectVec()) {
		auto& ptr = dynamic_pointer_cast<Box>(v);
		if (ptr) {
		GetStage()->RemoveGameObject<Box>(ptr);
		}
		}
		}
		*/
		//������̕\��
		//DrawStrings();
	}

	//void Cannon::DrawStrings()
	//{
	//}



	//������̕\��
	//void Cannon::DrawStrings() {


	//	auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
	//	wstring FPS(L"FPS: ");
	//	FPS += Util::UintToWStr(fps);
	//	FPS += L"\nElapsedTime: ";
	//	float ElapsedTime = App::GetApp()->GetElapsedTime();
	//	FPS += Util::FloatToWStr(ElapsedTime);
	//	FPS += L"\n";

	//	//�I�u�W�F�N�g��
	//	auto ObjCount = GetStage()->GetGameObjectVec().size();
	//	wstring OBJ_COUNT(L"OBJ_COUNT: ");
	//	OBJ_COUNT += Util::UintToWStr(ObjCount);
	//	OBJ_COUNT += L"\n";

	//	auto Pos = GetComponent<Transform>()->GetPosition();
	//	wstring PositionStr(L"Position:\t");
	//	PositionStr += L"X=" + Util::FloatToWStr(Pos.x, 6, Util::FloatModify::Fixed) + L",\t";
	//	PositionStr += L"Y=" + Util::FloatToWStr(Pos.y, 6, Util::FloatModify::Fixed) + L",\t";
	//	PositionStr += L"Z=" + Util::FloatToWStr(Pos.z, 6, Util::FloatModify::Fixed) + L"\n";

	//	wstring RididStr(L"Velocity:\t");
	//	auto Velocity = GetComponent<Rigidbody>()->GetVelocity();
	//	RididStr += L"X=" + Util::FloatToWStr(Velocity.x, 6, Util::FloatModify::Fixed) + L",\t";
	//	RididStr += L"Y=" + Util::FloatToWStr(Velocity.y, 6, Util::FloatModify::Fixed) + L",\t";
	//	RididStr += L"Z=" + Util::FloatToWStr(Velocity.z, 6, Util::FloatModify::Fixed) + L"\n";


	//	wstring HitObjectStr(L"HitObject: ");
	//	if (GetComponent<Collision>()->GetHitObjectVec().size() > 0) {
	//		for (auto&v : GetComponent<Collision>()->GetHitObjectVec()) {
	//			HitObjectStr += Util::UintToWStr((UINT)v.get()) + L",";
	//		}
	//		HitObjectStr += L"\n";
	//	}
	//	else {
	//		HitObjectStr += L"NULL\n";
	//	}
	//	wstring str = FPS + OBJ_COUNT + PositionStr + RididStr + HitObjectStr;
	//	//�����������
	//	auto PtrString = GetComponent<StringSprite>();
	//	PtrString->SetText(str);

	//---------------------------------------------------------------------
	///���˂��鋅��
	//---------------------------------------------------------------------

	CannonBall::CannonBall(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity) :
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity),
		m_Scale(0.25f)
	{}
	CannonBall::~CannonBall() {}


	//������
	void CannonBall::OnCreate() {
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellGroup");
		//�O���[�v�ɒǉ�
		Group->IntoGroup(GetThis<GameObject>());

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Emitter);

		//Rigidbody������
		auto PtrRedid = AddComponent<Rigidbody>();
		PtrRedid->SetVelocity(m_Velocity);
		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionSphere>();
		PtrCol->SetIsHitAction(IsHitAction::None);


		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");


	}

	void CannonBall::OnUpdate() {
		auto PtrGrav = GetBehavior<Gravity>();
		PtrGrav->Execute();
	}

	void CannonBall::OnUpdate2() {
		auto PtrTransform = GetComponent<Transform>();
		if (PtrTransform->GetPosition().y < -0.5f) {
			Erase();
		}
		if (GetComponent<Collision>()->GetHitObjectVec().size() > 0) {
			for (auto& v : GetComponent<Collision>()->GetHitObjectVec()) {
				auto& ptr = dynamic_pointer_cast<Cannon>(v);
				if (ptr) {
					GetStage()->RemoveGameObject<Cannon>(ptr);


				}
			}
		}
	}


	void CannonBall::Erase() {
		SetUpdateActive(false);
		SetDrawActive(false);
	}

	void CannonBall::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		SetUpdateActive(true);
		SetDrawActive(true);

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->ResetPosition(Emitter);
		auto PtrRedid = GetComponent<Rigidbody>();
		//�d�͂�0�ɂ���
		PtrRedid->SetGravityVelocityZero();
		PtrRedid->SetVelocity(Velocity);
	}

	//�h�A
	Door::Door(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale, const Vec3& StartRot, const Col4 color) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Rot(StartRot), m_Color(color)
	{
		Switchflag = false;
		m_PushFlag = false;
		m_move = Vec3(0.0f, -3.5f, 0.0f);
	}


	void Door::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		//�R���W������t����
		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetIsHitAction(IsHitAction::Stop);
		PtrCol->AddExcludeCollisionTag(L"Cube");
		PtrCol->AddExcludeCollisionTag(L"Wall");
		PtrCol->SetFixed(true);
		auto PtrRigid = AddComponent<Rigidbody>();
		//�`��
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"KOUSI_TX");
		PtrDraw->SetDepthStencilState(DepthStencilState::Read);
		PtrDraw->SetDiffuse(m_Color);
		SetAlphaActive(true);

		auto action = AddComponent<Action>();

		m_StateMachine.reset(new LayeredStateMachine<Door>(GetThis<Door>()));
		m_StateMachine->Reset(DoorDefalut::Instance());

		m_Pos2 = m_Pos;
		AddTag(L"Door");
		AddTag(L"Wall");
	}
	void Door::OnUpdate()
	{
		m_StateMachine->Update();
	}

	IMPLEMENT_SINGLETON_INSTANCE(DoorDefalut)
	void DoorDefalut::Enter(const shared_ptr<Door>& Obj)
	{
		Obj->SetSwichFlag(false);
	}
	void DoorDefalut::Execute(const shared_ptr<Door>& Obj)
	{

	}
	void DoorDefalut::Exit(const shared_ptr<Door>& Obj)
	{
	}

	IMPLEMENT_SINGLETON_INSTANCE(DoorMove)
	void DoorMove::CreateResourses()
	{
		wstring datadir;
		auto& app = App::GetApp();
		//�T�E���h
		app->GetDataDirectory(datadir);
		datadir += L"SE\\";
		auto SE = datadir + L"Door.wav";
		app->RegisterWav(L"Door", SE);
	}

	void DoorMove::Enter(const shared_ptr<Door>& Obj)
	{
	}
	void DoorMove::Execute(const shared_ptr<Door>& Obj)
	{
		auto action = Obj->GetComponent<Action>();
		if (action->GetArrived() && !Obj->GetSwichFlag())
		{

			action->AllActionClear();
			//action->AddMoveBy(1.0f, Obj->GetMove() * -1.0f);
			action->AddMoveBy(1.0f, Vec3(0.0f, Obj->GetMove().y * -1.0f, 0.0f));
			action->Run();
			Obj->SetMove(Obj->GetMove() * -1.0f);
			Obj->SetSwichFlag(true);
			m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
			m_AudioObjectPtr->AddAudioResource(L"Door");
			m_AudioObjectPtr->Start(L"Door", 0, 0.7f);

		}
		else if(action->GetArrived() && Obj->GetSwichFlag())
		{
			if (!Obj->GetPushFlag())
			{
				Obj->GetStateMachine()->Reset(DoorDefalut::Instance());
			}
		}
	}

	void DoorMove::Exit(const shared_ptr<Door>& Obj)
	{

	}
}