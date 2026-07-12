#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "SelectionManager.h"

TitleScene::TitleScene()
	: Scene()
{
	cow_voice_timer_ = 180; // è›»æ™å±“èœ€å’²å‡½ç¸ºE¾ç¸ºE§ç¸ºE®è •ãEE©æº˜ãƒµç¹ï½¬ç¹ï½¼ç¹ï¿½éšªE­è³ãƒ»
	
	title_graph_handle_ = Master::mpResourceManager->LoadGraphics("Resource/2D/ã‚¿ã‚¤ãƒˆãƒ«ã‚·ãƒ¼ãƒ³.png");
	ufo_graph_handle_ = Master::mpResourceManager->LoadGraphics("Resource/2D/ç‰›ã¨UFO.png");
	GetGraphSize(ufo_graph_handle_, &ufo_w_, &ufo_h_);

	ufo_x_ = 540;
	ufo_y_ = 30;

	is_dragging_ufo_ = false;
	offset_x_ = 0;
	offset_y_ = 0;

	ufo_vx_ = 2.0f;
	ufo_vy_ = 1.5f;

	ufo_angle_ = 0.0f;
	is_auto_patrol_ = false;
	auto_patrol_timer_ = 0;

	TitleButton newGameBtn;
	newGameBtn.type = SelectionManager::Title::NewGame;
	newGameBtn.graph_handle = Master::mpResourceManager->LoadGraphics("Resource/2D/ã‚¹ã‚¿ãƒ¼ãƒˆãEã‚¿ãƒ³1.png");
	newGameBtn.x = 920;
	newGameBtn.y = 50;
	GetGraphSize(newGameBtn.graph_handle, &newGameBtn.w, &newGameBtn.h);
	newGameBtn.is_hover = false;
	buttons_.push_back(newGameBtn);

	TitleButton tutorialBtn;
	tutorialBtn.type = SelectionManager::Title::Tutorial;
	tutorialBtn.graph_handle = Master::mpResourceManager->LoadGraphics("Resource/2D/ãƒãƒ¥ãƒ¼ãƒˆãƒªã‚¢ãƒ«ã¼ãŸã‚“.png");
	tutorialBtn.x = 900;
	tutorialBtn.y = 250;
	GetGraphSize(tutorialBtn.graph_handle, &tutorialBtn.w, &tutorialBtn.h);
	tutorialBtn.is_hover = false;
	buttons_.push_back(tutorialBtn);

	TitleButton opeBtn;
	opeBtn.type = SelectionManager::Title::OperationProcedures;
	opeBtn.graph_handle = Master::mpResourceManager->LoadGraphics("Resource/2D/ã›ã£ã¦ãE¼ãŸã‚“1.png");
	opeBtn.x = 915;
	opeBtn.y = 450;
	GetGraphSize(opeBtn.graph_handle, &opeBtn.w, &opeBtn.h);
	opeBtn.is_hover = false;
	buttons_.push_back(opeBtn);

	TitleButton exitBtn;
	exitBtn.type = SelectionManager::Title::titleOUT;
	exitBtn.graph_handle = Master::mpResourceManager->LoadGraphics("Resource/2D/ã—ã‚…ãE‚Šã‚E†ã¼ãŸã‚“ (1).png");
	exitBtn.x = 960;
	exitBtn.y = 680;
	GetGraphSize(exitBtn.graph_handle, &exitBtn.w, &exitBtn.h);
	exitBtn.is_hover = false;
	buttons_.push_back(exitBtn);

	fade_state_ = kSceneFadeIn;
	SetFadeAlpha(255.0f);
	next_scene_ = SceneManager::kSceneTitle;

	is_hover_new_game_ = false;
	frame_count_ = 0;

	// ç¹ï½©ç¹ï½³ç¹§E­ç¹ï½³ç¹§E°é™¦E¨é‰ï½ºé€•ï½¨ç¸ºE®é€•ï½»èœ’ä¸Šï½’ç¹ï½­ç¹ï½¼ç¹ä¼šï½¼ãƒ»xLibè›»æ™E‚„è›¹é–€E¾å¾Œã€’ãªã—âEèŸï½±è¬¨åŠ±â˜E¹§ä¹â—†ç¹§âˆšï¼E¸ºè–™ã€’é™¦å¾Œâ‰§ãƒ»ãƒ»
	rank_image_[0] = Master::mpResourceManager->LoadGraphics("Resource/2D/1ä½Epng");
	rank_image_[1] = Master::mpResourceManager->LoadGraphics("Resource/2D/2ä½Epng");
	rank_image_[2] = Master::mpResourceManager->LoadGraphics("Resource/2D/3ä½Epng");
	ranking_title_image_ = Master::mpResourceManager->LoadGraphics("Resource/2D/ãƒ©ãƒ³ã‚­ãƒ³ã‚°.png");
	point_img_ = Master::mpResourceManager->LoadGraphics("Resource/2D/ç‚¹.png");
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Master::GameFinishFlag = false;
	SetMouseDispFlag(true); // ç¹ï½¦ç¹ï½¼ç¹§E¶ç¹ï½¼ç¸ºå¾ŒãEç¹§E¿ç¹ï½³è¬«å ºE½æ‡Šã€’ç¸ºé˜ªE‹ç¹§åŒ»â‰§ç¸ºE«ç¹æ§­ãˆç¹§E¹ç¹§å®šï½¡E¨é‰ï½º
	Master::mpScore->LoadRanking(); // ç¹ï½©ç¹ï½³ç¹§E­ç¹ï½³ç¹§E°é™¦E¨é‰ï½ºé€•ï½¨ç¸ºE«è«æ™ï½­å€¥EE¹§å¾Œâ—†ç¹§E¹ç¹§E³ç¹§E¢ç¹§åµÎŸç¹ï½¼ç¹ãE
	Master::camera_->Initialize();

	Master::mpSoundManager->PlayBGM(SoundManager::kBgmTitle);
}

void TitleScene::Draw()
{
	Scene::Draw();
	DrawExtendGraph(0, 0, 1600, 900, title_graph_handle_, FALSE);

	int ufoDrawY = ufo_y_;

	// ç¹å³¨Î›ç¹ãEã’è³E­è‰ï½¥èŸæ‚¶ãƒ»ç¹§Eµç¹§E¤ç¹ï½³è±•ï½¢ç¸ºE§UFOç¸ºE®è±¬E®é©•ç¿«ãE¹ä¹Î“ç¹ï½¼ç¹§E·ç¹ï½§ç¹ï½³ç¹§å¸âEé€•ï½¨ç¸ºå¶EE
	if (!is_dragging_ufo_)
	{
		float ufoWave = sin(frame_count_ * 0.03f) * 20.0f;
		ufoDrawY += (int)ufoWave;
	}

	int ufoSize = 360;
	DrawExtendGraph(
		ufo_x_,
		ufoDrawY,
		ufo_x_ + ufoSize,
		ufoDrawY + ufoSize,
		ufo_graph_handle_,
		TRUE
	);

	// ç¹å¸™ãƒ°ç¹ï½¼è­ã‚…â†“ç¹æ‡Šã¡ç¹ï½³ç¹§å‘ˆåƒ‘èŸï½§è¬ å†—åˆ¤ç¸ºåŠ±Â€âˆ¬E¦å†¶E¦å¤‚å™ªç¸ºEªç¹è¼”ã…ç¹ï½¼ç¹å³¨ãƒ°ç¹ãEã‘ç¹§å‰E½¸å¼±âˆ´ç¹§ãƒ»
	for (int i = 0; i < buttons_.size(); i++)
	{
		float wave = sin(frame_count_ * 0.05f + (i * 1.5f)) * 10.0f;
		int drawY = buttons_[i].y + (int)wave;

		if (buttons_[i].is_hover == true)
		{
			int expand = 15;
			DrawExtendGraph(
				buttons_[i].x - expand,
				drawY - expand,
				buttons_[i].x + buttons_[i].w + expand,
				drawY + buttons_[i].h + expand,
				buttons_[i].graph_handle, TRUE);
		}
		else
		{
			DrawGraph(buttons_[i].x, drawY, buttons_[i].graph_handle, TRUE);
		}
	}

	
	DrawRankingUI();
	if (fade_state_ != kSceneFadeNone) {
		Scene::Fade(fade_state_);
	}
}

void TitleScene::Update()
{
	frame_count_++;

	if (fade_state_ != kSceneFadeOut)
	{
		cow_voice_timer_--;
		if (cow_voice_timer_ <= 0)
		{
			Master::mpSoundManager->PlaySE(SoundManager::kSeCow); // ç¹§E¿ç¹§E¤ç¹åŒ»Îé€•ï½»é«±E¢ç¸ºE®è²æ³ŒãEç¸ºE¨ç¸ºåŠ±â€»è³å£½æ‚E€§ãƒ»â†“èœ€å’²å‡½
			cow_voice_timer_ = GetRand(600) + 300; // èœ€å’²å‡½é«¢é¦´å›ˆç¹§åµÎ›ç¹ï½³ç¹Â€ç¹ï¿½è›¹æ‚¶E èœŠå€©EªE¿ç¸ºè¼”ï½’é«¦E²ç¸ºãƒ»
		}
	}

	if (fade_state_ == kSceneFadeOut)
	{
		Master::mpSoundManager->SetBGMVolume((Master::mpSoundManager->GetMasterBGMVolume() * (int)(255 - GetFadeAlpha())) / 255);
		if (GetFadeAlpha() >= 255)
		{
			SetFadeAlpha(255);
			Master::mpSceneManager->SetNextScene((SceneManager::SCENE_TYPE)next_scene_);
		}
		return;
	}

	int mouse_x_, mouse_y_;
	GetMousePoint(&mouse_x_, &mouse_y_);
	int mouseInput = GetMouseInput();
	int ufoSize = 360;

	// UFOç¹§E¯ç¹ï½ªç¹ãEã‘è­ã‚…â†“é««E½ç¸ºè‹“ï½¦âˆ«E´E½ç¸ºE®é–¾Eªèœå‹ŸE·E¡è—æ§­Î”ç¹ï½¼ç¹å³¨E’è¥ï½·èœè¼”âEç¹§ãƒ»
	if ((mouseInput & MOUSE_INPUT_LEFT) != 0)
	{
		if (mouse_x_ >= ufo_x_ && mouse_x_ <= ufo_x_ + ufoSize &&
			mouse_y_ >= ufo_y_ && mouse_y_ <= ufo_y_ + ufoSize)
		{
			is_auto_patrol_ = true;
			auto_patrol_timer_ = 240; // 4é˜å¸ä¿£ãƒ»ãƒ»40ç¹è¼”Îç¹ï½¼ç¹ï¿½ãƒ»èŠ½ãƒ»èœå‹ŸE·E¡è—æ§­E’é™¦å¾Œâ‰§
			is_dragging_ufo_ = true;
		}
	}
	else
	{
		is_dragging_ufo_ = false;
	}

	// è®Œå‹Ÿãƒ»éœE¢E¼E¸ºE®è¬¨E°è ‘ä¸Šï½’é€•ï½¨ç¸ºãƒ»â€»UFOç¹§å®šãEèœå‹ŸE·E¡è—æ§­EE¸ºå¸™ï½E
	if (is_auto_patrol_)
	{
		ufo_angle_ += 0.02f;
		ufo_x_ = 800 - (ufoSize / 2) + (int)(cos(ufo_angle_) * 400.0f);
		ufo_y_ = 450 - (ufoSize / 2) + (int)(sin(ufo_angle_) * 200.0f);

		auto_patrol_timer_--;
		if (auto_patrol_timer_ <= 0)
		{
			auto_patrol_timer_ = 0;
			is_auto_patrol_ = false;
		}
	}

	// UFOè¬«å ºE½æ‡‰ï½¸E­è‰ï½¥èŸæ‚¶â†“ç¹æ‡Šã¡ç¹ï½³ç¸ºE®ç¹æ§­ãˆç¹§E¹è›»E¤è³å£¹â†’ç¹§E·ç¹ï½¼ç¹ï½³é©•ï½·é˜ï½»éš•âˆµE±ã‚E½’é™¦å¾Œâ‰§
	if (is_dragging_ufo_ == false)
	{
		for (int i = 0; i < buttons_.size(); i++)
		{
			if (mouse_x_ >= buttons_[i].x && mouse_x_ <= buttons_[i].x + buttons_[i].w &&
				mouse_y_ >= buttons_[i].y && mouse_y_ <= buttons_[i].y + buttons_[i].h)
			{
				buttons_[i].is_hover = true;

				if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
				{
					switch (buttons_[i].type)
					{
					case SelectionManager::Title::NewGame:
						next_scene_ = SceneManager::kSceneLoading;
						fade_state_ = kSceneFadeOut;
						Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
						break;
					case SelectionManager::Title::Tutorial:
						next_scene_ = SceneManager::kSceneTutorial;
						fade_state_ = kSceneFadeOut;
						Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
						break;
					case SelectionManager::Title::OperationProcedures:
						next_scene_ = SceneManager::kSceneRule;
						fade_state_ = kSceneFadeOut;
						Master::mpSoundManager->PlaySE(SoundManager::kSeDecide);
						break;
					case SelectionManager::Title::titleOUT:
						PostQuitMessage(0); // OSç¸ºE«ç¹§E¢ç¹åŠ±Îœç¹§E±ç¹ï½¼ç¹§E·ç¹ï½§ç¹ï½³é‚¨ã‚E½ºãƒ»E¦âˆµE±ã‚E½’é¨¾âˆ½E¿E¡
						break;
					}
				}
			}
			else
			{
				buttons_[i].is_hover = false;
			}
		}
	}
	else
	{
		// UFOç¹å³¨Î›ç¹ãEã’è³E­ç¸ºE®ç¹æ‡Šã¡ç¹ï½³éš±E¤èœ¿æ¦Šï½¿æ‡Šï½’é«¦E²ç¸ºèˆŒâ—†ç¹§âˆšãEç¹èEãƒ»è¿¥E¶è«·ä¹ï½’éš—E£é«¯E¤
		for (int i = 0; i < buttons_.size(); i++) {
			buttons_[i].is_hover = false;
		}
	}

	Scene::Update();
}

void TitleScene::Finalize()
{
	// ResourceManager‚ÅŠÇ—‚µ‚Ä‚¢‚é‚½‚ßDeleteGraph‚ÍŒÄ‚Î‚È‚¢

	SetMouseDispFlag(false); // ƒQ[ƒ€’†‚ÌŒë‘€ì‚ğ–h‚®‚½‚ßƒ}ƒEƒX‚ğ”ñ•\¦‰»
	Master::mpSoundManager->StopBGM();
}

// [èœˆï½¥èœ‰å«Eãªã—[èœE½ºèœ‰å«Eãªã—[èœ‘ï½¯è´æ‡E•‘] é€•ï½»é«±E¢è³ç¿«â†“ç¹ï½©ç¹ï½³ç¹§E­ç¹ï½³ç¹§E°è¬¨E°è›Ÿï½¤ç¸ºE¨ç¹§E¢ç¹§E¤ç¹§E³ç¹ï½³ç¹§å‘ˆç·’é€•ï½»
void TitleScene::DrawRankingUI()
{
	int baseX = 40;
	int baseY = 40;

	DrawExtendGraph(
		baseX - 50,
		baseY - 100,
		baseX + 420,
		baseY + 120,
		ranking_title_image_,
		TRUE
	);

	for (int i = 0; i < 3; i++)
	{
		const Score::RankData& data = Master::mpScore->GetRanking(i);
		int y = baseY + 60 + i * 80;

		DrawExtendGraph(
			baseX,
			y - 20,
			baseX + 120,
			y + 100,
			rank_image_[i],
			TRUE
		);

		float scale = 0.6f;
		int w = (int)(80 * scale);
		int h = (int)(80 * scale);
		int drawY = y - 10 + (80 - h) / 2;

		Master::mpScore->DrawNumber(
			baseX + 180,
			drawY,
			data.score_,
			scale,
			4
		);

		int temp = data.score_;
		int digitCount = 0;
		if (temp == 0) digitCount = 1;
		else {
			while (temp > 0) {
				temp /= 10;
				digitCount++;
			}
		}
		if (digitCount < 4) digitCount = 4;
		int pointX = baseX + 180 + digitCount * w;
		DrawExtendGraph(pointX, drawY, pointX + w, drawY + h, point_img_, TRUE);
	}
}
