#pragma once
#include <vector>

class Collider;

/// @brief 当たり判定の漏れや二重判定を防ぐため、全Colliderを中央集約で管理するSingleton
/// @details 空間分割は未実装のため、オブジェクト数が極端に多い場合はパフォーマンス影響に注意すること
class ColliderManager
{
public:
	ColliderManager();
	~ColliderManager();

	/// @brief 判定中の即時削除によるイテレータ無効化バグを防ぐため、総当たり判定と不要オブジェクトの削除を行う
	/// @details なし [出力] なし [副作用] 衝突時コールバックの発火
	/// @details 破棄フラグ付きColliderの自動解放
	void Update();

	/// @brief 本番環境でのパフォーマンス低下を防ぐため、デバッグビルド時のみ呼び出すこと
	/// @details なし [出力] なし [副作用] 描画バッファへの頂点データ追加
	void Draw();

	/// @brief 初期化の順序依存バグを避けるため、静的初期化子ではなく遅延評価で生成する
	/// @details なし [出力] ColliderManagerインスタンス [副作用] 初回のみインスタンス生成
	static ColliderManager* GetInstance();

	/// @brief メモリリークを防ぐため、ゲームループ終了時やシーン破棄時に必ず1度だけ呼ぶこと
	/// @details なし [出力] なし [副作用] instance_の破棄とメモリ解放
	static void Finalize();

	/// @brief 判定漏れを防ぐため、Collider生成直後に必ず本関数経由で登録すること
	/// @details 登録するCollider [出力] なし [副作用] collider_list_への要素追加
	void AddCollider(Collider* collider);

	/// @brief シーン切り替え時に古い当たり判定が残存して誤検知するバグを防ぐために呼び出す
	/// @details なし [出力] なし [副作用] 全Colliderの破棄フラグ有効化とリストのクリア
	void DeleteAllCollider();

	/// @brief Update走査中のリスト書き換えによるクラッシュを防ぐため、一括で遅延削除を実行する処理
	/// @details なし [出力] なし [副作用] 破棄フラグが有効なColliderのリストからの除外
	void DeleteAllColliderIfNeeded();

	/// @brief 内部でO(N)の検索コストがかかるため、パフォーマンスを考慮し原則は遅延削除を優先すること
	/// @details 除外するCollider [出力] なし [副作用] リストからの要素除外
	void RemoveCollider(Collider* collider);

	/// @brief 外部からの走査が必要な特殊処理用。意図しない破棄を防ぐためリストの要素の直接削除は禁止
	/// @details なし [出力] collider_list_の参照 [副作用] なし
	std::vector<Collider*>& GetColliderList() { return collider_list_; }

private:
	/// @brief 判定呼び出し順序（AとB、BとA）によって結果が変わるバグを防ぐため内部で正規化して処理する
	/// @details 判定対象のCollider2つ [出力] 衝突有無 [副作用] なし
	bool CheckCollision(Collider* colA, Collider* colB);

	std::vector<Collider*> collider_list_; ///< 当たり判定の管理に使用する情報

	static ColliderManager* instance_; ///< 内部状態を管理する値
};
