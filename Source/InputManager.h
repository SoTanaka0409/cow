#pragma once

// キーボードからの入力状態（単発押し、押し続けなど）を判定するクラス
class InputManager
{
public:
	InputManager();
	~InputManager();

	/*
	 * @brief 指定したキーが今フレームで新しく押されたかを判定する（トリガー入力）
	 * [入力] KeyCode: 判定対象のキーコード（KEY_INPUT_...）
	 * [出力] 1: 新しく押された、0: それ以外
	 * [副作用] mDownBuffer内のキー状態バッファを更新
	 */
	static int CheckDownKey(int KeyCode);

	/*
	 * @brief 指定したキーが現在押されているかを判定する（プレス入力）
	 * [入力] KeyCode: 判定対象のキーコード
	 * [出力] 1: 押されている、0: 押されていない
	 * [副作用] なし
	 */
	static int CheckPressKey(int KeyCode);

private:
	static int mDownBuffer[256]; // キーが前フレームで押されていたかを追跡するためのバッファ
};
