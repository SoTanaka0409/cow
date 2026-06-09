#include "Score.h"
#include <stdio.h>
#include <string.h>
#include "DxLib.h"

int Score::resultScore = 0;

Score::Score()
{
	score = 0;
	strcpy_s(playerName, sizeof(playerName), "NO NAME");
	nameIndex = 0;
	nameInputMode = false;
	inputHandle = -1;

	for (int i = 0; i < 3; i++)
	{
		strcpy_s(ranking[i].name, sizeof(ranking[i].name), "NONE");
		ranking[i].score = 0;
	}

	scoreTextImage = LoadGraph("Resource/2D/SCORE.png");
	minusImg = LoadGraph("Resource/2D/マイナス.png");

	// スコア描画用デジタル数字画像アセットのロード（実在する正しいアセットパスを設定）
	numberImg[0] = LoadGraph("Resource/2D/コンボ数00.png");
	numberImg[1] = LoadGraph("Resource/2D/コンボ数01.png");
	numberImg[2] = LoadGraph("Resource/2D/コンボ数02.png");
	numberImg[3] = LoadGraph("Resource/2D/コンボ数03.png");
	numberImg[4] = LoadGraph("Resource/2D/コンボ数04.png");
	numberImg[5] = LoadGraph("Resource/2D/コンボ数05.png");
	numberImg[6] = LoadGraph("Resource/2D/コンボ数06.png");
	numberImg[7] = LoadGraph("Resource/2D/コンボ数07.png");
	numberImg[8] = LoadGraph("Resource/2D/コンボ数08.png");
	numberImg[9] = LoadGraph("Resource/2D/コンボ数09.png");

	LoadRanking();
}

Score::~Score()
{
	// 読み込み済みグラフィックハンドルの解放漏れを防止
	DeleteGraph(scoreTextImage);
	DeleteGraph(minusImg);
	for (int i = 0; i < 10; i++)
	{
		DeleteGraph(numberImg[i]);
	}
}

void Score::Draw()
{
	int x = 100;
	int y = 100;
	int width = 200;
	int height = 100;

	// 画面左上に「SCORE」テキストを描画
	DrawExtendGraph(
		x,
		y,
		x + width,
		y + height,
		scoreTextImage,
		TRUE
	);

	// 現在の合計スコアを数字画像で右側に描画
	int drawX = x + 220;
	int temp = score;
	int digit[10];
	int digitCount = 0;

	if (temp == 0)
	{
		digit[digitCount++] = 0;
	}
	else
	{
		while (temp > 0)
		{
			digit[digitCount] = temp % 10;
			temp /= 10;
			digitCount++;
		}
	}

	for (int i = digitCount - 1; i >= 0; i--)
	{
		DrawExtendGraph(
			drawX,
			y,
			drawX + 80,
			y + 80,
			numberImg[digit[i]],
			TRUE
		);
		drawX += 80;
	}
}

void Score::AddScore(int value)
{
	score += value;
	if (score < 0)
	{
		score = 0; // スコアが負の値に減少するのを防止
	}
}

void Score::ResetScore()
{
	score = 0;
}

int Score::GetScore() const
{
	return score;
}

const char* Score::GetName() const
{
	return playerName;
}

void Score::Save()
{
	FILE* fp = nullptr;
	if (fopen_s(&fp, "score.txt", "w") == 0)
	{
		fprintf(fp, "%d\n", resultScore);
		for (int i = 0; i < 3; i++)
		{
			fprintf(fp, "%s %d\n", ranking[i].name, ranking[i].score);
		}
		fclose(fp);
	}
}

void Score::Load()
{
	FILE* fp = nullptr;
	if (fopen_s(&fp, "score.txt", "r") == 0)
	{
		if (fscanf_s(fp, "%d", &score) != 1) score = 0;
		for (int i = 0; i < 3; i++)
		{
			if (fscanf_s(fp, "%s %d", ranking[i].name, (unsigned)_countof(ranking[i].name), &ranking[i].score) != 2)
			{
				break;
			}
		}
		fclose(fp);
	}
}

void Score::StartNameInput()
{
	nameInputMode = true;
	nameIndex = 0;
	playerName[0] = '\0';

	// 文字入力欄をアクティブ化し、キーボード入力を開始する
	inputHandle = MakeKeyInput(0, 0, 64, 1);
	SetActiveKeyInput(inputHandle);
}

void Score::UpdateNameInput()
{
	if (!nameInputMode) return;

	// 入力中の文字をplayerNameに取得し、画面にネームエントリー用の表示を描画
	GetKeyInputString(playerName, inputHandle);
	DrawFormatString(300, 100, GetColor(255, 255, 255), "お名前を入力してください");
	DrawFormatString(300, 120, GetColor(255, 255, 255), "入力後、ENTERキーを押してください");
	DrawFormatString(300, 160, GetColor(255, 255, 0), ">> %s", playerName);

	// 何か文字が入力されている状態でENTERが押されたら入力受付を終了
	if (CheckHitKey(KEY_INPUT_RETURN) && strlen(playerName) > 0)
	{
		nameInputMode = false;
	}
}

bool Score::IsNameInputFinished() const
{
	return !nameInputMode;
}

void Score::AddRanking()
{
	int target = score;

	// 上位3位の中に今回のスコアが割り込める位置があるか探索
	for (int i = 0; i < 3; i++)
	{
		if (target > ranking[i].score)
		{
			// 下位の順位を1つずつ繰り下げる
			for (int j = 2; j > i; j--)
			{
				ranking[j] = ranking[j - 1];
			}

			// 今回のプレイヤーとして仮登録（後にネーム入力値で確定）
			strcpy_s(ranking[i].name, "PLAYER");
			ranking[i].score = target;
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
			"%d位  %s : %d",
			i + 1,
			ranking[i].name,
			ranking[i].score
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
			fprintf(fp, "%s %d\n", ranking[i].name, ranking[i].score);
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
			if (fscanf_s(fp, "%s %d", ranking[i].name, (unsigned)_countof(ranking[i].name), &ranking[i].score) != 2)
			{
				break;
			}
		}
		fclose(fp);
	}
	else
	{
		// ファイルが存在しない場合は、NONE（スコア0）でダミーデータを作成して初期セーブを行う
		for (int i = 0; i < 3; i++)
		{
			strcpy_s(ranking[i].name, "NONE");
			ranking[i].score = 0;
		}
		SaveRanking();
	}
}

void Score::AddScoreWithCombo(int baseScore, int comboCount)
{
	// 1コンボにつき基本点の20%のボーナス倍率を加算する
	float multiplier = 1.0f + comboCount * 0.2f;
	int finalScore = static_cast<int>(baseScore * multiplier);

	score += finalScore;
	if (score < 0)
	{
		score = 0;
	}
}

void Score::Initialize()
{
	score = 0;
}

void Score::DrawNumber(int x, int y, int value)
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

	int drawX = x;

	if (isMinus)
	{
		DrawExtendGraph(
			drawX,
			y,
			drawX + 80,
			y + 80,
			minusImg,
			TRUE
		);
		drawX += 80;
	}

	for (int i = digitCount - 1; i >= 0; i--)
	{
		DrawExtendGraph(
			drawX,
			y,
			drawX + 80,
			y + 80,
			numberImg[digit[i]],
			TRUE
		);
		drawX += 80;
	}
}

const Score::RankData& Score::GetRanking(int index) const
{
	return ranking[index];
}

void Score::SetResultScore(int value)
{
	resultScore = value;
}

int Score::GetResultScore()
{
	return resultScore;
}
