#include"InputManager.h"
#include"DxLib.h"

int InputManager::mDownBuffer[256] = { 0 };

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

/*
 * @brief 指定したキーが今フレームで新しく押されたかを判定する
 * [入力] KeyCode: 判定対象のキーコード（KEY_INPUT_...）
 * [出力] 1: 今フレームで新しく押された、0: それ以外
 * [副作用] mDownBuffer[KeyCode] の値を現在のキー状態で更新
 */
int InputManager::CheckDownKey(int KeyCode)
{
	int result = 0;
	int keyState = CheckHitKey(KeyCode);
	
	// 前フレームで未入力かつ現フレームで入力ありの場合のみ「新規入力」とみなす
	if (mDownBuffer[KeyCode] == 0 && keyState == 1)
	{
		result = 1;
	}

	mDownBuffer[KeyCode] = keyState; // 現フレームの状態を保存して次回判定へ引き継ぐ
	return result;
}

/*
 * @brief 指定したキーが現在押されているかを判定する
 * [入力] KeyCode: 判定対象のキーコード
 * [出力] 1: 押されている、0: 押されていない
 * [副作用] なし
 */
int InputManager::CheckPressKey(int KeyCode)
{
	return CheckHitKey(KeyCode);
}
