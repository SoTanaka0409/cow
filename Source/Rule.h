#pragma once
#include"Scene.h"

// 業務ルール：プレイヤーがゲーム本編を遊ぶ前に、操作方法やルール（勝敗条件・制限時間等）を確認・把握するための専用シーン
class Rule : public Scene
{
public:
	Rule();
	~Rule();

	// 入力：なし
	// 出力：なし
	// 副作用：ルール説明画像の読み込み、ページ送り用フォントの初期化
	void Initialize() override;

	// 入力：なし
	// 出力：なし
	// 副作用：入力検知に伴うページ番号の更新、特定キー入力時のタイトル・ゲームへの画面遷移トリガー
	void Update() override;

	// 入力：なし
	// 出力：なし
	// 副作用：操作図解、テキスト、次へ進むためのナビゲーションUIの画面描画
	void Draw() override;

	// 入力：なし
	// 出力：なし
	// 副作用：表示用グラフィックハンドル、フォントリソースのメモリ解放
	void Finalize() override;

	enum MenuType
	{
		kMenuBgm,
		kMenuSe,
		kMenuShadow,
		kMenuBack,
		kMenuMax
	};

private:
	void DrawBackground();
	void DrawMenu();

	int rule_graph_;
	int font_handle_;
	int title_font_handle_;

	MenuType selected_index_ = kMenuBgm;

	// パフォーマンス理由：音量スライダー操作時にSEが毎フレーム暴発して処理負荷と耳障りな音覚が残るのを防ぐタイマー
	int play_se_delay_ = 0;

	// バグ回避：前シーンの決定キー入力の「長押し」を誤検知し、ロード直後に意図せずページがめくれてしまう挙動を防ぐ猶予カウンター
	int scene_frames_ = 0;

	bool UpdateFadeState();
	void UpdateMenu(int mouse_x, int mouse_y, int mouseInput, bool isMouseClicked, bool isMouseHeld);
};
