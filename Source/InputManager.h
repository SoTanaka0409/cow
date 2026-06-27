#pragma once

// DxLibのキー入力をラップし、フレーム間の状態変化を管理する
class InputManager
{
public:
    InputManager();
    ~InputManager();

    /*
     * 連続入力を防ぐため、押し始めの1フレームのみを検知する
     * [入力] KeyCode: DxLibのキーコード
     * [出力] 1: 押下開始フレーム、0: それ以外
     * [副作用] mDownBufferを更新
     */
    static int CheckDownKey(int KeyCode);

    /*
     * 押し続けを許容する通常のキー入力判定
     * [入力] KeyCode: DxLibのキーコード
     * [出力] 1: 押下中、0: 非押下
     * [副作用] なし
     */
    static int CheckPressKey(int KeyCode);

private:
    // トリガー判定に必要な前フレームのキー状態を保持
    static int downBuffer[256];
};
