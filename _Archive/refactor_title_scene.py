import re

with open(r"c:\Users\student\Desktop\programing\cow\Source\TitleScene.cpp", "r", encoding="utf-8-sig") as f:
    content = f.read()

match = re.search(r"void TitleScene::Update\(\)\s*\{.*?\n	Scene::Update\(\);\n\}", content, re.DOTALL)
if match:
    original_update = match.group(0)
    
    new_update = """void TitleScene::Update()
{
	frame_count_++;

	UpdateCowVoice();

	int mouse_x_, mouse_y_;
	GetMousePoint(&mouse_x_, &mouse_y_);
	int mouseInput = GetMouseInput();
	int ufoSize = 360;

	UpdateUFOInteraction(mouseInput, mouse_x_, mouse_y_, ufoSize);
	UpdateUFOAutoPatrol(ufoSize);
	UpdateMenuButtons(mouse_x_, mouse_y_);

	Scene::Update();
}

void TitleScene::UpdateCowVoice()
{
	if (fade_state_ != kSceneFadeOut)
	{
		cow_voice_timer_--;
		if (cow_voice_timer_ <= 0)
		{
			Master::mpSoundManager->PlaySE(SoundManager::kSeCow);
			// 機墁E的な周期感を無くし自然な環境音にするため、次回鳴動までの間隔を5〜15秒で散らす
			cow_voice_timer_ = GetRand(600) + 300;
		}
	}
}

void TitleScene::UpdateUFOInteraction(int mouseInput, int mouse_x, int mouse_y, int ufoSize)
{
	// イースターエッグ（隠し要素）として、UFO突っつき時に一定時間自動巡回モードへ移行させる
	if ((mouseInput & MOUSE_INPUT_LEFT) != 0)
	{
		if (mouse_x >= ufo_x_ && mouse_x <= ufo_x_ + ufoSize &&
			mouse_y >= ufo_y_ && mouse_y <= ufo_y_ + ufoSize)
		{
			is_auto_patrol_ = true;
			auto_patrol_timer_ = 240;
			is_dragging_ufo_ = true;
		}
	}
	else
	{
		is_dragging_ufo_ = false;
	}
}

void TitleScene::UpdateUFOAutoPatrol(int ufoSize)
{
	// 画面中央（800, 450）を起点とした綺麗な長楕円の軌道を描かせるための極座標計算
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
}

void TitleScene::UpdateMenuButtons(int mouse_x, int mouse_y)
{
	// メニューボタンのインタラクション処琁EUFOドラッグ中は誤爆を防ぐため判定をパスする
	if (is_dragging_ufo_ == false)
	{
		for (int i = 0; i < buttons_.size(); i++)
		{
			if (mouse_x >= buttons_[i].x && mouse_x <= buttons_[i].x + buttons_[i].w &&
				mouse_y >= buttons_[i].y && mouse_y <= buttons_[i].y + buttons_[i].h)
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
						// Win32 API のメッセージループを終了させ、アプリケーションを閉じる
						PostQuitMessage(0);
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
		// UFO操作中の画面のちらつきや誤動作を防止するため、全ボタンのホバー演Eを消去する
		for (int i = 0; i < buttons_.size(); i++) {
			buttons_[i].is_hover = false;
		}
	}
}"""
    content = content.replace(original_update, new_update)

    with open(r"c:\Users\student\Desktop\programing\cow\Source\TitleScene.cpp", "w", encoding="utf-8-sig") as f:
        f.write(content)
    print("Replaced TitleScene::Update successfully.")
else:
    print("Could not find TitleScene::Update.")
