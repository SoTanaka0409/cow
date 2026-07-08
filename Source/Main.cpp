#include "ServiceLocator.h"
#include "DxLib.h"
#include"Master.h"
#include"Camera.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"ResourceManager.h"
#include"SoundManager.h"
#include"ColliderManager.h"
#include "Utility.h"
#include"Score.h"
#include"EffectManager.h"
#include"Fever.h"
#include"InputManager.h"
#include <EffekseerForDXLib.h>

SceneManager* Master::mpSceneManager = new SceneManager();
Camera* Master::mpCamera = new Camera();
DebugCamera* Master::mpDebugCamera = new DebugCamera();
bool Master::mbIsDebugCamera = false;
ResourceManager* Master::mpResourceManager = new ResourceManager();
SoundManager* Master::mpSoundManager = new SoundManager();

EffectManager* Master::mpEffectManager = new EffectManager();

Score* Master::mpScore = nullptr;
bool Master::SelectSkill = false;
int Master::mnTutorialcount = 0;
bool Master::GameFinishFlag = false;
int Master::mnCaughtCowCount = 0;
bool Master::tutorial_vacum_flag_ = false;
bool Master::FeverFlag = false;
float Master::mfDeltaTime = 0.01666f;

VECTOR Utility::StageSize= VGet(6000, 0, 6000); // 3D遨ｺ髢薙・蠅・阜蛻ｶ邏・→縺励※繧ｹ繝・・繧ｸ繧ｵ繧､繧ｺ繧貞ｮ夂ｾｩ

/*
 * @brief 繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繧ｨ繝ｳ繝医Μ繝ｼ繝昴う繝ｳ繝・
 * [蜈･蜉嫋 hInstance, hPrevInstance, lpCmdLine, nCmdShow
 * [蜃ｺ蜉嫋 邨ゆｺ・さ繝ｼ繝・(豁｣蟶ｸ邨ゆｺ・凾縺ｯ0, 繧ｨ繝ｩ繝ｼ譎ゅ・-1)
 * [蜑ｯ菴懃畑] 繧ｲ繝ｼ繝繧ｦ繧｣繝ｳ繝峨え縺ｮ襍ｷ蜍輔√す繧ｹ繝・Β蛻晄悄蛹悶√Γ繧､繝ｳ繝ｫ繝ｼ繝怜ｮ溯｡後∝・繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ隗｣謾ｾ
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 繝・ヰ繝・げ繧・ｻ紋ｽ懈･ｭ縺ｮ荳ｦ陦後ｒ螳ｹ譏薙↓縺吶ｋ縺溘ａ繧ｦ繧｣繝ｳ繝峨え繝｢繝ｼ繝峨〒襍ｷ蜍・
	ChangeWindowMode(true);

	SetGraphMode(Utility::kScreenWidth, Utility::kScreenHeight, 32, 60);
	SetWindowSize(Utility::kScreenWidth, Utility::kScreenHeight);
	
	// DxLib縺ｮ蛻晄悄蛹悶お繝ｩ繝ｼ譎ゅ・螳溯｡檎ｶ咏ｶ壻ｸ榊庄縺ｮ縺溘ａ蜊ｳ譎らｵゆｺ・
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	Master::mpScore = new Score(); // DxLib蛻晄悄蛹門燕縺縺ｨ逕ｻ蜒剰ｪｭ縺ｿ霎ｼ縺ｿ縺悟､ｱ謨励☆繧句宛邏・′縺ゅｋ縺溘ａ縺薙％縺ｧ逕滓・

	SRand(GetNowCount());

	SetGlobalAmbientLight(GetColorF(0.4f, 0.4f, 0.4f, 1.0f));
	
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	Master::mpSoundManager->Initialize(); // 蜀咲函驕・ｻｶ繧帝亟縺舌◆繧∝・髻ｳ貅舌ョ繝ｼ繧ｿ繧偵・繝ｪ繝ｭ繝ｼ繝峨☆繧・

	// 蛻晄悄繧ｷ繝ｼ繝ｳ繧呈ｧ狗ｯ峨☆繧・
	Master::mpSceneManager->Initialize();
	
	// 謠冗判逕ｨ縺ｮ蜷・き繝｡繝ｩ繧貞・譛溷喧縺吶ｋ
	Master::mpCamera->Initialize();
	Master::mpDebugCamera->Initialize();

	Master::mpEffectManager->Initalize();

	int previousTime = GetNowCount();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 蜑阪ヵ繝ｬ繝ｼ繝縺ｮ謠冗判繧偵け繝ｪ繧｢縺励※譁ｰ隕乗緒逕ｻ縺ｮ貅門ｙ
		ClearDrawScreen();
		int time = GetNowCount();

		Master::mfDeltaTime = (time - previousTime) / 1000.0f;
		if (Master::mfDeltaTime > 0.1f) Master::mfDeltaTime = 0.1f;
		previousTime = time;

		// 髢狗匱蜉ｹ邇・喧縺ｮ縺溘ａF1繧ｭ繝ｼ縺ｧ繝・ヰ繝・げ繧ｫ繝｡繝ｩ繧偵ヨ繧ｰ繝ｫ
		if (InputManager::CheckDownKey(KEY_INPUT_F1))
		{
			Master::mbIsDebugCamera = !Master::mbIsDebugCamera;
			if (Master::mbIsDebugCamera) {
				Master::mpDebugCamera->Initialize();
			}
		}

		if (Master::mbIsDebugCamera) {
			Master::mpDebugCamera->Update();
		} else {
			Master::mpCamera->Update();
		}

		Master::mpEffectManager->Update();

		Master::mpSceneManager->Update();
		
		Master::mpSceneManager->Draw();
		
		Master::mpEffectManager->Draw();

		// 謠冗判縺ｮ縺｡繧峨▽縺阪ｒ髦ｲ縺舌◆繧√ヵ繝ｪ繝・・縺吶ｋ
		ScreenFlip();

		// 蝗ｺ螳壹ヵ繝ｬ繝ｼ繝繝ｬ繝ｼ繝・60FPS)邯ｭ謖√・縺溘ａ蠕・ｩ・
		while (GetNowCount() - time < 17)
		{
		}

		// 繝｡繝｢繝ｪ繝ｪ繝ｼ繧ｯ繧帝亟縺舌◆繧∫ｴ譽・ｦ∵ｱゅ・縺ゅｋ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ隗｣謾ｾ
		if (auto scene = ServiceLocator::GetCurrentScene())
		{
			scene->GetCollisionManager()->DeleteAllColliderIfNeeded();
		}
		if (auto objMgr = ServiceLocator::GetObjectManager())
		{
			objMgr->DeleteAll3DIfNeeded();
		}

		// 繝輔Ξ繝ｼ繝邨ゆｺ・凾縺ｫ螳牙・縺ｫ繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ繧定｡後≧
		Master::mpSceneManager->ChangeSceneIfNeeded();
	}
	// 繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ邨ゆｺ・↓莨ｴ縺・Μ繧ｽ繝ｼ繧ｹ隗｣謾ｾ
	Master::mpSceneManager->Finalize();
	delete Master::mpSceneManager;
	Master::mpSoundManager->Finalize();
	delete Master::mpSoundManager;
	Master::mpCamera->Finalize();
	delete Master::mpCamera;
	delete Master::mpDebugCamera;
	delete Master::mpResourceManager;

	ColliderManager::GetInstance()->Finalize();
	
	Effkseer_End();

	delete Master::mpScore;

	DxLib_End(); // DxLib縺ｮ蜀・Κ繝ｪ繧ｽ繝ｼ繧ｹ繧定ｧ｣謾ｾ

	return 0;
}
