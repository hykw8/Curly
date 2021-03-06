#include "stdafx.h"
#include "Project.h"
#include "LicenseStage.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------------------------
	//	担当範囲
	//--------------------------------------------------------------------------------------------------------

	void LicenseStage::OnCreate()
	{
		try
		{

			auto PtrView = CreateView<SingleView>();
			//ビューのカメラの設定
			auto PtrCamera = ObjectFactory::Create<Camera>();
			PtrView->SetCamera(PtrCamera);
			PtrCamera->SetEye(Vec3(0.0f, 2.0f, -3.0f));
			PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

			auto ptrSingleLight = CreateLight<SingleLight>();
			ptrSingleLight->GetLight().SetPositionToDirectional(0.0f, 1.0f, -0.25f);

			AddGameObject<TitleSprite>
				(
					L"LICENSE_TX", false,
				Vec2(1280.0f, 800.0f), Vec2(0.0f, 0.0f)
				);

			auto scene = App::GetApp()->GetScene<Scene>();
			PostEvent(2.0f, GetThis<ObjectInterface>(), scene, L"ToTitleStage");
		}
		catch (...)
		{
			throw;
		}
	}
}