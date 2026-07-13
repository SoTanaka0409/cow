#include"InputManager.h"
#include"DxLib.h"
int InputManager::down_buffer_[256] = { 0 };
InputManager::InputManager()
{
}
InputManager::~InputManager()
{
}
/*
 * 連続入力を防ぎ、押下開始フレームのみを検知する
 * [入力] KeyCode: DxLibのキーコード
 * [出力] 1: 押下開始フレーム、0: それ以外
 * [副作用] down_buffer_の更新
 */
int InputManager::CheckDownKey(int KeyCode)
{
    int result = 0;
    int keyState = CheckHitKey(KeyCode);
    
    // 長押しによる意図しない連続入力を防ぐため
    if (down_buffer_[KeyCode] == 0 && keyState == 1)
    {
        result = 1;
    }
    // 次フレームのトリガー判定で前回状態を参照するため
    down_buffer_[KeyCode] = keyState;
    return result;
}
/*
 * 押し続けを許容する通常のキー入力判定を行う
 * [入力] KeyCode: DxLibのキーコード
 * [出力] 1: 押下中、0: それ以外
 * [副作用] なし
 */
int InputManager::CheckPressKey(int KeyCode)
{
    return CheckHitKey(KeyCode);
}