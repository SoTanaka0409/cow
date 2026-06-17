import codecs
import os

# --- 1. ObjectManager.cpp ---
obj_mgr_cpp = """#include "ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"

/*
 * @brief オブジェクト管理クラスの初期化
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
ObjectManager::ObjectManager()
{
}

/*
 * @brief オブジェクト管理クラスの終了処理
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
ObjectManager::~ObjectManager()
{
}

/*
 * @brief 登録された全オブジェクトの更新とカメラ距離の算出を行う
 * [入力] なし
 * [出力] なし
 * [副作用] Zソート用カメラ距離の更新、コライダーの一括処理実行
 */
void ObjectManager::Update()
{
\tfor (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
\t{
\t\t(*itr)->Update();
\t}

\tfor (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
\t{
\t\t(*itr)->Update();
\t}

\t// 半透明描画時のZソートで必要となるため、カメラとの距離を算出する
\tfor (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
\t{
\t\tVECTOR cameraPos = Master::mpCamera->GetPosition();
\t\tVECTOR objPos = (*itr)->GetPosition();
\t\t(*itr)->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
\t}

\t// 座標更新が全て完了した後に衝突判定を行うためここで一括処理する
\tColliderManager::GetInstance()->Update();
}

/*
 * @brief 登録された全オブジェクトの描画を行う
 * [入力] なし
 * [出力] なし
 * [副作用] 描画フラグが有効なオブジェクトとコライダーの描画命令発行
 */
void ObjectManager::Draw()
{
\tfor (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
\t{
\t\tif ((*itr)->IsDrawFlag())
\t\t{
\t\t\t(*itr)->Draw();
\t\t}
\t}
\tColliderManager::GetInstance()->Draw();

\tfor (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
\t{
\t\tif ((*itr)->IsDrawFlag())
\t\t{
\t\t\t(*itr)->Draw();
\t\t}
\t}
}

/*
 * @brief 3Dオブジェクトをリストとキャッシュに追加する
 * [入力] object3D: 追加する3Dオブジェクトのポインタ
 * [出力] なし
 * [副作用] mObject3DListおよびmTagCache3Dへの要素追加
 */
void ObjectManager::AddObject(Object3D* object3D)
{
\tmObject3DList.push_back(object3D);
\tmTagCache3D[object3D->GetTag()].push_back(object3D);
}

/*
 * @brief 3Dオブジェクトをリストから削除する（メモリ解放はしない）
 * [入力] object3D: 削除対象の3Dオブジェクトのポインタ
 * [出力] なし
 * [副作用] mObject3DListからの要素削除とキャッシュの再構築
 */
void ObjectManager::RemoveObjectNoDelete(Object3D* object3D)
{
\tmObject3DList.remove(object3D);
\tRebuildTagCache3D();
}

/*
 * @brief 全ての3Dオブジェクトに削除フラグを立て、実際の削除処理を呼ぶ
 * [入力] なし
 * [出力] なし
 * [副作用] 全3Dオブジェクトの削除とメモリ解放
 */
void ObjectManager::DeleteAll3D()
{
\tif (mObject3DList.empty()) return;
\tfor (auto itr = mObject3DList.begin(); itr != mObject3DList.end();)
\t{
\t\t(*itr)->SetDeleteFlag(true);
\t\titr++;
\t}

\tDeleteAll3DIfNeeded();
}

/*
 * @brief 指定されたタグを持つ3Dオブジェクトを1つ取得する
 * [入力] tag: 検索対象のタグ
 * [出力] 条件に合致する最初のObject3Dポインタ（見つからない場合はnullptr）
 * [副作用] なし
 */
Object3D* ObjectManager::GetObject3DByTag(Object3D::Tag3D tag)
{
\tauto itr = std::find_if(
\t\tmObject3DList.begin(),
\t\tmObject3DList.end(),
\t\t[&](Object3D* obj) { return obj->GetTag() == tag; }
\t);
\tif (itr != mObject3DList.end())
\t{
\t\treturn (*itr);
\t}
\treturn nullptr;
}

/*
 * @brief 指定されたタグを持つ3Dオブジェクトのリストを取得する
 * [入力] tag: 検索対象のタグ
 * [出力] 条件に合致するObject3Dポインタのベクタ（キャッシュから参照）
 * [副作用] なし
 */
const std::vector<Object3D*>& ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
\treturn mTagCache3D[tag];
}

/*
 * @brief 削除フラグが立っている3Dオブジェクトをメモリ解放し、リストから除外する
 * [入力] なし
 * [出力] なし
 * [副作用] 不要オブジェクトのメモリ解放とキャッシュの再構築
 */
void ObjectManager::DeleteAll3DIfNeeded()
{
\tbool isDeleted = false;
\tif (mObject3DList.empty()) return;
\tfor (auto itr = mObject3DList.begin(); itr != mObject3DList.end();)
\t{
\t\tif ((*itr)->IsDeleteFlag())
\t\t{
\t\t\tObject3D* temp = *itr;
\t\t\titr = mObject3DList.erase(itr);
\t\t\tdelete temp;
\t\t\tisDeleted = true;
\t\t}
\t\telse
\t\t{
\t\t\titr++;
\t\t}
\t}
\t// 毎フレームのキャッシュ構築負荷を避けるため、要素変更時のみ再構築する
\tif (isDeleted) {
\t\tRebuildTagCache3D();
\t}
}

/*
 * @brief 2Dオブジェクトをリストとキャッシュに追加する
 * [入力] object2D: 追加する2Dオブジェクトのポインタ
 * [出力] なし
 * [副作用] mObject2DListおよびmTagCache2Dへの要素追加
 */
void ObjectManager::AddObject(Object2D* object2D)
{
\tmObject2DList.push_back(object2D);
\tmTagCache2D[object2D->GetTag()].push_back(object2D);
}

/*
 * @brief 全ての2Dオブジェクトをリストから削除し、メモリ解放する
 * [入力] なし
 * [出力] なし
 * [副作用] 全2Dオブジェクトの削除とキャッシュのクリア
 */
void ObjectManager::DeleteAll2D()
{
\tfor (auto itr = mObject2DList.begin(); itr != mObject2DList.end();)
\t{
\t\tObject2D* temp = *itr;
\t\titr = mObject2DList.erase(itr);
\t\tdelete temp;
\t}
\tmTagCache2D.clear();
}

/*
 * @brief 削除フラグが立っている2Dオブジェクトをメモリ解放し、リストから除外する
 * [入力] なし
 * [出力] なし
 * [副作用] 不要オブジェクトのメモリ解放とキャッシュの再構築
 */
void ObjectManager::DeleteAll2DIfNeeded()
{
\tbool isDeleted = false;
\tfor (auto itr = mObject2DList.begin(); itr != mObject2DList.end();)
\t{
\t\tif ((*itr)->IsDeleteFlag())
\t\t{
\t\t\tObject2D* temp = *itr;
\t\t\titr = mObject2DList.erase(itr);
\t\t\tdelete temp;
\t\t\tisDeleted = true;
\t\t}
\t\telse
\t\t{
\t\t\titr++;
\t\t}
\t}
\t// 毎フレームのキャッシュ構築負荷を避けるため、要素変更時のみ再構築する
\tif (isDeleted) {
\t\tRebuildTagCache2D();
\t}
}

/*
 * @brief 指定されたタグを持つ2Dオブジェクトを1つ取得する
 * [入力] tag: 検索対象のタグ
 * [出力] 条件に合致する最初のObject2Dポインタ（見つからない場合はnullptr）
 * [副作用] なし
 */
Object2D* ObjectManager::GetObject2DByTag(Object2D::Tag2D tag)
{
\tauto itr = std::find_if(
\t\tmObject2DList.begin(),
\t\tmObject2DList.end(),
\t\t[&](Object2D* obj) { return obj->GetTag() == tag; }
\t);

\tif (itr != mObject2DList.end())
\t{
\t\treturn (*itr);
\t}
\treturn nullptr;
}

/*
 * @brief 指定されたタグを持つ2Dオブジェクトのリストを取得する
 * [入力] tag: 検索対象のタグ
 * [出力] 条件に合致するObject2Dポインタのベクタ（キャッシュから参照）
 * [副作用] なし
 */
const std::vector<Object2D*>& ObjectManager::GetObject2DListByTag(Object2D::Tag2D tag)
{
\treturn mTagCache2D[tag];
}

/*
 * @brief 3Dオブジェクト検索用のタグキャッシュを全構築する
 * [入力] なし
 * [出力] なし
 * [副作用] mTagCache3Dのクリアと要素の再登録
 */
void ObjectManager::RebuildTagCache3D()
{
\tmTagCache3D.clear();
\tfor (auto obj : mObject3DList) {
\t\tmTagCache3D[obj->GetTag()].push_back(obj);
\t}
}

/*
 * @brief 2Dオブジェクト検索用のタグキャッシュを全構築する
 * [入力] なし
 * [出力] なし
 * [副作用] mTagCache2Dのクリアと要素の再登録
 */
void ObjectManager::RebuildTagCache2D()
{
\tmTagCache2D.clear();
\tfor (auto obj : mObject2DList) {
\t\tmTagCache2D[obj->GetTag()].push_back(obj);
\t}
}
"""

# --- 2. CowManager.h ---
cow_mgr_h = """#pragma once
#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "CowMove.h"

// ステージ上の牛たちの動的生成、更新、タグ判定、上限管理、解放などを一括管理するクラス
class CowManager
{
public:
\tCowManager();
\t~CowManager();

\t/*
\t * @brief 指定された種類の牛をランダムな位置に複数生成する（最大30匹制限あり）
\t * [入力] filename: モデルのファイルパス, pos: 出現中心基準座標, scale: モデルの拡大率, tag: 牛の種類識別タグ, count: 生成個数, mfever: フィーバー中かどうか
\t * [出力] なし
\t * [副作用] 動的メモリ確保された牛オブジェクトが管理リストに追加されるか、プールから復帰する
\t */
\tvoid SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::Tag_cow tag, int count, bool mfever = false);

\t/*
\t * @brief 管理している全ての牛の更新および不要な牛のリスト整理を行う
\t * [入力] なし
\t * [出力] なし
\t * [副作用] 各牛のUpdate実行と、削除フラグの立った牛のプール返却
\t */
\tvoid Update();

\t/*
\t * @brief 管理している牛の描画処理を行う
\t * [入力] なし
\t * [出力] なし
\t * [副作用] なし
\t */
\tvoid Draw();

\t/*
\t * @brief 削除フラグ(mbDeleteFlag)が立っている牛オブジェクトを管理リストから除外しプールへ返す
\t * [入力] なし
\t * [出力] なし
\t * [副作用] リストからのポインタ除外、プールへの追加
\t */
\tvoid EraseCow();

private:
\tstd::vector<CowMove*>mCows;         // 生成された牛オブジェクトのポインタ配列
\tstd::map<CowMove::Tag_cow, std::vector<CowMove*>> mPools; // オブジェクトプール
};
"""

# --- 3. CowManager.cpp ---
cow_mgr_cpp = """#include "ServiceLocator.h"
#include "CowManager.h"
#include "CowMove.h"
#include "Player3D.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Master.h"
#include "Cow.h"
#include "Cow_2.h"
#include "Cow_Tutorial.h"
#include "Cow_gold.h"
#include "CapsuleCollider.h"

/*
 * @brief 管理オブジェクトの初期化を行う
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
CowManager::CowManager()
{
}

/*
 * @brief 管理リストおよびオブジェクトプールの牛を全解放する
 * [入力] なし
 * [出力] なし
 * [副作用] 牛オブジェクトのメモリ解放
 */
CowManager::~CowManager()
{
\tmCows.clear();
\tfor (auto& pair : mPools)
\t{
\t\tfor (auto cow : pair.second)
\t\t{
\t\t\tdelete cow;
\t\t}
\t}
\tmPools.clear();
}

/*
 * @brief 指定された種類の牛を生成またはプールから再利用して配置する
 * [入力] filename: モデルファイル, pos: 出現基準座標, scale: 拡大率, tag: 牛のタグ, count: 生成数, mfever: フィーバーフラグ
 * [出力] なし
 * [副作用] 牛のメモリ確保およびmCowsへの追加、またはプールからの取り出し
 */
void CowManager::SpawnCow(std::string filename, VECTOR pos, float scale, CowMove::Tag_cow tag, int count, bool mfever)
{
\tfor (int i = 0; i < count; i++)
\t{
\t\t// パフォーマンス維持のため、同時出現数を最大30匹に制限する
\t\tif (mCows.size() >= 30)
\t\t{
\t\t\tif (tag == CowMove::Cow_gold)
\t\t\t{
\t\t\t\t// 金の牛を確実に出現させるため、プレイヤーから最も遠い普通の牛を優先して破棄し枠を空ける
\t\t\t\tbool erased = false;
\t\t\t\tfloat maxDistSq = -1.0f;
\t\t\t\tauto furthestIt = mCows.end();
\t\t\t\t
\t\t\t\tVECTOR playerPos = Master::mpCamera->GetPosition();

\t\t\t\tfor (auto it = mCows.begin(); it != mCows.end(); ++it)
\t\t\t\t{
\t\t\t\t\tif ((*it)->GetTag_cow() != CowMove::Cow_gold)
\t\t\t\t\t{
\t\t\t\t\t\tVECTOR cowPos = (*it)->GetPosition();
\t\t\t\t\t\tfloat dx = cowPos.x - playerPos.x;
\t\t\t\t\t\tfloat dy = cowPos.y - playerPos.y;
\t\t\t\t\t\tfloat dz = cowPos.z - playerPos.z;
\t\t\t\t\t\tfloat distSq = dx * dx + dy * dy + dz * dz;

\t\t\t\t\t\tif (distSq > maxDistSq)
\t\t\t\t\t\t{
\t\t\t\t\t\t\tmaxDistSq = distSq;
\t\t\t\t\t\t\tfurthestIt = it;
\t\t\t\t\t\t}
\t\t\t\t\t}
\t\t\t\t}

\t\t\t\tif (furthestIt != mCows.end())
\t\t\t\t{
\t\t\t\t\t// 既に削除フラグが立っている牛などはObjectManager側で消される
\t\t\t\t\t(*furthestIt)->Die(DEATH_LIMIT);
\t\t\t\t\tauto cow = *furthestIt;
\t\t\t\t\tcow->Deactivate();
\t\t\t\t\tmPools[cow->GetTag_cow()].push_back(cow);
\t\t\t\t\tmCows.erase(furthestIt);
\t\t\t\t\terased = true;
\t\t\t\t}
\t\t\t\telse if (!mCows.empty())
\t\t\t\t{
\t\t\t\t\t// 全ての牛が画面内などの場合、一番古いものの削除フラグを立ててリストから除外する
\t\t\t\t\tmCows.front()->Die(DEATH_LIMIT);
\t\t\t\t\tauto cow = mCows.front();
\t\t\t\t\tcow->Deactivate();
\t\t\t\t\tmPools[cow->GetTag_cow()].push_back(cow);
\t\t\t\t\tmCows.erase(mCows.begin());
\t\t\t\t\terased = true;
\t\t\t\t}

\t\t\t\tif (!erased) break;
\t\t\t}
\t\t\telse
\t\t\t{
\t\t\t\tbreak;
\t\t\t}
\t\t}

\t\tfloat randX = (float)(GetRand(pos.x) - pos.x / 2);
\t\tfloat randZ = (float)(GetRand(pos.z) - pos.z / 2);
\t\tVECTOR spawnPos = VGet(randX, pos.y, randZ);

\t\tif (tag == CowMove::Cow_1)
\t\t{
\t\t\tif (!mPools[tag].empty())
\t\t\t{
\t\t\t\tauto cow = mPools[tag].back();
\t\t\t\tmPools[tag].pop_back();
\t\t\t\tcow->Reset(spawnPos);
\t\t\t\tcow->SetScale(scale);
\t\t\t\tmCows.push_back(cow);
\t\t\t}
\t\t\telse
\t\t\t{
\t\t\t\tauto newCow = new Cow(filename, spawnPos, 1.0f);
\t\t\t\tnewCow->SetScale(scale);
\t\t\t\tmCows.push_back(newCow);
\t\t\t}
\t\t}
\t\telse if (tag == CowMove::Cow_2)
\t\t{
\t\t\tif (!mPools[tag].empty())
\t\t\t{
\t\t\t\tauto cow = mPools[tag].back();
\t\t\t\tmPools[tag].pop_back();
\t\t\t\tcow->Reset(spawnPos);
\t\t\t\tcow->SetScale(scale);
\t\t\t\tmCows.push_back(cow);
\t\t\t}
\t\t\telse
\t\t\t{
\t\t\t\tauto newCow = new Cow_2(filename, spawnPos);
\t\t\t\tnewCow->SetScale(scale);
\t\t\t\tmCows.push_back(newCow);
\t\t\t}
\t\t}
\t\telse if (tag == CowMove::Cow_gold)
\t\t{
\t\t\tauto feverMode = mfever ? Cow_gold::fever : Cow_gold::Nofever;
\t\t\tif (!mPools[tag].empty())
\t\t\t{
\t\t\t\tauto cow = dynamic_cast<Cow_gold*>(mPools[tag].back());
\t\t\t\tmPools[tag].pop_back();
\t\t\t\tif (cow) cow->SetFever(feverMode);
\t\t\t\tcow->Reset(spawnPos);
\t\t\t\tcow->SetScale(scale);
\t\t\t\tmCows.push_back(cow);
\t\t\t}
\t\t\telse
\t\t\t{
\t\t\t\tauto newCow = new Cow_gold(filename, spawnPos, feverMode);
\t\t\t\tnewCow->SetScale(scale);
\t\t\t\tmCows.push_back(newCow);
\t\t\t}
\t\t}
\t\telse if (tag == CowMove::Cow_T)
\t\t{
\t\t\tif (!mPools[tag].empty())
\t\t\t{
\t\t\t\tauto cow = mPools[tag].back();
\t\t\t\tmPools[tag].pop_back();
\t\t\t\tcow->Reset(spawnPos);
\t\t\t\tcow->SetScale(scale);
\t\t\t\tmCows.push_back(cow);
\t\t\t}
\t\t\telse
\t\t\t{
\t\t\t\tauto newCow = new Cow_Tutorial(filename, spawnPos);
\t\t\t\tnewCow->SetScale(scale);
\t\t\t\tmCows.push_back(newCow);
\t\t\t}
\t\t}
\t}
}

/*
 * @brief 全ての牛の更新処理と不要な牛の削除（プール返却）を行う
 * [入力] なし
 * [出力] なし
 * [副作用] 各牛のUpdate実行とEraseCowの実行
 */
void CowManager::Update()
{
\tfor (auto cow : mCows)
\t{
\t\tcow->Update();
\t}
\tEraseCow();
}

/*
 * @brief 全ての牛の描画を行う（現在は描画処理を外部で行っているため空）
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void CowManager::Draw()
{
}

/*
 * @brief 削除フラグが立っている牛を非アクティブ化しプールに返却する
 * [入力] なし
 * [出力] なし
 * [副作用] mCowsからの削除およびmPoolsへの追加
 */
void CowManager::EraseCow()
{
\tif (!mCows.empty())
\t{
\t\tfor (auto it = mCows.begin(); it != mCows.end();)
\t\t{
\t\t\tif ((*it)->GetCowDelete())
\t\t\t{
\t\t\t\tauto cow = *it;
\t\t\t\tcow->Deactivate();
\t\t\t\tmPools[cow->GetTag_cow()].push_back(cow);
\t\t\t\tit = mCows.erase(it);
\t\t\t}
\t\t\telse
\t\t\t{
\t\t\t\tit++;
\t\t\t}
\t\t}
\t}
}
"""

# --- 4. AnimalManager.h ---
animal_mgr_h = """#pragma once

#include <vector>
#include <string>
#include <map>
#include "DxLib.h"
#include "AnimalMove.h"

// ステージ上の一般アニマル（牛以外）の動的生成、更新、タグ判定、解放などを一括管理するクラス
class AnimalManager
{
public:
\tAnimalManager();
\t~AnimalManager();

\t/*
\t * @brief 指定された種類の動物をランダムな位置に複数生成する
\t * [入力] filename: モデルのファイルパス, pos: 出現中心基準座標, scale: モデルの拡大率, tag: 動物の種類識別タグ, count: 生成個数
\t * [出力] なし
\t * [副作用] 動的メモリ確保された動物オブジェクトが管理リストに追加される、またはプールから復帰する
\t */
\tvoid SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::Tag_animal tag, int count);

\t/*
\t * @brief 管理している全ての動物の更新および不要な動物の解放処理を行う
\t * [入力] なし
\t * [出力] なし
\t * [副作用] 各動物のUpdate実行と、削除フラグの立った動物のプール返却
\t */
\tvoid Update();

\t/*
\t * @brief 管理している動物の描画処理を行う
\t * [入力] なし
\t * [出力] なし
\t * [副作用] なし
\t */
\tvoid Draw();

\t/*
\t * @brief 削除フラグ(mbDeleteFlag)が立っている動物オブジェクトを管理リストから除外しプールへ返す
\t * [入力] なし
\t * [出力] なし
\t * [副作用] リストからのポインタ除外、プールへの追加
\t */
\tvoid EraseAnimal();

private:
\tstd::vector<AnimalMove*> mAnimals;  // 生成された動物オブジェクトのポインタ配列
\tstd::map<AnimalMove::Tag_animal, std::vector<AnimalMove*>> mPools; // オブジェクトプール
};
"""

# --- 5. AnimalManager.cpp ---
animal_mgr_cpp = """#include "ServiceLocator.h"
#include "AnimalManager.h"
#include "AnimalMove.h"
#include "Player3D.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Master.h"
#include "Animal.h"

/*
 * @brief 管理オブジェクトの初期化を行う
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
AnimalManager::AnimalManager()
{
}

/*
 * @brief 管理リストおよびオブジェクトプールの動物を全解放する
 * [入力] なし
 * [出力] なし
 * [副作用] 動物オブジェクトのメモリ解放
 */
AnimalManager::~AnimalManager()
{
\tmAnimals.clear();
\tfor (auto& pair : mPools)
\t{
\t\tfor (auto animal : pair.second)
\t\t{
\t\t\tdelete animal;
\t\t}
\t}
\tmPools.clear();
}

/*
 * @brief 指定された種類の動物を生成またはプールから再利用して配置する
 * [入力] filename: モデルファイル, pos: 出現基準座標, scale: 拡大率, tag: 動物のタグ, count: 生成数
 * [出力] なし
 * [副作用] 動物のメモリ確保およびmAnimalsへの追加、またはプールからの取り出し
 */
void AnimalManager::SpawnAnimal(std::string filename, VECTOR pos, float scale, AnimalMove::Tag_animal tag, int count)
{
\tfor (int i = 0; i < count; i++)
\t{
\t\tfloat randX = (float)(GetRand(pos.x) - pos.x / 2);
\t\tfloat randZ = (float)(GetRand(pos.z) - pos.z / 2);
\t\tVECTOR spawnPos = VGet(randX, 0.0f, randZ);
\t\tif (tag == AnimalMove::Animal_1)
\t\t{
\t\t\tif (!mPools[tag].empty())
\t\t\t{
\t\t\t\tauto animal = mPools[tag].back();
\t\t\t\tmPools[tag].pop_back();
\t\t\t\tanimal->Reset(spawnPos);
\t\t\t\tanimal->SetScale(scale);
\t\t\t\tmAnimals.push_back(animal);
\t\t\t}
\t\t\telse
\t\t\t{
\t\t\t\tauto newAnimal = new Animal(filename, spawnPos);
\t\t\t\tnewAnimal->SetScale(scale);
\t\t\t\tmAnimals.push_back(newAnimal);
\t\t\t}
\t\t}
\t}
}

/*
 * @brief 全ての動物の更新処理と不要な動物の削除（プール返却）を行う
 * [入力] なし
 * [出力] なし
 * [副作用] 各動物のUpdate実行とEraseAnimalの実行
 */
void AnimalManager::Update()
{
\tfor (auto animal : mAnimals)
\t{
\t\tanimal->Update();
\t}
\tEraseAnimal();
}

/*
 * @brief 全ての動物の描画を行う（現在は描画処理を外部で行っているため空）
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
void AnimalManager::Draw()
{
}

/*
 * @brief 削除フラグが立っている動物を非アクティブ化しプールに返却する
 * [入力] なし
 * [出力] なし
 * [副作用] mAnimalsからの削除およびmPoolsへの追加
 */
void AnimalManager::EraseAnimal()
{
\tif (!mAnimals.empty())
\t{
\t\tfor (auto it = mAnimals.begin(); it != mAnimals.end();)
\t\t{
\t\t\tif ((*it)->GetCharacterDelete())
\t\t\t{
\t\t\t\tauto animal = *it;
\t\t\t\tanimal->Deactivate();
\t\t\t\tmPools[animal->GetTag_animal()].push_back(animal);
\t\t\t\tit = mAnimals.erase(it);
\t\t\t}
\t\t\telse
\t\t\t{
\t\t\t\tit++;
\t\t\t}
\t\t}
\t}
}
"""

base_dir = r"c:\Users\student\Desktop\programing\cow\Source"

files = [
    ("ObjectManager.cpp", obj_mgr_cpp),
    ("CowManager.h", cow_mgr_h),
    ("CowManager.cpp", cow_mgr_cpp),
    ("AnimalManager.h", animal_mgr_h),
    ("AnimalManager.cpp", animal_mgr_cpp),
]

for filename, content in files:
    path = os.path.join(base_dir, filename)
    with codecs.open(path, "w", "utf-8-sig") as f:
        f.write(content)
    print(f"Updated {filename}")
