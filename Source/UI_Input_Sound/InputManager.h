#pragma once
/// @brief DxLibの入力をラップし、状態変化の管理を容易にするため。
class InputManager
{
public:
    InputManager();
    ~InputManager();
    /// @brief 連続入力を防ぎ、単発の操作だけを受け付けるため。
    /// @param key_code DxLibのキーコード
    /// @return 1: 押下開始フレーム、0: それ以外
    /// @details down_buffer_を更新
    static int CheckDownKey(int key_code);
    /// @brief 長押しによる連続的な操作を可能にするため。
    /// @param key_code DxLibのキーコード
    /// @return 1: 押下中、0: それ以外
    static int CheckPressKey(int key_code);
private:
    static int down_buffer_[256]; ///< 複数のデータを管理するコンテナ
};
