#pragma once

// 繝励Ξ繧､繝､繝ｼ縺ｮ繧ｹ繧ｳ繧｢縲√Λ繝ｳ繧ｭ繝ｳ繧ｰ縲√ロ繝ｼ繝蜈･蜉帙ｒ邂｡逅・☆繧・
class Score
{
public:
	// 繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ菫晄戟逕ｨ
	struct RankData
	{
		char name_[64];
		int score_;
	};

	Score();
	~Score();

	/*
	 * @brief 繧ｹ繧ｳ繧｢蛻晄悄蛹・
	 * [蜈･蜉嫋 なし[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] score縺ｮ蛻晄悄蛹・
	 */
	void Initialize();

	const RankData& GetRanking(int index) const;

	/*
	 * @brief 雋謨ｰ縺ｫ縺ｪ繧峨↑縺・ｈ縺・せ繧ｳ繧｢繧貞刈邂・
	 * [蜈･蜉嫋 value: 蜉邂励せ繧ｳ繧｢ [蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] score縺ｮ譖ｴ譁ｰ
	 */
	void AddScore(int value);
	
	/*
	 * @brief 繧ｹ繧ｳ繧｢繝ｪ繧ｻ繝・ヨ
	 * [蜈･蜉嫋 なし[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] score繧・縺ｫ險ｭ螳・
	 */
	void ResetScore();

	int GetScore() const;
	const char* GetName() const;

	/*
	 * @brief 繧ｿ繧､繝繧｢繝・・蠕後・繝阪・繝繧ｨ繝ｳ繝医Μ繝ｼ髢句ｧ・
	 * [蜈･蜉嫋 なし[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] 繧ｭ繝ｼ蜈･蜉帙ワ繝ｳ繝峨Ν縺ｮ譛牙柑蛹・
	 */
	void StartNameInput();

	/*
	 * @brief 繝阪・繝繧ｨ繝ｳ繝医Μ繝ｼ縺ｮ豈弱ヵ繝ｬ繝ｼ繝蜃ｦ逅・
	 * [蜈･蜉嫋 なし[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] playerName縺ｮ譖ｴ譁ｰ縲・NTER縺ｧ縺ｮ邨ゆｺ・
	 */
	void UpdateNameInput();

	bool IsNameInputFinished() const;

	/*
	 * @brief 繧ｹ繧ｳ繧｢繧偵Λ繝ｳ繧ｭ繝ｳ繧ｰ(荳贋ｽ・菴・縺ｫ逋ｻ骭ｲ
	 * [蜈･蜉嫋 なし[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] ranking縺ｮ譖ｴ譁ｰ
	 */
	void AddRanking();

	/*
	 * @brief 繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ繧池ank.txt縺ｫ菫晏ｭ・
	 * [蜈･蜉嫋 なし[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] rank.txt縺ｸ縺ｮ譖ｸ縺崎ｾｼ縺ｿ
	 */
	void SaveRanking();

	/*
	 * @brief rank.txt縺九ｉ繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ繧定ｪｭ縺ｿ霎ｼ縺ｿ・井ｸ榊惠譎ゅ・蛻晄悄蛹厄ｼ・
	 * [蜈･蜉嫋 なし[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] ranking縺ｮ譖ｴ譁ｰ縲〉ank.txt縺ｮ菴懈・
	 */
	void LoadRanking();

	/*
	 * @brief 繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ縺ｮ繝・ヰ繝・げ謠冗判
	 * [蜈･蜉嫋 x, y: 謠冗判蠎ｧ讓・[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] 逕ｻ髱｢謠冗判
	 */
	void DrawRanking(int x, int y);

	/*
	 * @brief 迴ｾ蝨ｨ縺ｮ繧ｹ繧ｳ繧｢縺ｨ繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ繧痴core.txt縺ｫ菫晏ｭ・
	 * [蜈･蜉嫋 なし[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] score_.txt縺ｸ縺ｮ譖ｸ縺崎ｾｼ縺ｿ
	 */
	void Save();

	/*
	 * @brief score_.txt縺九ｉ蜑榊屓縺ｮ繧ｹ繧ｳ繧｢縺ｨ繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ繧定ｪｭ縺ｿ霎ｼ縺ｿ
	 * [蜈･蜉嫋 なし[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] score縺ｨranking縺ｮ譖ｴ譁ｰ
	 */
	void Load();

	/*
	 * @brief 繝励Ξ繧､荳ｭ縺ｮ繧ｹ繧ｳ繧｢謠冗判
	 * [蜈･蜉嫋 なし[蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] 逕ｻ髱｢謠冗判
	 */
	void Draw();

	/*
	 * @brief 莉ｻ諢上・謨ｰ蛟､繧堤判蜒上ヵ繧ｩ繝ｳ繝医〒謠冗判
	 * [蜈･蜉嫋 x,y: 蠎ｧ讓・ value: 謨ｰ蛟､ [蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] 逕ｻ髱｢謠冗判
	 */
	void DrawNumber(int x, int y, int value, float scale = 1.0f, int minDigits = 1);

	/*
	 * @brief 繧ｳ繝ｳ繝懈焚縺ｫ蠢懊§縺溷咲紫縺ｧ繧ｹ繧ｳ繧｢蜉邂・
	 * [蜈･蜉嫋 baseScore: 蝓ｺ譛ｬ蛟､, combo_count_: 繧ｳ繝ｳ繝懈焚 [蜃ｺ蜉嫋 なし[蜑ｯ菴懃畑] score縺ｮ譖ｴ譁ｰ
	 */
	void AddScoreWithCombo(int baseScore, int combo_count_);

	// 繧ｷ繝ｼ繝ｳ髢灘ｼ輔″邯吶℃逕ｨ
	static void SetResultScore(int value);
	static int GetResultScore();

private:
	int input_handle_;              // DxLib繝阪・繝蜈･蜉帷畑
	char player_name_[64];          // 蜈･蜉帙ユ繧ｭ繧ｹ繝医ヰ繝・ヵ繧｡
	bool name_input_mode_;           // 蜈･蜉帙Δ繝ｼ繝峨ヵ繝ｩ繧ｰ

	int score_;                    // 迴ｾ蝨ｨ縺ｮ繧ｹ繧ｳ繧｢

	int score_image_;               // 莠育ｴ・
	int number_img_[10];            // 謨ｰ蟄礼判蜒上ワ繝ｳ繝峨Ν(0-9)
	int minus_img_;                 // 繝槭う繝翫せ逕ｻ蜒上ワ繝ｳ繝峨Ν

	int name_index_;                // 莠育ｴ・
	int score_text_image_;           // 縲郡CORE縲咲判蜒上ワ繝ｳ繝峨Ν

	RankData ranking_[3];          // 繝上う繧ｹ繧ｳ繧｢荳贋ｽ・場合

	static int result_score_;       // 譛邨ら佐蠕励せ繧ｳ繧｢
};