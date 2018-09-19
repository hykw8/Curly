
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::CreateResouce()
	{
		wstring datadir;
		auto& app = App::GetApp();

		//テクスチャ
		app->GetDataDirectory(datadir);
		datadir += L"Texture\\";
		wstring strTexture = datadir + L"TimeNumber.png";
		app->RegisterTexture(L"TIMER_TX", strTexture);

		strTexture = datadir + L"StageSelect_3.jpg";
		app->RegisterTexture(L"StageSelectBG_TX", strTexture);


		strTexture = datadir + L"Sango.png";
		app->RegisterTexture(L"SANGO_TX", strTexture);


		strTexture = datadir + L"PressAnyButton.png";
		app->RegisterTexture(L"PRESSANYBUTTON_TX", strTexture);

		strTexture = datadir + L"Metal_shell.png";
		app->RegisterTexture(L"METAL_SHELL_TX", strTexture);

		strTexture = datadir + L"Speech balloon2.png";
		app->RegisterTexture(L"Speech balloon2_TX", strTexture);

		strTexture = datadir + L"Speech balloon3.png";
		app->RegisterTexture(L"Speech_balloon3_TX", strTexture);

		strTexture = datadir + L"Volganon.png";
		app->RegisterTexture(L"VOLGANON_TX", strTexture);

		strTexture = datadir + L"MetalNetwork2.jpg";
		app->RegisterTexture(L"NETWORK_TX", strTexture);

		strTexture = datadir + L"Brake_Conkuri.png";
		app->RegisterTexture(L"BrakeWall_TX", strTexture);

		strTexture = datadir + L"Brake_Conkuri_housen.png";
		app->RegisterTexture(L"BrakeWallhousen_TX", strTexture);

		strTexture = datadir + L"Brake_Conkuri_Normal.png";
		app->RegisterTexture(L"BrakeWallNormal_TX", strTexture);

		strTexture = datadir + L"Conkuri.jpg";
		app->RegisterTexture(L"CONKURI_TX", strTexture);

		strTexture = datadir + L"Button_Red.png";
		app->RegisterTexture(L"BOTTUNRED_TX", strTexture);

		strTexture = datadir + L"Button_Blue .png";
		app->RegisterTexture(L"BOTTUN_BLUE_TX", strTexture);

		strTexture = datadir + L"Button_Yellow.png";
		app->RegisterTexture(L"BOTTUN_YELLOW_TX", strTexture);

		strTexture = datadir + L"Conkuri_Move.jpg";
		app->RegisterTexture(L"MOVEGROUND_TX", strTexture);

		strTexture = datadir + L"RestCorals2.png";
		app->RegisterTexture(L"RESTCORALS2_TX", strTexture);

		strTexture = datadir + L"batu.png";
		app->RegisterTexture(L"BATU_TX", strTexture);

		strTexture = datadir + L"Title4.png";
		app->RegisterTexture(L"TitleLogo_TX", strTexture);

		strTexture = datadir + L"TitleSprite3.jpg";
		app->RegisterTexture(L"TitleSprite_TX", strTexture);

		strTexture = datadir + L"Kousi.png";
		app->RegisterTexture(L"KOUSI_TX", strTexture);

		strTexture = datadir + L"Cursor.png";
		app->RegisterTexture(L"CURSOR_TX", strTexture);

		strTexture = datadir + L"Cursor_w.png";
		app->RegisterTexture(L"CURSOR_W_TX", strTexture);

		strTexture = datadir + L"balln.png";
		app->RegisterTexture(L"BALLN_TX", strTexture);

		strTexture = datadir + L"License.png";
		app->RegisterTexture(L"LICENSE_TX", strTexture);

		strTexture = datadir + L"SatgeSelectLogo.png";
		app->RegisterTexture(L"SELELOGO_TX", strTexture);

		strTexture = datadir + L"Star.png";
		app->RegisterTexture(L"STAR_TX", strTexture);

		strTexture = datadir + L"wave.png";
		app->RegisterTexture(L"WAVE_TX", strTexture);

		strTexture = datadir + L"starsky.jpg";
		app->RegisterTexture(L"STARSKY_TX", strTexture);

		strTexture = datadir + L"Wall3.png";
		app->RegisterTexture(L"BlueWall_TX", strTexture);

		strTexture = datadir + L"black.png";
		app->RegisterTexture(L"BLACK_TX", strTexture);

		strTexture = datadir + L"pauseselect.png";
		app->RegisterTexture(L"P_SELECT_TX", strTexture);

		strTexture = datadir + L"pausetitle.png";
		app->RegisterTexture(L"P_TITLE_TX", strTexture);

		strTexture = datadir + L"pauseback.png";
		app->RegisterTexture(L"P_BACK_TX", strTexture);

		strTexture = datadir + L"pauseBG.png";
		app->RegisterTexture(L"P_BG_TX", strTexture);

		strTexture = datadir + L"pauseName.png";
		app->RegisterTexture(L"P_NAME_TX", strTexture);



		// StageImage
		strTexture = datadir + L"Stage1.jpg";
		app->RegisterTexture(L"STAGE1_TX", strTexture);

		strTexture = datadir + L"Stage2.jpg";
		app->RegisterTexture(L"STAGE2_TX", strTexture);

		strTexture = datadir + L"Stage3.jpg";
		app->RegisterTexture(L"STAGE3_TX", strTexture);

		strTexture = datadir + L"Stage4.jpg";
		app->RegisterTexture(L"STAGE4_TX", strTexture);

		strTexture = datadir + L"Stage5.jpg";
		app->RegisterTexture(L"STAGE5_TX", strTexture);

		strTexture = datadir + L"Stage6.jpg";
		app->RegisterTexture(L"STAGE6_TX", strTexture);

		strTexture = datadir + L"Stage7.jpg";
		app->RegisterTexture(L"STAGE7_TX", strTexture);

		strTexture = datadir + L"Stage8.jpg";
		app->RegisterTexture(L"STAGE8_TX", strTexture);

		strTexture = datadir + L"Stage9.jpg";
		app->RegisterTexture(L"STAGE9_TX", strTexture);

		strTexture = datadir + L"Stage10.jpg";
		app->RegisterTexture(L"STAGE10_TX", strTexture);

		strTexture = datadir + L"X_bottun.png";
		app->RegisterTexture(L"XBUTTON_TX", strTexture);

		app->GetDataDirectory(datadir);
		datadir += L"Texture\\Word\\";
		strTexture = datadir + L"ability.png";
		app->RegisterTexture(L"Ability_TX", strTexture);

		strTexture = datadir + L"ability_2.png";
		app->RegisterTexture(L"Ability2_TX", strTexture);

		strTexture = datadir + L"colon.png";
		app->RegisterTexture(L"Colon_TX", strTexture);

		strTexture = datadir + L"colon_2.png";
		app->RegisterTexture(L"Colon2_TX", strTexture);

		strTexture = datadir + L"Dash.png";
		app->RegisterTexture(L"Dash_TX", strTexture);

		strTexture = datadir + L"Dash_2.png";
		app->RegisterTexture(L"Dash2_TX", strTexture);

		strTexture = datadir + L"hovering.png";
		app->RegisterTexture(L"Hovering_TX", strTexture);

		strTexture = datadir + L"hovering_2.png";
		app->RegisterTexture(L"Hovering2_TX", strTexture);

		strTexture = datadir + L"maueagaru.png";
		app->RegisterTexture(L"Maueagaru_TX", strTexture);

		strTexture = datadir + L"maueagaru_2.png";
		app->RegisterTexture(L"Maueagaru2_TX", strTexture);

		strTexture = datadir + L"mauehujo.png";
		app->RegisterTexture(L"Mauehujo_TX", strTexture);

		strTexture = datadir + L"mauehujo_2.png";
		app->RegisterTexture(L"Mauehujo2_TX", strTexture);

		strTexture = datadir + L"maueuku.png";
		app->RegisterTexture(L"Maueuku_TX", strTexture);

		strTexture = datadir + L"maueuku_2.png";
		app->RegisterTexture(L"Maueuku2_TX", strTexture);

		strTexture = datadir + L"Next.png";
		app->RegisterTexture(L"Next_TX", strTexture);

		strTexture = datadir + L"Next_2.png";
		app->RegisterTexture(L"Next2_TX", strTexture);

		strTexture = datadir + L"Next_2_w.png";
		app->RegisterTexture(L"Next2W_TX", strTexture);

		strTexture = datadir + L"NextSatge.png";
		app->RegisterTexture(L"NextSatge_TX", strTexture);

		strTexture = datadir + L"NextSatge_2.png";
		app->RegisterTexture(L"NextSatge2_TX", strTexture);

		strTexture = datadir + L"NextSatge_2_w.png";
		app->RegisterTexture(L"NextSatge2W_TX", strTexture);

		strTexture = datadir + L"none.png";
		app->RegisterTexture(L"None_TX", strTexture);

		strTexture = datadir + L"none_2.png";
		app->RegisterTexture(L"None2_TX", strTexture);

		strTexture = datadir + L"period.png";
		app->RegisterTexture(L"Period_TX", strTexture);

		strTexture = datadir + L"period_2.png";
		app->RegisterTexture(L"Period2_TX", strTexture);

		strTexture = datadir + L"propeller.png";
		app->RegisterTexture(L"Propeller_TX", strTexture);

		strTexture = datadir + L"propeller_2.png";
		app->RegisterTexture(L"Propeller2_TX", strTexture);

		strTexture = datadir + L"PushBottom.png";
		app->RegisterTexture(L"PushBottom_TX", strTexture);

		strTexture = datadir + L"PushBottom_2_w.png";
		app->RegisterTexture(L"PushBottom2W_TX", strTexture);

		strTexture = datadir + L"PushBottom_2.png";
		app->RegisterTexture(L"PushBottom2_TX", strTexture);

		strTexture = datadir + L"StageSelect.png";
		app->RegisterTexture(L"StageSelect_TX", strTexture);

		strTexture = datadir + L"StageSelect_2.png";
		app->RegisterTexture(L"StageSelect2_TX", strTexture);

		strTexture = datadir + L"StageSelect_2_w.png";
		app->RegisterTexture(L"StageSelect2W_TX", strTexture);

		strTexture = datadir + L"sugoku.png";
		app->RegisterTexture(L"Sugoku_TX", strTexture);

		strTexture = datadir + L"sugoku_2.png";
		app->RegisterTexture(L"Sugoku2_TX", strTexture);

		strTexture = datadir + L"weight.png";
		app->RegisterTexture(L"Weight_TX", strTexture);

		strTexture = datadir + L"weight_2.png";
		app->RegisterTexture(L"Weight2_TX", strTexture);

		strTexture = datadir + L"weightheavy.png";
		app->RegisterTexture(L"Weightheavy_TX", strTexture);

		strTexture = datadir + L"weightheavy.png";
		app->RegisterTexture(L"Weightheavy_TX", strTexture);

		strTexture = datadir + L"weightheavy_2.png";
		app->RegisterTexture(L"Weightheavy2_TX", strTexture);

		strTexture = datadir + L"weightlight.png";
		app->RegisterTexture(L"Weightlight_TX", strTexture);

		strTexture = datadir + L"weightlight_2.png";
		app->RegisterTexture(L"Weightlight2_TX", strTexture);

		strTexture = datadir + L"weightnormal.png";
		app->RegisterTexture(L"Weightnormal_TX", strTexture);

		strTexture = datadir + L"weightnormal_2.png";
		app->RegisterTexture(L"Weightnormal2_TX", strTexture);


		// BGM
		app->GetDataDirectory(datadir);
		datadir += L"BGM\\";

		app->GetDataDirectory(datadir);
		datadir += L"SE\\";

		// システム
		wstring Sound = datadir + L"System.wav";
		App::GetApp()->RegisterWav(L"System", Sound);

		// セレクト
		wstring Select = datadir + L"Select.mp3";
		App::GetApp()->RegisterWav(L"Select", Sound);




		//モデル
		app->GetDataDirectory(datadir);
		datadir += L"Model\\";
		auto modelMesh = MeshResource::CreateStaticModelMesh(datadir, L"DashShell.bmf");
		app->RegisterResource(L"Dash", modelMesh);
		modelMesh = MeshResource::CreateStaticModelMesh(datadir, L"Stage01.bmf");
		app->RegisterResource(L"Stage01", modelMesh);
		modelMesh = MeshResource::CreateStaticModelMesh(datadir, L"Curly.bmf");
		app->RegisterResource(L"Curly", modelMesh);
		modelMesh = MeshResource::CreateStaticModelMesh(datadir, L"NomalShell.bmf");
		app->RegisterResource(L"Normal", modelMesh);
		modelMesh = MeshResource::CreateStaticModelMesh(datadir, L"SpinShell.bmf");
		app->RegisterResource(L"Spin", modelMesh);
		modelMesh = MeshResource::CreateStaticModelMesh(datadir, L"MetalShell2.bmf");
		app->RegisterResource(L"Metal", modelMesh);
		modelMesh = MeshResource::CreateStaticModelMesh(datadir, L"unko_mk-6.bmf");
		app->RegisterResource(L"Unko", modelMesh);


		//モデル
		app->GetDataDirectory(datadir);
		datadir += L"Model\\";
		auto medelTexture = datadir + L"Metal_shell.png";
		app->RegisterTexture(L"MetalTex_TX", medelTexture);


		app->GetDataDirectory(datadir);
		datadir += L"BGM\\";
		auto bgm = datadir + L"Title.wav";
		app->RegisterWav(L"Title", bgm);
		bgm = datadir + L"StageSelect.wav";
		app->RegisterWav(L"StageSelect", bgm);
		bgm = datadir + L"GameStage.wav";
		app->RegisterWav(L"GameStage", bgm);

		app->GetDataDirectory(datadir);
		datadir += L"SE\\";
		auto SE = datadir + L"Ballon.wav";
		app->RegisterWav(L"Ballon", SE);
		SE = datadir + L"Break.wav";
		app->RegisterWav(L"Break",SE);
		SE = datadir + L"Change.wav";
		app->RegisterWav(L"Change", SE);
		SE = datadir + L"Dash.wav";
		app->RegisterWav(L"Dash_1", SE);
		SE = datadir + L"Door.wav";
		app->RegisterWav(L"Door", SE);
		SE = datadir + L"Item.wav";
		app->RegisterWav(L"Item", SE);
		 SE = datadir + L"OK.wav";
		app->RegisterWav(L"OK", SE);
		SE = datadir + L"Select.wav";
		app->RegisterWav(L"Select", SE);
		SE = datadir + L"Spin.wav";
		app->RegisterWav(L"Spin_1", SE);
		SE = datadir + +L"Switch.wav";
		app->RegisterWav(L"Switch", SE);
		SE = datadir + L"System.wav";
		app->RegisterWav(L"System", SE);
	}

	void Scene::OnCreate() 
	{
		try 
		{
			CreateResouce();

			//BGMの再生
			m_AudioObjectPtr = ObjectFactory::Create<MultiAudioObject>();
			m_AudioObjectPtr->AddAudioResource(L"Title");
			m_AudioObjectPtr->AddAudioResource(L"StageSelect");
			m_AudioObjectPtr->AddAudioResource(L"GameStage");

			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			//PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToTitleStage");
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToLicenseStage");
		}
		catch (...) 
		{
			throw;
		}
	}
	void Scene::Stop()
	{
		m_AudioObjectPtr->Stop(L"Title");
		m_AudioObjectPtr->Stop(L"StageSelect");
		m_AudioObjectPtr->Stop(L"GameStage");

	}
	void Scene::OnEvent(const shared_ptr<Event>& event) 
	{
		if (event->m_MsgStr == L"ToLicenseStage")
		{
			ResetActiveStage<LicenseStage>();
		}
		if (event->m_MsgStr == L"ToTitleStage")
		{
			Stop();
			m_AudioObjectPtr->Start(L"Title", XAUDIO2_LOOP_INFINITE, 0.5f);
			ResetActiveStage<TitleStage>();
		}
		if (event->m_MsgStr == L"ToStageSelect")
		{
			Stop();
			m_AudioObjectPtr->Start(L"StageSelect", XAUDIO2_LOOP_INFINITE, 0.5f);
			ResetActiveStage<StageSelect>();
		}
		if (event->m_MsgStr == L"ToGameStage")
		{
			Stop();
			m_AudioObjectPtr->Start(L"GameStage", XAUDIO2_LOOP_INFINITE, 0.5f);
			ResetActiveStage<GameStage>();
		}
	}

}
//end basecross