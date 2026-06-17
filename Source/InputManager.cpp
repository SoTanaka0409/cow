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
 * 連続入力を防ぐため、押し始めの1フレームのみを検知する
 * [入力] KeyCode: DxLibのキーコード
 * [出力] 1: 押下開始フレーム、0: それ以外
 * [副作用] mDownBufferを更新
 */
int InputManager::CheckDownKey(int KeyCode)
{
    int result = 0;
    int keyState = CheckHitKey(KeyCode);
    
    // 長押しによる連続処理を防ぐためのトリガー判定
    if (mDownBuffer[KeyCode] == 0 && keyState == 1)
    {
        result = 1;
    }

    // 次フレームのトリガー判定用に状態を記録
    mDownBuffer[KeyCode] = keyState;
    return result;
}

/*
 * 押し続けを許容する通常のキー入力判定
 * [入力] KeyCode: DxLibのキーコード
 * [出力] 1: 押下中、0: 非押下
 * [副作用] なし
 */
int InputManager::CheckPressKey(int KeyCode)
{
    return CheckHitKey(KeyCode);
}
