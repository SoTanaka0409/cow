#pragma once

#include <vector>
#include <map>
#include <utility>
#include <algorithm>

/// @brief 設計ルール：大量に発生・消失を繰り返すキャラ（牛・動物等）の動的メモリ確保（new/delete）による遅延を防ぐオブジェクトプールテンプレート
template <typename TMove, typename TTag>
class CreatureManager
{
public:
	CreatureManager()
	{
		/// @brief 実行中の動的配列の拡張（再確保）に伴うスパイクを防ぐため、初期領域を最大想定数（500）確保しておく
		mCreatures.reserve(500);
	}

	virtual ~CreatureManager()
	{
		/// @brief 稼働中のキャラはObjectManager側が一括破棄するため、ここでdeleteすると多重解放（二重解放バグ）になるためクリアのみ行う
		mCreatures.clear();
		for (auto& pair : pools_)
		{
			for (auto creature : pair.second)
			{
				delete creature;
			}
		}
		pools_.clear();
	}

	/// @brief 管理下にあるすべての生存キャラクターのUpdate呼び出し、および非アクティブ化したキャラのプール回収
	void Update()
	{
		for (auto creature : mCreatures)
		{
			creature->Update();
		}
		Erase();
	}

	void Draw()
	{
	}

	/// @brief 削除要求フラグが立ったキャラを生存リストから除外し、対応する識別タグのオブジェクトプール（待機リスト）へ返却する
	void Erase()
	{
		if (!mCreatures.empty())
		{
			mCreatures.erase(
				std::remove_if(mCreatures.begin(), mCreatures.end(), [this](auto creature) {
					/// @details CowMoveとAnimalMoveはいずれもCharacterMoveを継承しており削除フラグの共通監視が可能
					if (creature->GetCharacterDelete())
					{
						creature->Deactivate();

						TTag tag = GetTag(creature);
						pools_[tag].push_back(creature);
						return true;
					}
					return false;
					}),
				mCreatures.end()
			);
		}
	}

protected:
	/// @brief 値を取得する
	/// @param creature タグを取得したいキャラオブジェクトのポインタ
	/// @return 個別アクターを特定するための識別タグ（enum等）
	virtual TTag GetTag(TMove* creature) = 0;

	/// @param tag 識別タグ
	/// @param spawnPos 出現座標
	/// @param scale 拡縮率
	/// @param args 新規生成時の可変長コンストラクタ引数
	/// @return 再利用または新規生成された具象キャラオブジェクトのキャスト済みポインタ
	/// @details プールからの取得と各種パラメータ（座標・スケール）の初期化、またはメモリの動的確保（new）
	template <typename TConcrete, typename... Args>
	TConcrete* SpawnAndInit(TTag tag, VECTOR spawnPos, float scale, Args&&... args)
	{
		TConcrete* creature = nullptr;
		/// @details プールに休止オブジェクトがある場合はメモリ確保をバイパスし、Resetを呼んで初期値に戻して再利用する
		if (!pools_[tag].empty())
		{
			creature = static_cast<TConcrete*>(pools_[tag].back());
			pools_[tag].pop_back();
			creature->Reset(spawnPos);
		}
		else
		{
			creature = new TConcrete(std::forward<Args>(args)...);
		}
		creature->SetScale(scale);
		mCreatures.push_back(creature);
		return creature;
	}

protected:
	std::vector<TMove*> mCreatures;             ///< 現在ステージ上で稼働しており、毎フレームの更新処理が走る生存キャラクターリスト
	std::map<TTag, std::vector<TMove*>> pools_; ///< メモリ再確保を回避するために、待機（非アクティブ）状態のアクターをプールしておく連想配列
};
