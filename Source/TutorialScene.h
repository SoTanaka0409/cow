#pragma once
#include "Scene.h"
#include "ModelUtility.h"
#include "CowManager.h"
#include "Thunder.h"
#include "Texture.h"

class Camera;
class Combo;

// 設計ルール：ゲーム本編（牛の拉致アクション）の基本操作や各システムを段階的に学ばせるための練習用シーン
class TutorialScene : public Scene
{
public:
	enum State
	{
		kStateMove,
		kStateBeam,
		kStateComboScore,
		kStatePhase,
		kStateSkill,
		kStateFever,
		kStateEnd
	};

	enum State_fever
	{
		kFever1,
		kFever2,
		kFever3,
	};

	TutorialScene();
	virtual ~TutorialScene();

	// 入力：なし
	// 出力：なし
	// 副作用：操作解説用テクスチャのロード、初期配置オブジェクトの設定、専用BGMの再生
	void Initialize() override;

	// 入力：なし
	// 出力：なし
	// 副作用：現在の進捗ステートに応じた操作クリア判定、および段階的な説明UI・フラグの更新
	void Update() override;

	// 入力：なし
	// 出力：なし
	// 副作用：3Dゲーム空間、および画面中央への固定メッセージ・操作ナビゲーションUIの描画
	void Draw() override;

	// 入力：なし
	// 出力：なし
	// 副作用：チュートリアル専用BGMの停止、ロードしたUI用リソースの全解放
	void Finalize() override;

	void SetCamera(Camera* camera);

	// 出力：なし
	// 副作用：プレイヤーが牛の格納（アブダクション）に成功したというトリガーをシーン側に通知する

private:
	// 副作用：プレイヤーが勝手に遠くへ行かないよう、初期位置の周囲を囲う障害物（柵）を生成・配置する
	void CreateFences();

	// 副作用：カメラやプレイヤーがステージ外の暗黒空間に落下するのを防ぐ透明な衝突判定（防壁）を生成する
	void CreateWalls();

	void UpdateStateMove();
	void UpdateStateBeam();
	void UpdateStateComboScore();
	void UpdateStatePhase();
	void UpdateStateSkill();
	void UpdateStateFever();
	void UpdateStateEnd();

	// 入力：text=表示したい文章, yOffset=標準描画位置からの上下ズレ（px）
	// 副作用：指定された文字列を画面中央に見やすくレイアウトして描画する
	void DrawTutorialText(const char* text, int yOffset = 0);

private:
	int font_handle_;                // 画面上に大きく操作方法を表示するためのアンチエイリアス付きフォントハンドル
	int timer_count_;                // テキストの文字送りや、課題クリアから次の解説へ進むまでの暗転猶予タイマー

	State state_ = kStateMove;       // チュートリアル全体が現在どの解説フェーズにあるかを示す進行管理変数
	State_fever fever_state_;        // 後半のフィーバー体験セクションにおける、内部の演出ステップ管理用ステート

	Texture* texture_;               // 各ステップの背景に表示する、操作図解（コントローラー画像など）の個別テクスチャ
	Texture* texture2_;             
	Texture* texture3_;             
	Texture* texture4_;             
	Texture* texture5_;             
	Texture* texture6_;             

	int mouse_x_;                    // ビーム照射やUIクリックにおける、マウスポインタのX軸入力キャッシュ

	Thunder* thunder_;               // 雷トラップの回避テストを体験させるために、シーン内で個別生成する環境オブジェクト
	Camera* camera_ = nullptr;       // プレイヤーの視点を解説用（固定俯瞰）に強制クランプするための制御用カメラ
	Combo* combo_ = nullptr;         

	bool cow_delete_;                // チュートリアル終了時やリセット時に、場にいる練習用の牛を消去するための消去フラグ
	bool skill_flag_{};              // スキル選択画面のシステム解説がすでに完了したかを記憶する進行スキップ防止フラグ
	bool fever_flag_{};              // フィーバータイムの発生演出がすでにトリガーされたかを確認する多重発生防止フラグ
};