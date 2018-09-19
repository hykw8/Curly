#include "stdafx.h"
#include "Project.h"
#include "UI.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------


	Fade::Fade(const shared_ptr<Stage>& StagePtr, const Vec2 & StartPos, const Vec2 & StartScale, const bool Flag) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Flag(Flag)
	{}

	void Fade::OnCreate()
	{
		float halfSize = 0.5f;

		m_BackupVertices =
		{
			{ VertexPositionColor(Vec3(-halfSize, halfSize, 0), Col4(0.0f, 0.0f, 0.0f, 1.0f))},
			{ VertexPositionColor(Vec3(halfSize, halfSize, 0), Col4(0.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-halfSize, -halfSize, 0), Col4(0.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(halfSize, -halfSize, 0), Col4(0.0f, 0.0f, 0.0f, 1.0f)) },
		};
		vector<uint16_t> indices = { 0,1,2,1,3,2 };
		SetAlphaActive(true);

		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos.x, m_Pos.y, 0.0f);
		PtrTrans->SetScale(m_Scale.x, m_Scale.y, 1.0f);

		auto PtrDraw = AddComponent<PCSpriteDraw>(m_BackupVertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearClamp);

		if (m_Flag)
		{
			m_TotalTime = 0.0f;
		}
		else
		{
			m_TotalTime = 1.0f;
		}

		SetDrawLayer(100);
	}

	void Fade::OnUpdate()
	{
		if (m_Flag)
		{
			Infade();
		}
		else
		{
			Outfade();
		}
	}

	void Fade::Infade()
	{
		vector<VertexPositionColor> NewVertices;
		for (size_t i = 0; i < m_BackupVertices.size(); i++)
		{
			Col4 col = m_BackupVertices[i].color;
			col.w = m_TotalTime;
			auto v = VertexPositionColor
			(
				m_BackupVertices[i].position,
				col
			);
			NewVertices.push_back(v);
		}
		auto PtrDraw = GetComponent<PCSpriteDraw>();
		PtrDraw->UpdateVertices(NewVertices);

		//１秒経ったら削除
		if (m_TotalTime >= 1.0f)
		{
			GetStage()->RemoveGameObject<Fade>(GetThis<Fade>());
		}

		auto time = App::GetApp()->GetElapsedTime();
		m_TotalTime += time;
	}

	void Fade::Outfade()
	{
		vector<VertexPositionColor> NewVertices;
		for (size_t i = 0; i < m_BackupVertices.size(); i++)
		{
			Col4 col = m_BackupVertices[i].color;
			col.w = m_TotalTime;
			auto v = VertexPositionColor
			(
				m_BackupVertices[i].position,
				col
			);
			NewVertices.push_back(v);
		}
		auto PtrDraw = GetComponent<PCSpriteDraw>();
		PtrDraw->UpdateVertices(NewVertices);

		//0より小さくなったら削除
		if (m_TotalTime <= 0.0f)
		{
			GetStage()->RemoveGameObject<Fade>(GetThis<Fade>());
		}

		auto time = App::GetApp()->GetElapsedTime();
		m_TotalTime -= time;
	}

	//-------------------------------------------------------------------------
	//	class NumberSprite : public GameObject
	//	用途 : 数字変換用
	//-------------------------------------------------------------------------
	NumberSprite::NumberSprite(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{}

	void NumberSprite::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		PtrTrans->SetScale(m_Scale);

		auto PtrSprite = AddComponent<PCTSpriteDraw>();
		PtrSprite->SetTextureResource(L"TIMER_TX");
		SetAlphaActive(true);
		//頂点のバックアップの取得
		auto& SpVertexVec = PtrSprite->GetMeshResource()->GetBackupVerteces<VertexPositionColorTexture>();
		auto Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		//各数字ごとにUV値を含む頂点データを配列化する
		for (size_t i = 0; i < 10; i++)
		{
			float from = static_cast<float>(i) / 10.0f;
			float to = from + (1.0f / 10.0f);
			vector<VertexPositionColorTexture> NumVirtex =
			{
				//左上頂点
				VertexPositionColorTexture
				(
					SpVertexVec[0].position,
					Color,
					Vec2(from,0.0f)
				),
				//右上頂点
				VertexPositionColorTexture
				(
					SpVertexVec[1].position,
					Color,
					Vec2(to,0.0f)
				),
				//左下頂点
				VertexPositionColorTexture
				(
					SpVertexVec[2].position,
					Color,
					Vec2(from,1.0f)
				),
				//右下頂点
				VertexPositionColorTexture
				(
					SpVertexVec[3].position,
					Color,
					Vec2(to,1.0f)
				)
			};
			m_NumVertexVec.push_back(NumVirtex);
		}
	}

	void NumberSprite::OnUpdate()
	{
		m_Num = m_Num % 10;
		auto PtrSprite = GetComponent<PCTSpriteDraw>();
		auto MeshRes = PtrSprite->GetMeshResource();
		//動的にUV値を変える
		MeshRes->UpdateVirtexBuffer(m_NumVertexVec[m_Num]);
	}

	//集める物の数表示
	GoalCount::GoalCount(const shared_ptr<Stage>& StagePtr, const Vec3 & StartPos, const Vec3 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{}

	void GoalCount::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);

		CreateUI();
	}

	void GoalCount::OnUpdate()
	{
		Counter();
	}

	void GoalCount::Counter()
	{
		int count = 0;
		auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());

		for (auto v : GetStage()->GetSharedObjectGroup(L"GoalItemGroup")->GetGroupVector())
		{
			count++;
		}
		m_CollectionCount = m_Count - count;
		
		for (int i = 0; i < m_CollectionCount; i++)
		{
			m_UI[i].lock()->GetComponent<PCTSpriteDraw>()->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
			m_UI[i].lock()->GetComponent<Action>()->SetLooped(true);
			m_UI[i].lock()->GetComponent<Action>()->SetUpdateActive(true);
		}

		if (m_Count == m_CollectionCount)
		{
			camera->SetGoalFlag(true);
		}

		if (camera->GetStartFlag())
		{
			SetDrawActive(false);
		}
		else
		{
			SetDrawActive(true);
		}
	}

	void GoalCount::CreateUI()
	{
		int count = 0;
		float offSet = 90.0f;
		auto pos = GetComponent<Transform>()->GetPosition();
		for (auto v : GetStage()->GetSharedObjectGroup(L"GoalItemGroup")->GetGroupVector())
		{
			count++;

			m_UI.push_back(GetStage()->AddGameObject<OnlyDisPlay>(Vec3(625.0f / 7.0f, 625.0f / 7.0f, 1.0f), Vec3(pos.x + (offSet * count - 1), -340.0f, 0.0f), Vec3(0.0f,0.0f,-10.0f * XM_PI / 180.0f), L"SANGO_TX"));
			m_UI[count - 1].lock()->GetComponent<PCTSpriteDraw>()->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 1.0f));
			auto action = m_UI[count - 1].lock()->GetComponent<Action>();
			action->AddRotateBy(1.0f, Vec3(0.0f, 0.0f, 25.0f * XM_PI / 180.0f));
			action->AddRotateBy(1.0f, Vec3(0.0f, 0.0f, -25.0f * XM_PI / 180.0f));
			action->SetLooped(false);
			action->Run();
			action->SetUpdateActive(false);
		}

		m_Count = count;
	}

	//	のこりサンゴ
	OnlyDisPlay::OnlyDisPlay(const shared_ptr<Stage>& StagePtr, const Vec3& Scale, const Vec3& Pos, wstring TexturePtr) :
		GameObject(StagePtr),	
		m_Scale(Scale),
		m_Pos(Pos),
		m_TextureKey(TexturePtr)
	{
		m_Rot = Vec3(0);
	}

	OnlyDisPlay::OnlyDisPlay(const shared_ptr<Stage>& StagePtr, const Vec3 & Scale, const Vec3 & Pos, const Vec3 & Rotation, wstring TexturePtr):
		GameObject(StagePtr),
		m_Pos(Pos),
		m_Scale(Scale),
		m_Rot(Rotation),
		m_TextureKey(TexturePtr)
	{}

	void OnlyDisPlay::OnCreate()
	{

		float HelfSize = 0.5f; 
		//頂点配列
			m_BackupVertices = {
				{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
				{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
			};

		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetPosition(m_Pos);
		PtrTrans->SetRotation(m_Rot);
		PtrTrans->SetScale(m_Scale);

		auto PtrDraw = AddComponent<PCTSpriteDraw>(m_BackupVertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);

		AddComponent<Action>();

	}

	void OnlyDisPlay::OnUpdate() 
	{
		auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (camera->GetStartFlag())
		{
			SetDrawActive(false);
		}
		else
		{
			SetDrawActive(true);
		}
	}
	//TitleのPressAnyButton
	PressAnyButton::PressAnyButton(const shared_ptr<Stage>& StagePtr, const Vec2 & StartPos, const Vec2 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{
		m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	PressAnyButton::PressAnyButton(const shared_ptr<Stage>& StagePtr, const Vec2 & StartPos, const Vec2 & StartScale, Col4 color) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Color(color)
	{}
	PressAnyButton::PressAnyButton(const shared_ptr<Stage>& StagePtr, const Vec2 & StartPos, const Vec2 & StartScale, wstring TexturePtr) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Texture(TexturePtr)
	{
		m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void PressAnyButton::OnCreate()
	{
		float HelfSize = 0.5f;
		// 頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(2126.0f * m_Scale.x, 591.0f * m_Scale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_Pos.x, m_Pos.y, 0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_Texture);
		PtrDraw->SetDiffuse(m_Color);
	}

	void PressAnyButton::OnUpdate()
	{
		auto time = App::GetApp()->GetElapsedTime();
		auto ptrDraw = GetComponent<PCTSpriteDraw>();
		m_TotalTime += time * 1.5f;
		if (m_TotalTime >= XM_PI)
		{
			m_TotalTime = 0.0f;
		}

		ptrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, sin(m_TotalTime)));

		vector<VertexPositionColorTexture> NewVertices;
		for (size_t i = 0; i < m_BackupVertices.size(); i++)
		{
			Col4 col = m_BackupVertices[i].color;
			col.w = m_TotalTime;
			auto v = VertexPositionColorTexture
			(
				m_BackupVertices[i].position,
				col,
				m_BackupVertices[i].textureCoordinate
			);
			NewVertices.push_back(v);
		}
	}

	//ゴール時に表示するUI
	GoalUI::GoalUI(const shared_ptr<Stage>& StagePtr, const Vec2 & StartPos, const Vec2 & StartScale, wstring TexturePtr) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Texture(TexturePtr)
	{}
	void GoalUI::OnCreate()
	{
		float HelfSize = 0.5f;
		// 頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale.x, m_Scale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_Pos.x, m_Pos.y, 0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_Texture);
	}
	void GoalUI::OnUpdate()
	{

	}

	//ゴールUIをまとめるやつ
	GoalAllUI::GoalAllUI(const shared_ptr<Stage>& StagePtr, const Vec2 & StartPos, const Vec2 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Num(0), m_ViewFlag(false)
	{}

	void GoalAllUI::CreateResourses()
	{
		wstring datadir;
		auto& app = App::GetApp();
		//サウンド
		app->GetDataDirectory(datadir);
		datadir += L"SE\\";
		auto SE = datadir + L"OK.wav";
		app->RegisterWav(L"OK", SE);

		SE = datadir + L"Select.wav";
		app->RegisterWav(L"Select", SE);
	}

	void GoalAllUI::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_Pos.x, m_Pos.y, 0.0f);
		ptrTrans->SetScale(m_Scale.x, m_Scale.y, 1.0f);

		m_NextUI = GetStage()->AddGameObject<GoalUI>(Vec2(-250.0f + m_Pos.x, -150.0f + m_Pos.y), Vec2(m_Scale * 0.1f), L"NextSatge2W_TX");
		m_NextUI.lock()->SetDrawActive(false);
		m_Select = GetStage()->AddGameObject<GoalUI>(Vec2(250.0f + m_Pos.x, -150.0f + m_Pos.y), Vec2(m_Scale * 0.1f), L"StageSelect2W_TX");
		m_Select.lock()->SetDrawActive(false);

		auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (camera)
		{
			camera->SetGoalUI(GetThis<GoalAllUI>());
		}
	}
	void GoalAllUI::OnUpdate()
	{
		auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (m_ViewFlag)
		{
			m_NextUI.lock()->SetDrawActive(true);
			m_Select.lock()->SetDrawActive(true);

			auto ptrBehavior = GetBehavior<PlayerBehavior>();
			int num = 1;;
			auto scene = App::GetApp()->GetScene<Scene>();
			if (scene->GetStageNum() >= 10)
			{
				m_NextUI.lock()->SetDrawActive(false);
			}
			else
			{
				num = ptrBehavior->Choose(m_Num, 1);
			}

			switch (num)
			{
			case 0:
				m_NextUI.lock()->GetThis<GoalUI>()->GetComponent<Transform>()->SetScale(m_Scale.x * 0.3f, m_Scale.y * 0.3f, 1.0f);
				m_Select.lock()->GetThis<GoalUI>()->GetComponent<Transform>()->SetScale(m_Scale.x * 0.2f, m_Scale.y * 0.2f, 1.0f);

				break;
			case 1:
				m_Select.lock()->GetThis<GoalUI>()->GetComponent<Transform>()->SetScale(m_Scale.x * 0.3f, m_Scale.y * 0.3f, 1.0f);
				m_NextUI.lock()->GetThis<GoalUI>()->GetComponent<Transform>()->SetScale(m_Scale.x * 0.2f, m_Scale.y * 0.2f, 1.0f);
				break;
			}

			if (m_Num != num)
			{
				m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
				m_AudioObjectPtr->AddAudioResource(L"Select");
				m_AudioObjectPtr->Start(L"Select", 0, 1.0f);
			}

			m_Num = num;

			auto CntVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntVec[0].bConnected)
			{
				if (CntVec[0].wPressedButtons & XINPUT_GAMEPAD_A)
				{
					auto scene = App::GetApp()->GetScene<Scene>();
					switch (num)
					{
					case 0:
						scene->SetStageNum(scene->GetStageNum() + 1);
						//scene->m_StageName = L"NextStage";
						scene->SetStageName(L"GameStage");
						m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
						m_AudioObjectPtr->AddAudioResource(L"OK");
						m_AudioObjectPtr->Start(L"OK", 0, 1.0f);
						scene->SetCheckPoint(false);
						PostEvent(1.0f, GetThis<ObjectInterface>(), scene, L"ToGameStage");
						break;
					case 1:
						//scene->m_StageName = L"StageSelect";
						scene->SetStageName(L"StageSelect");
						m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
						m_AudioObjectPtr->AddAudioResource(L"OK");
						m_AudioObjectPtr->Start(L"OK", 0, 1.0f);
						scene->SetCheckPoint(false);
						PostEvent(1.0f, GetThis<ObjectInterface>(), scene, L"ToStageSelect");
						break;
					default:
						break;
					}
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------







	// タイトルスプライト

	TitleSprite::TitleSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& TitleScale, const Vec2& TitlePos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_TitleScale(TitleScale),
		m_TitlePos(TitlePos)

	{}

	TitleSprite::~TitleSprite() {}

	void TitleSprite::OnCreate() {
		float HelfSize = 0.5f;
		// 頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_TitleScale.x, m_TitleScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_TitlePos.x, m_TitlePos.y, 0.0f);

		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
	}

	//タイトルロゴ
	TitleLogo::TitleLogo(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& TitleScale, const Vec2& TitlePos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_TitleScale(TitleScale),
		m_TitlePos(TitlePos)

	{}

	TitleLogo::~TitleLogo() {}

	void TitleLogo::OnCreate() {
		float HelfSize = 0.5f;
		// 頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_TitleScale.x, m_TitleScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_TitlePos.x, m_TitlePos.y, 0.0f);

		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
	}



	// ステージセレクトスプライト

	StageSelectSprite::StageSelectSprite(const shared_ptr<Stage>& StagePtr, const wstring& StageSelectKey, bool Trace,
		const Vec2& StageSelectScale, const Vec2& StageSelectPos) :
		GameObject(StagePtr),
		m_StageSelectKey(StageSelectKey),
		m_Trace(Trace),
		m_StageSelectScale(StageSelectScale),
		m_StageSelectPos(StageSelectPos)

	{}

	StageSelectSprite::~StageSelectSprite() {}

	void StageSelectSprite::OnCreate() {
		float HelfSize = 0.5f;
		// 頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StageSelectScale.x, m_StageSelectScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StageSelectPos.x, m_StageSelectPos.y,1.0f);

		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_StageSelectKey);
		SetDrawLayer(2);
	}

	// スキルゲージ
	SkillGauge::SkillGauge(const shared_ptr<Stage>& StagePtr,bool StartGauge, const Vec3 & StartPos, const Vec2 & StartScale) :
		GameObject(StagePtr),
		m_Gauge(StartGauge),
		m_Pos(StartPos),
		m_Scale(StartScale),
		m_TotalTime (0)
	{}

	SkillGauge::~SkillGauge()
	{}

	void SkillGauge::OnCreate() 
	{
		float HelfSize = 0.5f;
		//頂点配列
		//m_BackupVertices = {
		//	{ VertexPositionColor(Vec3(-HelfSize,+HelfSize, 0), Col4(0.9f, 0.8f, 0.0f, 1.0f)) },
		//	{ VertexPositionColor(Vec3(+HelfSize,+HelfSize, 0), Col4(0.9f, 0.8f, 0.0f, 1.0f)) },
		//	{ VertexPositionColor(Vec3(-HelfSize,-HelfSize, 0), Col4(0.9f, 0.8f, 0.0f, 1.0f)) },
		//	{ VertexPositionColor(Vec3(+HelfSize,-HelfSize, 0), Col4(0.9f, 0.8f, 0.0f, 1.0f)) },
		//};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Gauge);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale.x, m_Scale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_Pos);
		//頂点とインデックスを指定してスプライト作成
		AddComponent<PCSpriteDraw>(m_BackupVertices, indices);
	}

	void SkillGauge::OnUpdate() 
	{
		
		//Energy(10.0f);
		//BalloonGauge(100.0f);
	}

	//void SkillGauge::Energy(float point1, float point2)
	//{

	//	float HelfSize = 0.5f;
	//	//頂点配列
	//	m_BackupVertices = {
	//		{ VertexPositionColor(Vec3(-HelfSize,+HelfSize, 0), Col4(0.9f, 0.8f, 0.0f, 1.0f)) },
	//		{ VertexPositionColor(Vec3(+HelfSize,+HelfSize, 0), Col4(0.9f, 0.8f, 0.0f, 1.0f)) },
	//		{ VertexPositionColor(Vec3(-HelfSize,-HelfSize, 0), Col4(0.9f, 0.8f, 0.0f, 1.0f)) },
	//		{ VertexPositionColor(Vec3(+HelfSize,-HelfSize, 0), Col4(0.9f, 0.8f, 0.0f, 1.0f)) },
	//	};
	//	//インデックス配列
	//	vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };

	//	//頂点とインデックスを指定してスプライト作成
	//	AddComponent<PCSpriteDraw>(m_BackupVertices, indices);

	//	m_Scale.y = point1 * 6;
	//	//m_Pos.y -=(downPoint*m_Pos.y)/2;
	//	//downPoint = 0.0f;
	//	if (m_Scale.y < 0.0f)
	//	{
	//		m_Scale.y = 0.0f;
	//	}
	//}

	void SkillGauge::WaterGunGauge(float point1,float point2)
	{
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_Pos.x, m_Pos.y, m_Pos.z);
		PtrTransform->SetScale(m_Scale.x, m_Scale.y, 1.0f);

		float HelfSize = 0.5f;
		//頂点配列
		m_BackupVertices = {
			{ VertexPositionColor(Vec3(-HelfSize,+HelfSize, 0), Col4(0.0f, 1.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(+HelfSize,+HelfSize, 0), Col4(0.0f, 1.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize,-HelfSize, 0), Col4(0.0f, 1.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(+HelfSize,-HelfSize, 0), Col4(0.0f, 1.0f, 1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };

		//頂点とインデックスを指定してスプライト作成
		AddComponent<PCSpriteDraw>(m_BackupVertices, indices);

		m_Scale.y = 300 * (point1 / point2);
		if (m_Scale.y < 0.0f)
		{
			m_Scale.y = 0.0f;
		}
		else if(m_Scale.y > 300.0f) 
		{
			m_Scale.y = 300.0f;
		}
	}

	void SkillGauge::BalloonGauge(float point1) {

		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_Pos.x, m_Pos.y, m_Pos.z);
		PtrTransform->SetScale(m_Scale.x, m_Scale.y, 1.0f);

		float HelfSize = 0.5f;
		//頂点配列
		m_BackupVertices = {
			{ VertexPositionColor(Vec3(-HelfSize,+HelfSize, 0), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(+HelfSize,+HelfSize, 0), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize,-HelfSize, 0), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(+HelfSize,-HelfSize, 0), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };

		//頂点とインデックスを指定してスプライト作成
		AddComponent<PCSpriteDraw>(m_BackupVertices, indices);


		m_Scale.y = (300.0f * (1.0f -(point1/1.0f)) * 1.5f);

		if (m_Scale.y < 0.0f)
		{
			float downPoint = 1.0f;
			m_Scale.y -= downPoint;
			m_Pos.y -= downPoint / 2;
			if (m_Scale.y < 0.0f)
			{
				m_Scale.y = 0.0f;
			}
		}

	}

	// StageImage
	StageImage::StageImage(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace, const Vec3& StartPos, const Vec2& StartScale,const int& StartStage):
		GameObject(StagePtr), m_TextureKey(TextureKey),m_Trace(Trace),
		m_Pos(StartPos),m_Scale(StartScale),m_StageNum(StartStage)
	{}

	void StageImage::OnCreate() {
		float HelfSize = 0.5f;
		vector<VertexPositionColorTexture> vertices;
		vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
		};

		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale.x, m_Scale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_Pos);
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);

		NumberSpritePtr.push_back(GetStage()->AddGameObject<NumberSprite>(m_Pos, Vec3(m_Scale.x,m_Scale.y,1.0f)));
		NumberSpritePtr.push_back(GetStage()->AddGameObject<NumberSprite>(Vec3(m_Pos.x - 17.0f, m_Pos.y, m_Pos.z), Vec3(m_Scale.x, m_Scale.y, 1.0f)));

		SetDrawLayer(1);
	}

	void StageImage::OnUpdate() {
		StageImageCounter();
	}

	void StageImage::StageImageCounter() {
		int count = 0;
		if (m_StageNum < 10)
		{
			NumberSpritePtr[0]->SetNum(m_StageNum);
			NumberSpritePtr[1]->SetDrawActive(false);
		}
		else
		{
			auto ten = m_StageNum / 10;
			NumberSpritePtr[1]->SetNum(ten);
			NumberSpritePtr[0]->SetNum(m_StageNum);
			NumberSpritePtr[0]->GetComponent<Transform>()->SetPosition(m_Pos.x + 17.0f, m_Pos.y, m_Pos.z);
		}
	}


	//SelectCursor
	SelectCursor::SelectCursor(const shared_ptr<Stage>& StagePtr, const bool& StartFlg, const Vec3& StartPos, const Vec2& StartScale, const float StartMove):
		GameObject(StagePtr),m_CursorMoveFlg(StartFlg), m_Pos(StartPos), m_Scale(StartScale),m_CursorMove(StartMove)
	{
		m_BeforeNum = 0;
		m_ToatalTime = 1.0f;
		m_ScaleTime = 0.5f;
		m_DefalutScaleTime = m_ScaleTime;
	}

	void SelectCursor::CreateResourses()
	{
		wstring datadir;
		auto& app = App::GetApp();
		//サウンド
		app->GetDataDirectory(datadir);
		datadir += L"SE\\";
		auto SE = datadir + L"OK.wav";
		app->RegisterWav(L"OK", SE);

		SE = datadir + L"Select.wav";
		app->RegisterWav(L"Select", SE);
	}
	void SelectCursor::StageSelector() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto stageDisplay = GetStage()->GetSharedGameObject<SelectStageDisplay>(L"SelectStageDisplay");
		auto PtrTransform = GetComponent<Transform>();


		//float xpos = 250.0f;
		//float ypos = 250.0f;

		PtrTransform->SetPosition(m_Pos);

		int nowNum = m_InputHandler.SelectCursor(m_BeforeNum, 9);

		if (m_ToatalTime >= 0.15f)
		//if (m_ToatalTime == 1.0f)
		{
			switch (nowNum)
			{
			// 1
			case static_cast<int>(pos0_0) :
				m_Pos.x = -300.0f;
				m_Pos.y = -235.0f;
				pos = pos0_0;
				break;
			// 2
			case static_cast<int>(pos1_0) :
				m_Pos.x = -150.0f;
				m_Pos.y = -235.0f;
				pos = pos1_0;
				break;
			// 3
			case static_cast<int>(pos2_0) :
				m_Pos.x = 0.0f;
				m_Pos.y = -235.0f;
				pos = pos2_0;
				break;
			// 4
			case static_cast<int>(pos3_0) :
				m_Pos.x = 150.0f;
				m_Pos.y = -235.0f;
				pos = pos3_0;
				break;
			// 5
			case static_cast<int>(pos4_0) :
				m_Pos.x = 300.0f;
				m_Pos.y = -235.0f;
				pos = pos4_0;
				break;
			// 6
			case static_cast<int>(pos0_1) :
				m_Pos.x = -300.0f;
				m_Pos.y = -345.0f;
				pos = pos0_1;
				break;
			// 7
			case static_cast<int>(pos1_1) :
				m_Pos.x = -150.0f;
				m_Pos.y = -345.0f;
				pos = pos1_1;
				break;
			// 8
			case static_cast<int>(pos2_1) :
				m_Pos.x = 0.0f;
				m_Pos.y = -345.0f;
				pos = pos2_1;
				break;
			// 9
			case static_cast<int>(pos3_1) :
				m_Pos.x = 150.0f;
				m_Pos.y = -345.0f;
				pos = pos3_1;
				break;
			// 10
			case static_cast<int>(pos4_1) :
				m_Pos.x = 300.0f;
				m_Pos.y = -345.0f;
				pos = pos4_1;
				break;
			default:
				break;
			}
			if (m_BeforeNum != nowNum)
			{
				m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
				m_AudioObjectPtr->AddAudioResource(L"Select");
				m_AudioObjectPtr->Start(L"Select", 0, 1.0f);
			}
			m_BeforeNum = nowNum;
			stageDisplay->m_StageNum = m_BeforeNum;
			m_ToatalTime = 0.0f;
		}
		else if(m_ToatalTime <= 0.15f)
		{
			auto time = App::GetApp()->GetElapsedTime();
			m_ToatalTime += time;
		}

		auto CntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto scene = App::GetApp()->GetScene<Scene>();
		if (CntVec[0].bConnected)
		{
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A)
			{
				GetStage()->AddGameObject<Fade>
					(
						Vec2(0.0f, 0.0f),
						Vec2(1280.0f, 800.0f),
						true
					);
				m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
				m_AudioObjectPtr->AddAudioResource(L"OK");
				m_AudioObjectPtr->Start(L"OK", 0, 1.0f);
				scene->SetStageNum(static_cast<int>(pos) + 1);
				//scene->m_StageName = L"GameStage";
				scene->SetStageName(L"GameStage");
				PostEvent(1.0f, GetThis<ObjectInterface>(), scene, L"ToGameStage");
				SetUpdateActive(false);
			}
		}
	}

	void SelectCursor::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize,+HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f,0.0f)) },
			{ VertexPositionColorTexture(Vec3(+HelfSize,+HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f,0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize,-HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f,1.0f)) },
			{ VertexPositionColorTexture(Vec3(+HelfSize,-HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f,1.0f)) },
		};

		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale.x, m_Scale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_Pos);
		//頂点とインデックスを指定してスプライト作成
		auto ptrDraw = AddComponent<PCTSpriteDraw>(m_BackupVertices, indices);
		ptrDraw->SetTextureResource(L"CURSOR_W_TX");
		SetDrawLayer(0);
	}

	void SelectCursor::OnUpdate() {
		StageSelector();

		auto time = App::GetApp()->GetElapsedTime();
		auto trans = GetComponent<Transform>();
		m_ScaleTime += time * 1.5f;
		if (m_ScaleTime >= XM_PI - m_DefalutScaleTime)
		{
			m_ScaleTime = m_DefalutScaleTime;
		}

		trans->SetScale(Vec3(m_Scale.x * sinf(m_ScaleTime), m_Scale.y * sinf(m_ScaleTime), 1.0f));	
	}

	UIWord::UIWord(const shared_ptr<Stage>& Stageptr, const Vec2& StartPos, const Vec2& StartScale, const wstring texture) :
		GameObject(Stageptr), m_pos(StartPos), m_Scale(StartScale), m_texture(texture)
	{}
	void UIWord::OnCreate()
	{
		float HelfSize = 0.5f;
		// 頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(1181.0f * m_Scale.x, 354.0f * m_Scale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_pos.x, m_pos.y, 0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_texture);
		SetDrawLayer(1);
	}

	UIType::UIType(const shared_ptr<Stage>& StagePtr, const Vec2 & StartPos, const Vec2 & StartScale, const shared_ptr<GameObject>& shell) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_point(shell)
	{}
	void UIType::OnCreate()
	{
		float HelfSize = 0.5f;
		// 頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(1181.0f * m_Scale.x, 354.0f * m_Scale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_Pos.x, m_Pos.y, 0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		SetDrawLayer(1);

		if (m_point.lock() != nullptr)
		{
			auto PtrShell = m_point.lock()->GetThis<Shell>()->GetName();

			if (PtrShell == L"Dash")
			{
				PtrDraw->SetTextureResource(L"Dash_TX");
			}
			else if (PtrShell == L"Temp")
			{
				PtrDraw->SetTextureResource(L"None_TX");
			}
			else if (PtrShell == L"Spin")
			{
				PtrDraw->SetTextureResource(L"Hovering2_TX");
			}
			else if (PtrShell == L"Ballon")
			{
				PtrDraw->SetTextureResource(L"Maueuku_TX");
			}
			else if (PtrShell == L"Iron")
			{
				PtrDraw->SetTextureResource(L"None_TX");
			}
		}
		else
		{
			PtrDraw->SetTextureResource(L"None_TX");
		}

		SetAlphaActive(true);
		SetDrawLayer(1);
	}

	void UIType::OnUpdate()
	{
		auto PtrDraw = GetComponent<PCTSpriteDraw>();
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PtrTrans = GetComponent<Transform>();
		m_point = player->GetMyShell();

		if (player->GetStateMachine()->GetTopState() == PlayerBurdenedState::Instance())
		{
			auto PtrShell = m_point.lock()->GetThis<Shell>()->GetName();

			if (PtrShell == L"Dash")
			{
				PtrTrans->SetScale(1181.0f * m_Scale.x * 1.0f, 354.0f * m_Scale.y * 1.0f, 1.0f);
				PtrDraw->SetTextureResource(L"Dash_TX");
			}
			else if (PtrShell == L"Temp")
			{
				PtrTrans->SetScale(1181.0f * m_Scale.x * 1.0f, 354.0f * m_Scale.y * 1.0f, 1.0f);
				PtrDraw->SetTextureResource(L"None_TX");
			}
			else if (PtrShell == L"Spin")
			{
				PtrTrans->SetScale(1181.0f * m_Scale.x * 0.8f, 354.0f * m_Scale.y * 0.8f, 1.0f);
				PtrDraw->SetTextureResource(L"Hovering_TX");
			}
			else if (PtrShell == L"Ballon")
			{
				PtrTrans->SetScale(1181.0f * m_Scale.x * 0.7f, 354.0f * m_Scale.y * 0.8f, 1.0f);
				PtrDraw->SetTextureResource(L"Maueuku_TX");
			}
			else if (PtrShell == L"Iron")
			{
				PtrTrans->SetScale(1181.0f * m_Scale.x * 1.0f, 354.0f * m_Scale.y * 1.0f, 1.0f);
				PtrDraw->SetTextureResource(L"None_TX");
			}
		}
		else
		{
			PtrTrans->SetScale(1181.0f * m_Scale.x * 1.0f, 354.0f * m_Scale.y * 1.0f, 1.0f);
			PtrDraw->SetTextureResource(L"None_TX");
		}
	}

	UIWeight::UIWeight(const shared_ptr<Stage>& StagePtr, const Vec2 & StartPos, const Vec2 & StartScale, const shared_ptr<GameObject>& shell) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_point(shell)
	{}
	void UIWeight::OnCreate()
	{
		float HelfSize = 0.5f;
		// 頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(1181.0f * m_Scale.x, 354.0f * m_Scale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_Pos.x, m_Pos.y, 0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		SetDrawLayer(1);

		if (m_point.lock() != nullptr)
		{
			auto PtrWeight = m_point.lock()->GetThis<Shell>()->GetWeight();

			switch (PtrWeight)
			{
			case basecross::Weight::m_VeryLight:
				break;
			case basecross::Weight::m_Light:
				PtrDraw->SetTextureResource(L"Weightlight_TX");
				break;
			case basecross::Weight::m_NomalWeight:
				PtrDraw->SetTextureResource(L"Weightnormal_TX");
				break;
			case basecross::Weight::m_Heavy:
				PtrDraw->SetTextureResource(L"Weightheavy_TX");
				break;
			case basecross::Weight::m_VeryHeavy:
				break;
			default:
				break;
			}
		}
		else
		{
			PtrDraw->SetTextureResource(L"None_TX");
		}

		SetAlphaActive(true);
	}

	void UIWeight::OnUpdate()
	{
		auto PtrDraw = GetComponent<PCTSpriteDraw>();
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		m_point = player->GetMyShell();

		if (player->GetStateMachine()->GetTopState() == PlayerBurdenedState::Instance())
		{

			auto PtrWeight = m_point.lock()->GetThis<Shell>()->GetWeight();

			switch (PtrWeight)
			{
			case basecross::Weight::m_VeryLight:
				break;
			case basecross::Weight::m_Light:
				PtrDraw->SetTextureResource(L"Weightlight_TX");
				break;
			case basecross::Weight::m_NomalWeight:
				PtrDraw->SetTextureResource(L"Weightnormal_TX");
				break;
			case basecross::Weight::m_Heavy:
				PtrDraw->SetTextureResource(L"Weightheavy_TX");
				break;
			case basecross::Weight::m_VeryHeavy:
				break;
			default:
				break;
			}
		}
		else
		{
			PtrDraw->SetTextureResource(L"None_TX");
		}
	}

	hukidasi::hukidasi(const shared_ptr<Stage>& StagePtr, const Vec2 & Startpos, const Vec2 & StartScale) :
		GameObject(StagePtr), m_Pos(Startpos), m_Scale(StartScale)
	{}
	void hukidasi::OnCreate()
	{
		float HelfSize = 0.5f;
		// 頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale.x, m_Scale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_Pos.x, m_Pos.y, 0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(L"Speech_balloon3_TX");

		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");

		m_ui[0] = GetStage()->AddGameObject<UIWord>(Vec2(m_Pos.x - 85.0f, m_Pos.y + 34.0f), Vec2(0.15f, 0.15f), L"Ability_TX");
		m_ui[1] = GetStage()->AddGameObject<UIWord>(Vec2(m_Pos.x - 85.0f, m_Pos.y -34.0f), Vec2(0.15f, 0.15f), L"Weight_TX");
		m_ui[2] = GetStage()->AddGameObject<colon>(Vec2(m_Pos.x - 19.0f, m_Pos.y + 34.0f), Vec2(0.15f, 0.15f));
		m_ui[3] = GetStage()->AddGameObject<colon>(Vec2(m_Pos.x - 19.0f, m_Pos.y - 34.0f), Vec2(0.15f, 0.15f));
		m_ui[4] = GetStage()->AddGameObject<UIType>(Vec2(m_Pos.x + 55.0f, m_Pos.y + 34.0f), Vec2(0.15f, 0.15f), player->GetThis<Player>()->GetMyShell().lock());
		m_ui[5] = GetStage()->AddGameObject<UIWeight>(Vec2(m_Pos.x + 55.0f, m_Pos.y - 34.0f), Vec2(0.15f, 0.15f), player->GetThis<Player>()->GetMyShell().lock());
	}
	void hukidasi::OnUpdate()
	{
		auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (camera->GetStartFlag())
		{
			for (int i = 0; i < 6; i++)
			{
				m_ui[i].lock()->SetDrawActive(false);
			}

			SetDrawActive(false);
		}
		else
		{
			for (int i = 0; i < 6; i++)
			{
				m_ui[i].lock()->SetDrawActive(true);
			}

			SetDrawActive(true);
		}

		if (camera->GetGoalFlag())
		{
			for (int i = 0; i < 6; i++)
			{
				m_ui[i].lock()->SetDrawActive(false);
			}
			SetDrawActive(false);
		}

	}


	colon::colon(const shared_ptr<Stage>& StagePtr, const Vec2 & Startpos, const Vec2 & StartScale):
		GameObject(StagePtr), m_Pos(Startpos), m_Scale(StartScale)
	{}
	void colon::OnCreate()
	{
		float HelfSize = 0.5f;
		// 頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(354.0f * m_Scale.x, 354.0f * m_Scale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_Pos.x, m_Pos.y, 0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(L"Colon_TX");

		SetDrawLayer(2);
	}
	void colon::OnUpdate()
	{
	}

	
	SelectStageDisplay::SelectStageDisplay(const shared_ptr<Stage>& StagePtr, const Vec2 & StartPos, const Vec2 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{
		m_StageNum = 0;
	}

	void SelectStageDisplay::OnCreate()
	{
		float HelfSize = 0.5f;
		//頂点配列
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize,+HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f,0.0f)) },
			{ VertexPositionColorTexture(Vec3(+HelfSize,+HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f,0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize,-HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f,1.0f)) },
			{ VertexPositionColorTexture(Vec3(+HelfSize,-HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f,1.0f)) },
		};

		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_Pos.x, m_Pos.y, 0.0f);
		PtrTransform->SetScale(1280.0f * m_Scale.x, 800.0f * m_Scale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		//頂点とインデックスを指定してスプライト作成
		auto ptrDraw = AddComponent<PCTSpriteDraw>(m_BackupVertices, indices);
		ptrDraw->SetTextureResource(L"VOLGANON_TX");
		SetAlphaActive(true);
		SetDrawLayer(0);

		GetStage()->SetSharedGameObject(L"SelectStageDisplay", GetThis<SelectStageDisplay>());
	}

	void SelectStageDisplay::OnUpdate()
	{
		StageTextureChange();
	}

	void SelectStageDisplay::StageTextureChange()
	{
		auto ptrDraw = GetComponent<PCTSpriteDraw>();
		switch (m_StageNum)
		{
		case 0:
			ptrDraw->SetTextureResource(L"STAGE1_TX");
			break;
		case 1:
			ptrDraw->SetTextureResource(L"STAGE2_TX");
			break;
		case 2:
			ptrDraw->SetTextureResource(L"STAGE3_TX");
			break;
		case 3:
			ptrDraw->SetTextureResource(L"STAGE4_TX");
			break;
		case 4:
			ptrDraw->SetTextureResource(L"STAGE5_TX");
			break;
		case 5:
			ptrDraw->SetTextureResource(L"STAGE6_TX");
			break;
		case 6:
			ptrDraw->SetTextureResource(L"STAGE7_TX");
			break;
		case 7:
			ptrDraw->SetTextureResource(L"STAGE8_TX");
			break;
		case 8:
			ptrDraw->SetTextureResource(L"STAGE9_TX");
			break;
		case 9:
			ptrDraw->SetTextureResource(L"STAGE10_TX");
			break;
		default:
			break;
		}
	}

	//-------------------------------------------------------------------------
	//	class Pause : public GameObject
	//	用途 : ポーズ
	//-------------------------------------------------------------------------
	Pause::Pause(const shared_ptr<Stage> StagePtr, const Vec3 & StartPos, const Vec2 & StartScale) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale)
	{
		m_OnPause = false;
		m_CursorNum = 1;
	}

	void Pause::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_Pos);
		ptrTrans->SetScale(m_Scale.x, m_Scale.y, 1.0f);

		m_sprites.push_back(GetStage()->AddGameObject<P_Sprite>(Vec3(0), Vec2(2.0f, 2.5f), L"P_BG_TX"));
		m_sprites[0].lock()->SetDrawLayer(1);

		m_sprites.push_back(GetStage()->AddGameObject<P_Sprite>(Vec3(0.0f, 50.0f, 0.0f), Vec2(1.5f, 0.7f), L"P_BACK_TX"));
		m_sprites[1].lock()->SetDrawLayer(2);
		m_sprites[1].lock()->GetComponent<PCTSpriteDraw>()->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 1.0f));

		m_sprites.push_back(GetStage()->AddGameObject<P_Sprite>(Vec3(0.0f, -50.0f, 0.0f), Vec2(1.5f, 0.7f), L"P_SELECT_TX"));
		m_sprites[2].lock()->SetDrawLayer(2);
		m_sprites[2].lock()->GetComponent<PCTSpriteDraw>()->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 1.0f));

		m_sprites.push_back(GetStage()->AddGameObject<P_Sprite>(Vec3(0.0f, -150.0f, 0.0f), Vec2(1.5f, 0.8f), L"P_TITLE_TX"));
		m_sprites[3].lock()->SetDrawLayer(2);
		m_sprites[3].lock()->GetComponent<PCTSpriteDraw>()->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 1.0f));

		m_sprites.push_back(GetStage()->AddGameObject<P_Sprite>(Vec3(0.0f, 200.0f, 0.0f), Vec2(1.5f, 0.7f), L"P_NAME_TX"));
		m_sprites[4].lock()->SetDrawLayer(2);
		m_sprites[4].lock()->GetComponent<PCTSpriteDraw>()->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 1.0f));

		m_sprites.push_back(GetStage()->AddGameObject<P_Sprite>(Vec3(0), Vec2(10.0f), L"BLACK_TX"));
		m_sprites[5].lock()->SetDrawLayer(0);
		m_sprites[5].lock()->GetComponent<PCTSpriteDraw>()->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 0.6f));

		for (int i = 0; i < static_cast<int>(m_sprites.size()); i++)
		{
			m_sprites[i].lock()->SetDrawActive(false);
		}

		GetStage()->SetSharedGameObject(L"Pause", GetThis<Pause>());
	}
	
	void Pause::OnUpdate()
	{
		PushStart();
	}

	void Pause::PushStart()
	{
		auto CntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntVec[0].bConnected)
		{
			if (CntVec[0].wPressedButtons & XINPUT_GAMEPAD_START)
			{
				auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
				if (!camera->GetStartFlag() && !camera->GetGoalFlag())
				{
					m_OnPause = !m_OnPause;
					StopUpdate();
				}
			}
		}

		ViewDraw();
	}

	void Pause::ViewDraw()
	{
		auto camera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (GetPause())
		{
			camera->SetUpDateActive(false);
			for (int i = 0; i < static_cast<int>(m_sprites.size()); i++)
			{
				m_sprites[i].lock()->SetDrawActive(true);
			}

			CursorSwich();
			Choose();
		}
		else
		{
			camera->SetUpDateActive(true);
			for (int i = 0; i < static_cast<int>(m_sprites.size()); i++)
			{
				m_sprites[i].lock()->SetDrawActive(false);
			}
		}
	}

	void Pause::Choose()
	{
		auto cntVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (cntVec[0].bConnected)
		{
			if (cntVec[0].fThumbLY == 1.0f)
			{
				m_CursorNum -= 1;
			}
			else if (cntVec[0].fThumbLY == -1.0f)
			{
				m_CursorNum += 1;
			}
			else if (cntVec[0].wPressedButtons & XINPUT_GAMEPAD_DPAD_UP)
			{
				m_CursorNum -= 1;
			}
			else if (cntVec[0].wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			{
				m_CursorNum += 1;
			}


			if (m_CursorNum > 3)
			{
				m_CursorNum = 1;
			}
			else if (m_CursorNum < 1)
			{
				m_CursorNum = 3;
			}

			PushChoose(cntVec);
		}
	}

	void Pause::CursorSwich()
	{
		float mag = 1.2f;
		float res = 256.0f;
		switch (m_CursorNum)
		{
		case 1:
			m_sprites[1].lock()->GetComponent<Transform>()->SetScale(Vec3(res * 1.5f * mag, res * 0.7f * mag, 1.0f));
			m_sprites[2].lock()->GetComponent<Transform>()->SetScale(Vec3(res *1.5f, res *0.7f, 1.0f));
			m_sprites[3].lock()->GetComponent<Transform>()->SetScale(Vec3(res *1.5f, res *0.8f, 1.0f));
			break;
		case 2:
			m_sprites[1].lock()->GetComponent<Transform>()->SetScale(Vec3(res *1.5f, res *0.7f, 1.0f));
			m_sprites[2].lock()->GetComponent<Transform>()->SetScale(Vec3(res *1.5f * mag, res *0.7f * mag, 1.0f));
			m_sprites[3].lock()->GetComponent<Transform>()->SetScale(Vec3(res *1.5f, res *0.8f, 1.0f));
			break;
		case 3:
			m_sprites[1].lock()->GetComponent<Transform>()->SetScale(Vec3(res *1.5f, res *0.7f, 1.0f));
			m_sprites[2].lock()->GetComponent<Transform>()->SetScale(Vec3(res *1.5f, res *0.7f, 1.0f));
			m_sprites[3].lock()->GetComponent<Transform>()->SetScale(Vec3(res *1.5f * mag, res *0.8f * mag, 1.0f));
			break;
		default:
			break;
		}
	}

	void Pause::PushChoose(vector<CONTROLER_STATE> cntVec)
	{
		auto scene = App::GetApp()->GetScene<Scene>();
		if (cntVec[0].wPressedButtons & XINPUT_GAMEPAD_A)
		{
			switch (m_CursorNum)
			{
			case 1:
				m_OnPause = false;
				break;
			case 2:
				m_OnPause = false;
				scene->SetStageName(L"StageSelect");
				scene->SetCheckPoint(false);
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"ToStageSelect");
				break;
			case 3:
				m_OnPause = false;
				scene->SetStageName(L"Title");
				scene->SetStageNum(0);
				scene->SetCheckPoint(false);
				PostEvent(0.0f, GetThis<ObjectInterface>(), scene, L"ToTitleStage");
				break;
			default:
				break;
			}
			StopUpdate();
		}
	}

	void Pause::StopUpdate()
	{
		if (m_OnPause)
		{
			auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
			for (auto& v : group->GetGroupVector())
			{
				v.lock()->SetUpdateActive(false);
			}
		}
		else
		{
			auto group = GetStage()->GetSharedObjectGroup(L"AllObject");
			for (auto& v : group->GetGroupVector())
			{
				v.lock()->SetUpdateActive(true);
			}
		}
	}

	//-------------------------------------------------------------------------
	//	class P_Sprite : public GameObject
	//	用途 : ポーズ用テクスチャ
	//-------------------------------------------------------------------------
	P_Sprite::P_Sprite(const shared_ptr<Stage> StagePtr, const Vec3 & StartPos, const Vec2 & StartScale, wstring TexturePtr) :
		GameObject(StagePtr), m_Pos(StartPos), m_Scale(StartScale), m_Texture(TexturePtr)
	{}

	void P_Sprite::OnCreate()
	{
		float HelfSize = 0.5f;
		// 頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(256.0f * m_Scale.x, 256.0f * m_Scale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_Pos.x, m_Pos.y, 0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_Texture);
	}

	void P_Sprite::OnUpdate()
	{

	}
}