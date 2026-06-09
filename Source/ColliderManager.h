#pragma once
#include <list>

class Collider;

// ゲーム内のすべてのコライダーオブジェクトを管理し、相互の当たり判定を一括処理するマネージャークラス
class ColliderManager
{
public:
	ColliderManager();
	~ColliderManager();

	/*
	 * @brief 毎フレーム登録されたコライダー同士の交差判定を総当たりで実行する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各コライダーの当たり判定・イベント発行の呼び出し、不要コライダーの自動クリーンアップ
	 */
	void Update();

	/*
	 * @brief （デバッグ表示用）各コライダーの外形を描画する（現在処理なし）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw();

	/*
	 * @brief シングルトンインスタンスを取得する（存在しない場合は新規生成）
	 * [入力] なし
	 * [出力] ColliderManagerクラスの静的インスタンスへのポインタ
	 * [副作用] なし
	 */
	static ColliderManager* GetInstance()
	{
		if (Instance == nullptr)
		{
			Instance = new ColliderManager();
		}
		return Instance;
	}

	/*
	 * @brief シングルトンインスタンスを安全に解放し、後片付けを行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 静的インスタンス(Instance)のdelete
	 */
	static void Finalize()
	{
		if (Instance != nullptr)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

public:
	/*
	 * @brief コライダーを管理リストに追加する
	 * [入力] Collider: 追加するコライダーオブジェクトへのポインタ
	 * [出力] なし
	 * [副作用] mColliderListへの追加
	 */
	void AddCollider(Collider* Collider);

	/*
	 * @brief 登録されているすべてのコライダーの削除フラグを立て、管理リストを空にする
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 全登録コライダーのSetDeleteFlag(true)実行、およびmColliderListのクリア
	 */
	void DeleteAllCollider();

	/*
	 * @brief 削除フラグ(mbDeleteFlag)が真に設定されているコライダーを管理リストから自動除外する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mColliderListからの削除
	 */
	void DeleteAllColliderIfNeeded();

	/*
	 * @brief 特定のコライダーを管理リストから手動で除外する
	 * [入力] collider: 除外するコライダーオブジェクトへのポインタ
	 * [出力] なし
	 * [副作用] mColliderListからの特定コライダーの削除
	 */
	void RemoveCollider(Collider* collider);
	
	std::list<Collider*>& GetColliderList() { return mColliderList; }

private:
	std::list<Collider*> mColliderList; // 判定対象となるアクティブな全コライダーの管理リスト

	static ColliderManager* Instance;  // シングルトン管理用の静的インスタンスポインタ
};
