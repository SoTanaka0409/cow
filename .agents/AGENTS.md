# コーディング規約

このプロジェクト（DxLib C++ ゲーム）で統一するコメントと命名のルール。

---

## 1. コメントルール

### 1-1. `.cpp` — 関数定義前（`/* */` ブロック）

関数定義の**直前**に必ず書く。

```cpp
/*
 * ～するため          ← この関数が存在する理由・目的を「〜するため」の形で書く
 * [入力] 引数名: 説明, 引数名: 説明   ← なければ「なし」
 * [出力] 戻り値の説明                 ← void なら「なし」
 * [副作用] 外部状態への影響
 */
void MyClass::MyFunction(int arg)
{
```

- 1行目は **「なぜ存在するか（目的・理由）」** を書く。「何をするか」ではない
- `[入力]` / `[出力]` / `[副作用]` の3項目は省略不可

### 1-2. `.h` — 関数宣言前（`//` 1〜2行）

```cpp
// [入力] arg: 説明 [出力] なし [副作用] ～が更新される
// 呼び出し側が守るべき注意点・設計意図（必要なときのみ2行目に追記）
void MyFunction(int arg);
```

- `[入力]` `[出力]` `[副作用]` を **1行にまとめる**
- 重要な注意点があれば2行目に追加する

### 1-3. インラインコメント（`//`）

```cpp
// テクスチャ本来の色合いで描画するため
SetUseLighting(false);
```

- **「何をしているか」ではなく「なぜそうしているか」** を書く
- メンバー変数の横にも簡潔な役割説明を書く

---

## 2. 命名規則

### 2-1. クラス名

**アッパーキャメルケース（PascalCase）**

```
CowManager  Object_Stage  SceneManager  StageLoader
```

- 単語の先頭を大文字にする
- アンダースコアは原則使わない（`Object_Stage` のように概念が複合している場合は例外）

### 2-2. 関数名（メンバー関数・グローバル関数）

**アッパーキャメルケース（PascalCase）**

```
Initialize()  SpawnCow()  LoadFromCSV()  SetColor()  GetPosition()
```

- 動詞 + 目的語の形にする
- getter は `Get〜()`、setter は `Set〜()` で統一する

### 2-3. メンバー変数

**スネークケース + 末尾アンダースコア**

```cpp
int mass_spawn_timer_;
Phase phase_;
int font_back_graph_;
Model* model_;
```

- すべて小文字、単語はアンダースコアで区切る
- **末尾に `_` を付ける**（ローカル変数と区別するため）

### 2-4. ローカル変数・引数

**スネークケース（末尾アンダースコアなし）**

```cpp
float rock_scale;
std::string csv_path;
int spawn_count;
```

- 引数も同じルール（例: `filename`, `init_pos`, `scatter_radius`）

### 2-5. 定数・enum 値

**先頭に `k` + アッパーキャメルケース**

```cpp
const std::string kCombo = "...";        // 定数
enum Phase { kNormal, kTornadoCrisis };  // enum 値
```

- 定数・列挙値はすべて `k` プレフィックスで始める

### 2-6. 構造体メンバー

**スネークケース（末尾アンダースコアなし）**

```cpp
struct CharacterParams {
    float speed;
    std::string model_path;
};
```

### 一覧表

| 対象 | スタイル | 例 |
|---|---|---|
| クラス名 | PascalCase | `CowManager`, `StageLoader` |
| 関数名 | PascalCase | `Initialize()`, `SpawnCow()` |
| メンバー変数 | snake_case + 末尾 `_` | `mass_spawn_timer_`, `model_` |
| ローカル変数・引数 | snake_case | `rock_scale`, `csv_path` |
| 定数・enum 値 | `k` + PascalCase | `kNormal`, `kCowDefault` |
| 構造体メンバー | snake_case | `model_path`, `death_time_height` |