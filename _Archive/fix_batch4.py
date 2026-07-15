import re
import codecs

def process_file(path, replacements):
    try:
        with open(path, 'r', encoding='utf-8') as f:
            text = f.read()
    except UnicodeDecodeError:
        with open(path, 'r', encoding='shift_jis', errors='replace') as f:
            text = f.read()
    
    for old, new in replacements:
        text = re.sub(old, new, text)
        
    with open(path, 'w', encoding='utf-8-sig') as f:
        f.write(text)

process_file('Source/DebugCamera.cpp', [
    (r'// \?E\?p\?E\?x\?E\?l.*', '// 角度が180度を超えた場合ラップアラウンドさせる'),
    (r'// \?E\?\?E\?\?E\?\?E\?\?E\?\?E\?\?E\?\?E\?s.*', '// 0キー入力でマウスカーソルを画面中央に固定する')
])

process_file('Source/EffectManager.cpp', [
    (r'// DXライブラリ側でDirect3D11を使用するように設定する！Effekseerの要求仕様！E', '// Effekseerの要求仕様であるDirect3D11を使用するように設定する'),
    (r'// 表示可能なパEチEクルの最大数を指定して、EffekseerシスチEをE期化する', '// Effekseerシステムを初期化する'),
    (r'// 画面モードE替Eウィンドウ/フルスクリーンE時、EffekseerのグラフィチEスチEイスリセチEを防ぁE', '// 画面モード切替時のグラフィックスデバイスリセットを防ぐ'),
    (r'// DXライブラリでのチEイス喪失E復帰とEffekseerの冁Eリソース再生成を結Eつけるコールバックを設定すめE', '// デバイス喪失時のコールバックを設定する'),
    (r'// 3Dエフェクト描画時に前後関係（深度値Eが正しく老EEされるよぁEZバッファ処琁E有効にする', '// 深度値が正しく考慮されるようにZバッファ処理を有効にする'),
    (r'// 毎フレーム呼び出し、現在再生中の全Effekseer3DエフェクトEアニメーションスチEプを進める', '// 現在再生中の全エフェクトのアニメーションステップを進める'),
    (r'// 再生中かつ画面冁E収まるEffekseerエフェクトを3D描画パスへ送る', '// 画面内に収まるエフェクトを描画する')
])

process_file('Source/EffekseerEffect.cpp', [
    (r'// \?E\?E\?E\?\[\?E\?h\?E\?\?E\?\?\?Effekseer\?E\?G\?E\?t\?E\?F\?E\?N\?E\?g\?E\?A\?E\?Z\?E\?b\?E\?g\?E\?E\?E\?\\E\?\[\?E\?X.*', '// ロード済みのEffekseerエフェクトリソースを破棄する'),
    (r'// .*efk.*', '// 指定されたパスからエフェクトデータをロードする'),
    (r'// .*SetPosPlayingEffekseer3DEffect.*', '// 再生中のエフェクト位置、回転角度、スケール値を更新する\n\t\tSetPosPlayingEffekseer3DEffect'),
    (r'// \?E\?\?\?\?E\?\?\?E\?G\?E\?t\?E\?F\?E\?N\?E\?g\?E\?E\?E\?\\E\?\[\?E\?X.*', '// ロードしたリソースを元に再生を開始する'),
    (r'// \?E\?E\?E\?\?\?\?E\?G\?E\?t\?E\?F\?E\?N\?E\?g.*', '// 再生状態を取得し、再生中の場合のみ停止する')
])

process_file('Source/Fever.cpp', [
    (r'// 難易度調整のためフィーバE継続時間を10私E600フレーム\)に固定すめE', '// 難易度調整のためフィーバー継続時間を固定する'),
    (r'// スチEEジ上E獲物が枯渁EるEを防ぐため終亁Eに基本構Eで再E置する', '// 獲物が枯渇するのを防ぐため終了時に再配置する'),
    (r'// フィーバE状態であることを視覚的に伝えるため画面全体に加算ブレンドでエフェクトを描画する', '// フィーバー状態を視覚的に伝えるためエフェクトを描画する'),
    (r'// ボEナススコア獲得機会を提供するため一定間隔で金E牛を生Eする', '// ボーナススコア獲得のため一定間隔で金の牛を生成する'),
    (r'// フィーバE継続時間を趁Eしたため状態を通常に戻ぁE', '// フィーバー継続時間を超過したため状態を通常に戻す')
])

process_file('Source/Floor.cpp', [
    (r'\s*// 床.右上頂点の設宁.', ''),
    (r'\s*// 床.左下頂点の設宁.', ''),
    (r'\s*// 床.右下頂点の設宁.', ''),
    (r'\s*// 床.左上頂点の設宁.', ''),
    (r'// .ꍇ点の法線.クトルを計算して面全体.向きを統一する', '// 各頂点の法線ベクトルを計算して面全体の向きを統一する'),
    (r'// ライチ.ング計算を無効化し、テクスチャ本来の色で床を描画する', '// テクスチャ本来の色で床を描画するためライティング計算を無効化する')
])
