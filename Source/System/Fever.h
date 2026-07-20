#pragma once

// 爽快感の提供とスコア獲得のチャンスを作るため、フィーバー状態を管理する
class Fever
{
public:
	Fever();
	~Fever();

	void Update();

	void AddGauge(int value);

	bool IsFever();

	void StartFever(class Player3D* player);

	void EndFever();

private:
	int drop_time_;       // 難易度およびスコア調整のための金の牛スポーン間隔
	int drop_count_;      // スポーン周期を管理するためのフレームカウンター
	int gauge_;         // UI表示やフィーバー発動判定に用いるゲージ蓄積値
	int timer_;         // フィーバー終了判定に用いる残り継続フレーム数
	bool is_fever_;      // 他クラスから状態を参照・分岐させるためのフラグ
	float player_status_;
	class Player3D* fever_player_; // 終了時にステータスを復元するための対象プレイヤーポインタ
};
