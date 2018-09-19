#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------
	
	//------------------------------------------------------------------
	///壊れる壁
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
	///水車 奥山
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

	// 水車の足場

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

	//初期化
	void WaterWheelChild::OnCreate() {

		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();

		auto PtrCol = AddComponent<CollisionObb>();

		auto WaterWheelptr = m_Parent.lock();
		if (!WaterWheelptr) {
			throw BaseException(
				L"水車が見つかりません",
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

		//親子関係にする
		GetComponent<Transform>()->SetParent(WaterWheelptr);

		//衝突判定をつける
		auto WaterWheelCol = AddComponent<CollisionObb>();
		//プレイヤーとは衝突しない
		WaterWheelCol->AddExcludeCollisionTag(L"WaterWheel");
		WaterWheelCol->SetDrawActive(true);
		//ヒット時のアクションは自分で記述
		//WaterWheelCol->SetIsHitAction(IsHitAction::None);
		WaterWheelCol->SetFixed(true);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//描画するテクスチャを設定
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
		//プレイヤーチャイルドが何かに当たった
		//プレイヤーとは衝突しないので、プレイヤー以外の何か

	}

	//------------------------------------------------------------------
	///スイッチ
	//------------------------------------------------------------------
	Switch::Switch(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale, const Col4& color) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Color(color)
	{}

	void Switch::CreateResourses()
	{
		wstring datadir;
		auto& app = App::GetApp();
		//サウンド
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

		//透過処理
		SetAlphaActive(true);
	}

	void Switch::OnUpdate()
	{
		ColorLimit();
	}

	void Switch::ColorLimit()
	{
		//赤
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
		//青
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
		//黄色
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
		//自分の
		auto TransPos = GetComponent<Transform>()->GetPosition();
		auto TransSle = GetComponent<Transform>()->GetScale();
		//Playerの
		auto Shad = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto transPos = Shad->GetComponent<Transform>()->GetPosition();

		auto OBJ = Shad->GetMyShell();
		//距離計算
		auto DisPos = Distance(TransPos.x, TransPos.z, transPos.x, transPos.z);
		float discol = Distance(TransPos.x, TransPos.y, transPos.x, transPos.y);

		//殻を背負っているか
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
	///大砲
	//------------------------------------------------------------------
	Cannon::Cannon(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{
	}


	//// yボタンで弾を撃つ
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
	//			//空きが見つかった
	//			shptr->Reset(Pos, velo);
	//			return;
	//		}
	//	}
	//	//ここまで来てれば空きがない
	//	GetStage()->AddGameObject<ShellSphere>(Pos, velo);

	//}

	void Cannon::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(Vec3(0.0f, 0.0f, 0.0f));
		//コリジョンを付ける
		auto PtrCol = AddComponent<CollisionObb>();


		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");

		//ステートマシン
		m_StateMachine.reset(new StateMachine<Cannon>(GetThis<Cannon>()));
		//最初のステートをSeekFarStateに設定
		//m_StateMachine->ChangeState(BoxDefaultState::Instance());

	}

	void Cannon::OnUpdate()
	{
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}
	//プレイヤーとの距離を得る
	float Cannon::GetToPlayerLen() const {
		auto Ptr = GetComponent<Transform>();
		Vec3 Pos = Ptr->GetPosition();
		auto PlayerPos
			= GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>()->GetPosition();
		return length(Pos - PlayerPos);
	}

	//砲弾発射行動
	void Cannon::FirCannonBehavior(int div) {
		if (Util::DivProbability(div)) {
			//200分の1の確率で発射
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
					//空きが見つかったn2f2f
					shptr->Reset(Pos, velo);
					return;
				}
			}
			//ここまで来てれば空きがない
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
	//	//Yボタン
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
		//文字列の表示
		//DrawStrings();
	}

	//void Cannon::DrawStrings()
	//{
	//}



	//文字列の表示
	//void Cannon::DrawStrings() {


	//	auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
	//	wstring FPS(L"FPS: ");
	//	FPS += Util::UintToWStr(fps);
	//	FPS += L"\nElapsedTime: ";
	//	float ElapsedTime = App::GetApp()->GetElapsedTime();
	//	FPS += Util::FloatToWStr(ElapsedTime);
	//	FPS += L"\n";

	//	//オブジェクト数
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
	//	//文字列をつける
	//	auto PtrString = GetComponent<StringSprite>();
	//	PtrString->SetText(str);

	//---------------------------------------------------------------------
	///発射する球体
	//---------------------------------------------------------------------

	CannonBall::CannonBall(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity) :
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity),
		m_Scale(0.25f)
	{}
	CannonBall::~CannonBall() {}


	//初期化
	void CannonBall::OnCreate() {
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellGroup");
		//グループに追加
		Group->IntoGroup(GetThis<GameObject>());

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(Quat());
		PtrTransform->SetPosition(m_Emitter);

		//Rigidbodyをつける
		auto PtrRedid = AddComponent<Rigidbody>();
		PtrRedid->SetVelocity(m_Velocity);
		//衝突判定をつける
		auto PtrCol = AddComponent<CollisionSphere>();
		PtrCol->SetIsHitAction(IsHitAction::None);


		//影をつける
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
		//重力を0にする
		PtrRedid->SetGravityVelocityZero();
		PtrRedid->SetVelocity(Velocity);
	}

	//ドア
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
		//コリジョンを付ける
		auto PtrCol = AddComponent<CollisionObb>();
		PtrCol->SetIsHitAction(IsHitAction::Stop);
		PtrCol->AddExcludeCollisionTag(L"Cube");
		PtrCol->AddExcludeCollisionTag(L"Wall");
		PtrCol->SetFixed(true);
		auto PtrRigid = AddComponent<Rigidbody>();
		//描画
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
		//サウンド
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