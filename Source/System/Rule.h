#pragma once
#include"Scene.h"

/// @brief プレイヤーがゲーム本編を遊ぶ前に、操作方法やルール（勝敗条件・制限時間等）を確認・把握するための専用シーン
class Rule : public Scene
{
public:
	Rule();
	~Rule();

	/// @brief ルール説明画像の読み込み、ページ送り用フォントの初期化
	void Initialize() override;

	/// @brief 入力検知に伴うページ番号の更新、特定キー入力時のタイトル・ゲームへの画面遷移トリガー
	void Update() override;

	/// @brief 操作図解、テキスト、次へ進むためのナビゲーションUIの画面描画
	void Draw() override;

	/// @brief 表示用グラフィックハンドル、フォントリソースのメモリ解放
	void Finalize() override;

	enum MenuType
	{
		kMenuBgm,
		kMenuSe,
		kMenuShadow,
		kMenuDebug,
		kMenuBack,
		kMenuMax
	};

private:
	/// @brief 処理の実行
	void DrawBackground();
	/// @brief 処理の実行
	void DrawMenu();

	int rule_graph_;        ///< 描画やリソース管理に使用するハンドル
	int font_handle_;       ///< 描画やリソース管理に使用するハンドル
	int title_font_handle_; ///< 描画やリソース管理に使用するハンドル

	MenuType selected_index_ = kMenuBgm; ///< 内部状態を管理する値

	/// @brief 音量スライダー操作時にSEが毎フレーム暴発して処理負荷と耳障りな音覚が残るのを防ぐタイマー
	int play_se_delay_ = 0; ///< 音量やサウンド再生状態を管理する値

	/// @brief 前シーンの決定キー入力の「長押し」を誤検知し、ロード直後に意図せずページがめくれてしまう挙動を防ぐ猶予カウンター
	int scene_frames_ = 0; ///< 内部状態を管理する値

	/// @brief 処理の実行
	bool UpdateFadeState();
	/// @brief 処理の実行
	void UpdateMenu(int mouse_x, int mouse_y, int mouseInput, bool isMouseClicked, bool isMouseHeld);
};
