#pragma once
// DxLibの入力をラップし、状態変化の管理を容易にするため。
class InputManager
{
public:
    InputManager();
    ~InputManager();
    /*
     * 連続入力を防ぎ、単発の操作だけを受け付けるため。
     * [入力] KeyCode: DxLibのキーコード
     * [出力] 1: 押下開始フレーム、0: それ以外
     * [副作用] down_buffer_を更新
     */
    static int CheckDownKey(int KeyCode);
    /*
     * 長押しによる連続的な操作を可能にするため。
     * [入力] KeyCode: DxLibのキーコード
     * [出力] 1: 押下中、0: それ以外
     * [副作用] なし
     */
    static int CheckPressKey(int KeyCode);
private:
    static int down_buffer_[256];
};
