#pragma once
#include <vector>

class Collider;

// 当たり判定の漏れや二重判定を防ぐため、全Colliderを中央集約で管理するSingleton
// 空間分割は未実装のため、オブジェクト数が極端に多い場合はパフォーマンス影響に注意すること
class ColliderManager
{
public:
	ColliderManager();
	~ColliderManager();

	// [入力] なし [出力] なし [副作用] 衝突時コールバックの発火、破棄フラグ付きColliderの自動解放
	// 判定中の即時削除によるイテレータ無効化バグを防ぐため、総当たり判定と不要オブジェクトの削除を行う
	void Update();

	// [入力] なし [出力] なし [副作用] 描画バッファへの頂点データ追加
	// 本番環境でのパフォーマンス低下を防ぐため、デバッグビルド時のみ呼び出すこと
	void Draw();

	// [入力] なし [出力] ColliderManagerインスタンス [副作用] 初回のみインスタンス生成
	// 初期化の順序依存バグを避けるため、静的初期化子ではなく遅延評価で生成する
	static ColliderManager* GetInstance();

	// [入力] なし [出力] なし [副作用] instance_の破棄とメモリ解放
	// メモリリークを防ぐため、ゲームループ終了時やシーン破棄時に必ず1度だけ呼ぶこと
	static void Finalize();

	// [入力] 登録するCollider [出力] なし [副作用] collider_list_への要素追加
	// 判定漏れを防ぐため、Collider生成直後に必ず本関数経由で登録すること
	void AddCollider(Collider* collider);

	// [入力] なし [出力] なし [副作用] 全Colliderの破棄フラグ有効化とリストのクリア
	// シーン切り替え時に古い当たり判定が残存して誤検知するバグを防ぐために呼び出す
	void DeleteAllCollider();

	// [入力] なし [出力] なし [副作用] 破棄フラグが有効なColliderのリストからの除外
	// Update走査中のリスト書き換えによるクラッシュを防ぐため、一括で遅延削除を実行する処理
	void DeleteAllColliderIfNeeded();

	// [入力] 除外するCollider [出力] なし [副作用] リストからの要素除外
	// 内部でO(N)の検索コストがかかるため、パフォーマンスを考慮し原則は遅延削除を優先すること
	void RemoveCollider(Collider* collider);

	// [入力] なし [出力] collider_list_の参照 [副作用] なし
	// 外部からの走査が必要な特殊処理用。意図しない破棄を防ぐためリストの要素の直接削除は禁止
	std::vector<Collider*>& GetColliderList() { return collider_list_; }

private:
	// [入力] 判定対象のCollider2つ [出力] 衝突有無 [副作用] なし
	// 判定呼び出し順序（AとB、BとA）によって結果が変わるバグを防ぐため内部で正規化して処理する
	bool CheckCollision(Collider* colA, Collider* colB);

	std::vector<Collider*> collider_list_;

	static ColliderManager* instance_;
};
