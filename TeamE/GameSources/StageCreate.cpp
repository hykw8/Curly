#include "stdafx.h"
#include "Project.h"
namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//	class UnityCreate
	//	用途: UnityのTransform情報を.csvからとって来て配置する処理
	//--------------------------------------------------------------------------------------
	UnityCreate::UnityCreate(const shared_ptr<Stage>& StagePtr, const wstring FileName):
		m_Stage(StagePtr), m_FileName(FileName)
	{}

	void UnityCreate::OnCreate()
	{
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);
		dataDir += L"Csv\\";
		CsvFile stageCsv(dataDir + m_FileName);
		if (!stageCsv.ReadCsv())
		{
			throw BaseException
			(
				L"if (!StageCsv.ReadCsv())",
				L"Csvファイルの取得できないですよ",
				L"見直して"
			);
		}

		const int rowNum = stageCsv.GetRowCount();

		auto stage = m_Stage.lock();
		stage->CreateSharedObjectGroup(L"GoalItemGroup");
		stage->CreateSharedObjectGroup(L"ShellGroup");
		stage->CreateSharedObjectGroup(L"DoorRed");
		stage->CreateSharedObjectGroup(L"DoorBule");
		stage->CreateSharedObjectGroup(L"DoorYellow");
		stage->CreateSharedObjectGroup(L"AllObject");

		auto SangoSparkPtr = stage->AddGameObject<SangoSpark>();
		//共有オブジェクトにスパークを登録
		stage->SetSharedGameObject(L"SangoSpark", SangoSparkPtr);


		for (int i = 1; i < rowNum; i++)
		{
			stageCsv.GetRowVec(i, RowStr);

			auto scene = App::GetApp()->GetScene<Scene>();

			//床
			if (RowStr[0].find(L"Cube") != -1)
			{
				stage->AddGameObject<Floor>(GetT(Tf::Pos), GetT(Tf::Scale), GetT(Tf::Rot));
			}
			//プレイヤー
			else if (RowStr[0].find(L"Player") != -1)
			{
				auto player = stage->AddGameObject<Player>(GetT(Tf::Scale), GetT(Tf::Rot), GetT(Tf::Pos));
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(player);
			}
			//サンゴ
			else if (RowStr[0].find(L"Sango") != -1)
			{
				auto sango = stage->AddGameObject<GoalItem>(GetT(Tf::Pos), GetT(Tf::Scale) * 1.5f);
				//auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				//groupObj->IntoGroup(sango);
			}
			//殻
			else if (RowStr[0].find(L"Temp") != -1)
			{
				auto temp = stage->AddGameObject<TempShell>(GetT(Tf::Pos), GetT(Tf::Scale));
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(temp);
			}
			else if (RowStr[0].find(L"Dash") != -1)
			{
				auto dash = stage->AddGameObject<DashShell>(GetT(Tf::Pos), GetT(Tf::Scale));
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(dash);
			}
			else if (RowStr[0].find(L"Spin") != -1)
			{
				auto spin = stage->AddGameObject<SpinShell>(GetT(Tf::Pos), GetT(Tf::Scale));
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(spin);
			}
			else if (RowStr[0].find(L"Iron") != -1)
			{
				auto iron = stage->AddGameObject<IronShell>(GetT(Tf::Pos), GetT(Tf::Scale));
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(iron);
			}
			else if (RowStr[0].find(L"Ballon") != -1)
			{
				auto ballon = stage->AddGameObject<BallonShell>(GetT(Tf::Pos), 1.0f);
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(ballon);
			}
			//死ぬやつ
			else if (RowStr[0].find(L"DeadZone") != -1)
			{
				stage->AddGameObject<DeadZone>(GetT(Tf::Pos), GetT(Tf::Scale));

				if (GetT(Tf::Scale).x > GetT(Tf::Scale).z)
				{
					scene->SetStageSize(GetT(Tf::Scale).x);
				}
				else
				{
					scene->SetStageSize(GetT(Tf::Scale).z);
				}

				scene->m_StageCenter = GetT(Tf::Pos);
			}
			//ギミック
			else if (RowStr[0].find(L"NewMoveGround") != -1)
			{
				auto newMove = stage->AddGameObject<MoveGround>(GetT(Tf::Pos), GetT(Tf::Scale), Vec3(stof(RowStr[10]), stof(RowStr[11]), stof(RowStr[12])), stof(RowStr[13]));
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(newMove);
			}
			else if (RowStr[0].find(L"MoveGroundX") != -1)
			{
				auto moveX = stage->AddGameObject<MoveGround>(GetT(Tf::Pos), GetT(Tf::Scale), Vec3(2.0f, 0.0f, 0.0f), 4.0f);
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(moveX);
			}
			else if (RowStr[0].find(L"MoveGroundY") != -1)
			{
				auto moveY = stage->AddGameObject<MoveGround>(GetT(Tf::Pos), GetT(Tf::Scale), Vec3(0.0f, 2.0f, 0.0f), 4.0f);
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(moveY);
			}
			else if (RowStr[0].find(L"MoveGroundZ") != -1)
			{
				auto moveZ = stage->AddGameObject<MoveGround>(GetT(Tf::Pos), GetT(Tf::Scale), Vec3(0.0f, 0.0f, 2.0f), 4.0f);
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(moveZ);
			}
			else if (RowStr[0].find(L"DoorRed") != -1)
			{
				auto red = stage->AddGameObject<Door>(GetT(Tf::Pos), GetT(Tf::Scale), GetT(Tf::Rot), Col4(1.0f, 0.0f, 0.0f, 1.0f));
				auto group = stage->GetSharedObjectGroup(L"DoorRed");
				group->IntoGroup(red);
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(red);
			}
			else if (RowStr[0].find(L"DoorBlue") != -1)
			{
				auto blue = stage->AddGameObject<Door>(GetT(Tf::Pos), GetT(Tf::Scale), GetT(Tf::Rot), Col4(0.0f, 0.0f, 1.0f, 1.0f));
				auto group = stage->GetSharedObjectGroup(L"DoorBule");
				group->IntoGroup(blue);
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(blue);
			}
			else if (RowStr[0].find(L"DoorYellow") != -1)
			{
				auto yellow = stage->AddGameObject<Door>(GetT(Tf::Pos), GetT(Tf::Scale), GetT(Tf::Rot), Col4(1.0f, 1.0f, 0.0f, 1.0f));
				auto group = stage->GetSharedObjectGroup(L"DoorYellow");
				group->IntoGroup(yellow);
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(yellow);
			}
			else if (RowStr[0].find(L"SwitchRed") != -1)
			{
				stage->AddGameObject<Switch>(GetT(Tf::Pos), GetT(Tf::Scale), Col4(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else if (RowStr[0].find(L"SwitchBlue") != -1)
			{
				stage->AddGameObject<Switch>(GetT(Tf::Pos), GetT(Tf::Scale), Col4(0.0f, 0.0f, 1.0f, 1.0f));
			}
			else if (RowStr[0].find(L"SwitchYellow") != -1)
			{
				stage->AddGameObject<Switch>(GetT(Tf::Pos), GetT(Tf::Scale), Col4(1.0f, 1.0f, 0.0f, 1.0f));
			}
			else if (RowStr[0].find(L"BreakWall") != -1)
			{
				stage->AddGameObject<BreakWall>(GetT(Tf::Pos), GetT(Tf::Scale));
			}
			//壁系
			else if (RowStr[0].find(L"InvisibleWall") != -1)
			{
				stage->AddGameObject<ClearWall>(GetT(Tf::Pos), GetT(Tf::Scale));

				auto scene = App::GetApp()->GetScene<Scene>();
				if (GetT(Tf::Pos).y > scene->GetStageHeight())
				{
					scene->SetStageHeight(GetT(Tf::Pos).y);
				}
			}
			else if (RowStr[0].find(L"Wall") != -1)
			{
				stage->AddGameObject<Wall>(GetT(Tf::Pos), GetT(Tf::Scale));
			}
			else if (RowStr[0].find(L"Slope") != -1)
			{
				stage->AddGameObject<Slope>(GetT(Tf::Pos), GetT(Tf::Scale), GetT(Tf::Rot));
			}
			//敵
			else if (RowStr[0].find(L"NewHermitCrab") != -1)
			{
				auto newCrab = stage->AddGameObject<NewHermitCrab>(GetT(Tf::Pos), GetT(Tf::Scale), Vec3(0.0f, 0.0f, 0.0f));
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(newCrab);
			}
			else if (RowStr[0].find(L"HermitCrab") != -1)
			{
				auto crab = stage->AddGameObject<ComeEnemy>(GetT(Tf::Pos), GetT(Tf::Scale), Vec3(0.0f, 0.0f, 0.0f));
				auto groupObj = stage->GetSharedObjectGroup(L"AllObject");
				groupObj->IntoGroup(crab);
			}
			//チェックポイント
			else if (RowStr[0].find(L"CheckPoint") != -1)
			{
				stage->AddGameObject<CheckPoint>(GetT(Tf::Pos), GetT(Tf::Scale));
			}

			if (scene->GetStageHeight() < GetT(Tf::Pos).y)
			{
				scene->SetStageHeight(GetT(Tf::Pos).y);
			}
		}
	}

	Vec3 UnityCreate::GetT(const Tf & t)
	{
		Vec3 v = Vec3(0, 0, 0);
		switch (t)
		{
		case Tf::Pos:
		{
			v = Vec3(stof(RowStr[1]), stof(RowStr[2]), stof(RowStr[3]));
			break;
		}
		case Tf::Rot:
		{
			auto rx = stof(RowStr[4]) * (XM_PI / 180.0f);
			auto ry = stof(RowStr[5]) * (XM_PI / 180.0f);
			auto rz = stof(RowStr[6]) * (XM_PI / 180.0f);

			v = Vec3(rx, ry, rz);
			break;
		}
		case Tf::Scale:
		{
			v = Vec3(stof(RowStr[7]), stof(RowStr[8]), stof(RowStr[9]));
			break;
		}
		default:
			break;
		}
		return v;
	}
	//--------------------------------------------------------------------------------------------------------
}

