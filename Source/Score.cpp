#include "Score.h"
#include <stdio.h>
#include <string.h>
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
int Score::result_score_ = 0;
/*
 * オブジェクトの初期化を行うため
 * [入力] なし
 * [出力] なし
 * [副作用] メンバ変数の初期化、画像の読み込み
 */
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
	score_text_image_ = Master::resource_manager_->LoadGraphics("Resource/2D/ScoreHudText.png");
	minus_img_ = Master::resource_manager_->LoadGraphics("Resource/2D/MinusText.png");
	number_img_[0] = Master::resource_manager_->LoadGraphics("Resource/2D/ComboDigit00.png");
	number_img_[1] = Master::resource_manager_->LoadGraphics("Resource/2D/ComboDigit01.png");
	number_img_[2] = Master::resource_manager_->LoadGraphics("Resource/2D/ComboDigit02.png");
	number_img_[3] = Master::resource_manager_->LoadGraphics("Resource/2D/ComboDigit03.png");
	number_img_[4] = Master::resource_manager_->LoadGraphics("Resource/2D/ComboDigit04.png");
	number_img_[5] = Master::resource_manager_->LoadGraphics("Resource/2D/ComboDigit05.png");
	number_img_[6] = Master::resource_manager_->LoadGraphics("Resource/2D/ComboDigit06.png");
	number_img_[7] = Master::resource_manager_->LoadGraphics("Resource/2D/ComboDigit07.png");
	number_img_[8] = Master::resource_manager_->LoadGraphics("Resource/2D/ComboDigit08.png");
	number_img_[9] = Master::resource_manager_->LoadGraphics("Resource/2D/ComboDigit09.png");
	LoadRanking();
}
/*
 * メモリ解放のため
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
Score::~Score()
{
}
/*
 * 現在のスコアを画面に描画するため
 * [入力] なし
 * [出力] なし
 * [副作用] 画面描画
 */
void Score::Draw()
{
	int x = Utility::kUiBaseX;
	int y = Utility::kUiScoreY;
	int width = Utility::kUiPanelW;
	int height = Utility::kUiPanelH;
	DrawExtendGraph(x, y, x + width, y + height, score_text_image_, TRUE);
	DrawNumber(Utility::kUiDigitX, y, score_, 1.0f, 4);
}
/*
 * 獲得したスコアを加算するため
 * [入力] value: 加算するスコア
 * [出力] なし
 * [副作用] score_の更新
 */
void Score::AddScore(int value)
{
	score_ += value;
	if (score_ < 0)
	{
		// 負のスコアを防ぐため
		score_ = 0;
	}
	else if (score_ >=9999)
	{
		//規定値を超えないように
		score_ = 9999;
	}
}
/*
 * スコアをリセットするため
 * [入力] なし
 * [出力] なし
 * [副作用] score_を0に設定
 */
void Score::ResetScore()
{
	score_ = 0;
}
/*
 * 現在のスコアを取得するため
 * [入力] なし
 * [出力] 現在のスコア
 * [副作用] なし
 */
int Score::GetScore() const
{
	return score_;
}
/*
 * 入力されたプレイヤー名を取得するため
 * [入力] なし
 * [出力] プレイヤー名の文字列
 * [副作用] なし
 */
const char* Score::GetName() const
{
	return player_name_;
}
/*
 * デバッグ用にスコア情報をファイルに保存するため
 * [入力] なし
 * [出力] なし
 * [副作用] score_.txtへの書き込み
 */
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
/*
 * デバッグ用にスコア情報をファイルから読み込むため
 * [入力] なし
 * [出力] なし
 * [副作用] score_とranking_の更新
 */
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
/*
 * リザルト画面でネームエントリーを開始するため
 * [入力] なし
 * [出力] なし
 * [副作用] キー入力ハンドルの作成と有効化
 */
void Score::StartNameInput()
{
	name_input_mode_ = true;
	name_index_ = 0;
	player_name_[0] = '\0';
	// 文字入力処理をDxLibに委譲するため
	input_handle_ = MakeKeyInput(0, 0, 64, 1);
	SetActiveKeyInput(input_handle_);
}
/*
 * キーボード入力からプレイヤー名を更新するため
 * [入力] なし
 * [出力] なし
 * [副作用] player_name_の更新、入力完了状態の変更
 */
void Score::UpdateNameInput()
{
	if (!name_input_mode_) return;
	GetKeyInputString(player_name_, input_handle_);
	// 入力確定を検知して受付を終了するため
	if (CheckHitKey(KEY_INPUT_RETURN) && strlen(player_name_) > 0)
	{
		name_input_mode_ = false;
		if (name_index_ >= 0 && name_index_ < 3)
		{
			strcpy_s(ranking_[name_index_].name_, sizeof(ranking_[name_index_].name_), player_name_);
		}
	}
}
/*
 * ネームエントリーが完了したか判定するため
 * [入力] なし
 * [出力] 完了したかどうか
 * [副作用] なし
 */
bool Score::IsNameInputFinished() const
{
	return !name_input_mode_;
}
/*
 * 獲得スコアをランキングに反映させるため
 * [入力] なし
 * [出力] なし
 * [副作用] ranking_の更新
 */
void Score::AddRanking()
{
	int target = score_;
	name_index_ = -1;
	// 上位3位以内にランクインした場合に順位を更新するため
	for (int i = 0; i < 3; i++)
	{
		if (target > ranking_[i].score_)
		{
			// 下位のランキングを押し下げるため
			for (int j = 2; j > i; j--)
			{
				ranking_[j] = ranking_[j - 1];
			}
			// 仮名で登録しネームエントリーで確定させるため
			strcpy_s(ranking_[i].name_, "PLAYER");
			ranking_[i].score_ = target;
			name_index_ = i;
			break;
		}
	}
}
/*
 * ランキング結果を画面に描画するため
 * [入力] x: 描画X座標, y: 描画Y座標
 * [出力] なし
 * [副作用] 画面描画
 */
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
/*
 * ランキング情報をファイルに保存するため
 * [入力] なし
 * [出力] なし
 * [副作用] rank.txtへの書き込み
 */
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
/*
 * ランキング情報をファイルから読み込むため
 * [入力] なし
 * [出力] なし
 * [副作用] ranking_の更新、ファイルがない場合は新規作成
 */
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
		// 記録ファイルがない場合はダミーデータで新規作成するため
		for (int i = 0; i < 3; i++)
		{
			strcpy_s(ranking_[i].name_, "NONE");
			ranking_[i].score_ = 0;
		}
		SaveRanking();
	}
}
/*
 * コンボボーナスを含めたスコアを加算するため
 * [入力] baseScore: 基本スコア, combo_count_: コンボ数
 * [出力] なし
 * [副作用] score_の更新
 */
void Score::AddScoreWithCombo(int baseScore, int combo_count_)
{
	// コンボ数に応じてボーナス倍率を決定するため
	float multiplier = 1.0f + combo_count_ * 0.2f;
	int finalScore = static_cast<int>(baseScore * multiplier);
	score_ += finalScore;
	if (score_ < 0)
	{
		score_ = 0;
	}
}
/*
 * ゲーム再開時にスコアを初期化するため
 * [入力] なし
 * [出力] なし
 * [副作用] score_の初期化
 */
void Score::Initialize()
{
	score_ = 0;
}
/*
 * 画像を使って数値を画面に描画するため
 * [入力] x: X座標, y: Y座標, value: 数値, scale: 拡大率, minDigits: 最小桁数
 * [出力] なし
 * [副作用] 画面描画
 */
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
		DrawExtendGraph(drawX, y, drawX + w, y + h, minus_img_, TRUE);
		drawX += w;
	}
	for (int i = digitCount - 1; i >= 0; i--)
	{
		DrawExtendGraph(drawX, y, drawX + w, y + h, number_img_[digit[i]], TRUE);
		drawX += w;
	}
}
/*
 * 指定した順位のランキングデータを取得するため
 * [入力] index: 取得する順位のインデックス
 * [出力] ランキングデータ
 * [副作用] なし
 */
const Score::RankData& Score::GetRanking(int index) const
{
	return ranking_[index];
}
/*
 * シーン間でリザルトスコアを引き継ぐため
 * [入力] value: 保存するスコア
 * [出力] なし
 * [副作用] result_score_の更新
 */
void Score::SetResultScore(int value)
{
	result_score_ = value;
}
/*
 * リザルトスコアを取得するため
 * [入力] なし
 * [出力] リザルトスコア
 * [副作用] なし
 */
int Score::GetResultScore()
{
	return result_score_;
}
/*
 * スコアと単位画像を並べて描画するため
 * [入力] x: X座標, y: Y座標, score: スコア, scale: 拡大率, minDigits: 最小桁数, point_graph_handle: 画像ハンドル
 * [出力] なし
 * [副作用] 画面描画
 */
void Score::DrawScoreWithPoint(int x, int y, int score, float scale, int minDigits, int point_graph_handle)
{
	DrawNumber(x, y, score, scale, minDigits);
	int temp = score;
	int digitCount = 0;
	if (temp == 0) {
		digitCount = 1;
	} else {
		if (temp < 0) temp = -temp;
		while (temp > 0) {
			temp /= 10;
			digitCount++;
		}
	}
	if (digitCount < minDigits) {
		digitCount = minDigits;
	}
	int w = (int)(80 * scale);
	int h = (int)(80 * scale);
	int pointX = x + digitCount * w;
	DrawExtendGraph(pointX, y, pointX + w, y + h, point_graph_handle, TRUE);
}
