#include "Score.h"
#include <stdio.h>
#include <string.h>
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"

int Score::result_score_ = 0;

Score::Score()
{
	score_ = 0;
	strcpy_s(player_name_, sizeof(player_name_), "NO NAME");
	name_index_ = 0;
	name_input_mode_ = false;
	input_handle_ = -1;

	for (int i = 0; i < 3; i++)
	{
		strcpy_s(ranking_[i].name_, sizeof(ranking_[i].name_), "NONE");
		ranking_[i].score_ = 0;
	}

	score_text_image_ = Master::mpResourceManager->LoadGraphics("Resource/2D/SCORE.png");
	minus_img_ = Master::mpResourceManager->LoadGraphics("Resource/2D/マイナス.png");

	// スコア描画用アセットロード
	number_img_[0] = Master::mpResourceManager->LoadGraphics("Resource/2D/コンボ数00.png");
	number_img_[1] = Master::mpResourceManager->LoadGraphics("Resource/2D/コンボ数01.png");
	number_img_[2] = Master::mpResourceManager->LoadGraphics("Resource/2D/コンボ数02.png");
	number_img_[3] = Master::mpResourceManager->LoadGraphics("Resource/2D/コンボ数03.png");
	number_img_[4] = Master::mpResourceManager->LoadGraphics("Resource/2D/コンボ数04.png");
	number_img_[5] = Master::mpResourceManager->LoadGraphics("Resource/2D/コンボ数05.png");
	number_img_[6] = Master::mpResourceManager->LoadGraphics("Resource/2D/コンボ数06.png");
	number_img_[7] = Master::mpResourceManager->LoadGraphics("Resource/2D/コンボ数07.png");
	number_img_[8] = Master::mpResourceManager->LoadGraphics("Resource/2D/コンボ数08.png");
	number_img_[9] = Master::mpResourceManager->LoadGraphics("Resource/2D/コンボ数09.png");

	LoadRanking();
}

Score::~Score()
{
}

void Score::Draw()
{
	int x = Utility::kUiBaseX;
	int y = Utility::kUiScoreY;
	int width = Utility::kUiPanelW;
	int height = Utility::kUiPanelH;

	DrawExtendGraph(
		x,
		y,
		x + width,
		y + height,
		score_text_image_,
		TRUE
	);

	DrawNumber(Utility::kUiDigitX, y, score_, 1.0f, 4);
}

void Score::AddScore(int value)
{
	score_ += value;
	if (score_ < 0)
	{
		score_ = 0; // 負のスコアを防止
	}
}

void Score::ResetScore()
{
	score_ = 0;
}

int Score::GetScore() const
{
	return score_;
}

const char* Score::GetName() const
{
	return player_name_;
}

void Score::Save()
{
	FILE* fp = nullptr;
	if (fopen_s(&fp, "score_.txt", "w") == 0)
	{
		fprintf(fp, "%d\n", result_score_);
		for (int i = 0; i < 3; i++)
		{
			fprintf(fp, "%s %d\n", ranking_[i].name_, ranking_[i].score_);
		}
		fclose(fp);
	}
}

void Score::Load()
{
	FILE* fp = nullptr;
	if (fopen_s(&fp, "score_.txt", "r") == 0)
	{
		if (fscanf_s(fp, "%d", &score_) != 1) score_ = 0;
		for (int i = 0; i < 3; i++)
		{
			if (fscanf_s(fp, "%s %d", ranking_[i].name_, (unsigned)_countof(ranking_[i].name_), &ranking_[i].score_) != 2)
			{
				break;
			}
		}
		fclose(fp);
	}
}

void Score::StartNameInput()
{
	name_input_mode_ = true;
	name_index_ = 0;
	player_name_[0] = '\0';

	// DxLibのキーボード入力システムを有効化
	input_handle_ = MakeKeyInput(0, 0, 64, 1);
	SetActiveKeyInput(input_handle_);
}

void Score::UpdateNameInput()
{
	if (!name_input_mode_) return;

	GetKeyInputString(player_name_, input_handle_);
	/*DrawFormatString(300, 100, GetColor(255, 255, 255), "お名前を入力してください");
	DrawFormatString(300, 120, GetColor(255, 255, 255), "入力後はENTERキーを押してください");
	DrawFormatString(300, 160, GetColor(255, 255, 0), ">> %s", player_name_);*/

	// 入力完了判定（ENTERキー）
	if (CheckHitKey(KEY_INPUT_RETURN) && strlen(player_name_) > 0)
	{
		name_input_mode_ = false;
		if (name_index_ >= 0 && name_index_ < 3)
		{
			strcpy_s(ranking_[name_index_].name_, sizeof(ranking_[name_index_].name_), player_name_);
		}
	}
}

bool Score::IsNameInputFinished() const
{
	return !name_input_mode_;
}

void Score::AddRanking()
{
	int target = score_;
	name_index_ = -1;

	// 上位3枠以内にランクインした場合確定
	for (int i = 0; i < 3; i++)
	{
		if (target > ranking_[i].score_)
		{
			// 後続を一つ下へ
			for (int j = 2; j > i; j--)
			{
				ranking_[j] = ranking_[j - 1];
			}

			// 仮に"PLAYER"で登録、ネームエントリー後に確定する
			strcpy_s(ranking_[i].name_, "PLAYER");
			ranking_[i].score_ = target;
			name_index_ = i;
			break;
		}
	}
}

void Score::DrawRanking(int x, int y)
{
	DrawFormatString(x, y, GetColor(255, 255, 255), "RANKING");
	for (int i = 0; i < 3; i++)
	{
		DrawFormatString(
			x,
			y + 30 + i * 20,
			GetColor(255, 255, 255),
			"%d位 %s : %d",
			i + 1,
			ranking_[i].name_,
			ranking_[i].score_
		);
	}
}

void Score::SaveRanking()
{
	FILE* fp = nullptr;
	if (fopen_s(&fp, "rank.txt", "w") == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			fprintf(fp, "%s %d\n", ranking_[i].name_, ranking_[i].score_);
		}
		fclose(fp);
	}
}

void Score::LoadRanking()
{
	FILE* fp = nullptr;
	if (fopen_s(&fp, "rank.txt", "r") == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			if (fscanf_s(fp, "%s %d", ranking_[i].name_, (unsigned)_countof(ranking_[i].name_), &ranking_[i].score_) != 2)
			{
				break;
			}
		}
		fclose(fp);
	}
	else
	{
		// ランキングファイルがない場合はダミーデータで新規作成
		for (int i = 0; i < 3; i++)
		{
			strcpy_s(ranking_[i].name_, "NONE");
			ranking_[i].score_ = 0;
		}
		SaveRanking();
	}
}

void Score::AddScoreWithCombo(int baseScore, int comboCount)
{
	// コンボボーナス: 1コンボあたり20%増加
	float multiplier = 1.0f + comboCount * 0.2f;
	int finalScore = static_cast<int>(baseScore * multiplier);

	score_ += finalScore;
	if (score_ < 0)
	{
		score_ = 0;
	}
}

void Score::Initialize()
{
	score_ = 0;
}

void Score::DrawNumber(int x, int y, int value, float scale, int minDigits)
{
	int digit[10];
	int digitCount = 0;
	bool isMinus = false;
	int temp = value;

	if (temp < 0)
	{
		isMinus = true;
		temp = -temp;
	}

	if (temp == 0)
	{
		digit[digitCount++] = 0;
	}
	else
	{
		while (temp > 0)
		{
			digit[digitCount++] = temp % 10;
			temp /= 10;
		}
	}

	while (digitCount < minDigits)
	{
		digit[digitCount++] = 0;
	}

	int drawX = x;
	int w = (int)(80 * scale);
	int h = (int)(80 * scale);

	if (isMinus)
	{
		DrawExtendGraph(
			drawX,
			y,
			drawX + w,
			y + h,
			minus_img_,
			TRUE
		);
		drawX += w;
	}

	for (int i = digitCount - 1; i >= 0; i--)
	{
		DrawExtendGraph(
			drawX,
			y,
			drawX + w,
			y + h,
			number_img_[digit[i]],
			TRUE
		);
		drawX += w;
	}
}

const Score::RankData& Score::GetRanking(int index) const
{
	return ranking_[index];
}

void Score::SetResultScore(int value)
{
	result_score_ = value;
}

int Score::GetResultScore()
{
	return result_score_;
}