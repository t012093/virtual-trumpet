# プラグイン・対応環境仕様

## 1. 開発基盤

- 言語: C++20
- フレームワーク: JUCE
- ビルド: CMake
- macOS IDE: Xcode
- Windows IDE: Visual Studio
- DSP試作: Faust
- AI推論候補: RTNeural、ONNX Runtime、独自軽量推論

## 2. 出力形式

| 形式 | macOS | Windows | 用途 |
|---|---|---|---|
| AUv2 | 必須 | 非対応 | GarageBand、Logic Pro、Ableton Live |
| VST3 | 必須 | 必須 | Ableton Live、Cubase、Studio One、Reaper |
| Standalone | 必須 | 必須 | 単体テスト、ライブ演奏 |
| AUv3 | 将来 | 非対応 | macOS/iOS拡張 |
| CLAP | 将来 | 将来 | 対応ホスト向け |

## 3. 対応DAW

初期検証対象:

- GarageBand
- Ableton Live
- Logic Pro
- Reaper

追加検証:

- Cubase
- Studio One

## 4. プラグインバス

- MIDI入力: 1
- 音声入力: なし
- 音声出力: Stereo
- Sidechain: なし
- MIDI出力: 初期版ではなし

内部物理モデルはMonoで生成し、Room / Spatializer モジュールでStereo化する。Width=0で完全Mono出力になることを保証する。

## 5. 公開パラメータ

### Performance

- Breath
- Tongue
- Lip Tension
- Lip Aperture
- Vibrato Depth
- Vibrato Rate
- Expression Assist
- Legato Mode

### Instrument

- Mouthpiece
- Bore Profile
- Bell
- Valve Tuning
- Mute
- Tuning (cents, default A4=440Hz, range ±50 cents)

### Output

- Output Gain
- Width
- Room Amount
- Safety Limiter

DAWオートメーションの互換性を維持するため、公開後のパラメータIDは変更しない。

## 6. 状態保存

保存対象:

- 全公開パラメータ
- MIDI Learn割り当て
- 選択した楽器定義
- AI有効状態と補助量
- UI表示状態のうち必要なもの
- 状態形式バージョン

学習用データや大容量モデルをDAW状態へ埋め込まない。

### プリセット管理

| 種別 | 保存先 | 編集 | 備考 |
|---|---|---|---|
| ファクトリープリセット | `Resources/Presets/` | 読み取り専用 | バンドルに同梱。ユーザーが上書き不可 |
| ユーザープリセット | ユーザーディレクトリ（`~/Documents/VirtualTrumpet/Presets/` 等） | 自由に作成・編集・削除 | DAWプロジェクトとは独立して保存 |

プリセットブラウザは MVP に含め、ファクトリー/ユーザーのタブ切替を備える。

### サンプルレート変更時の動作

`prepareToPlay` 呼び出し時に以下を実行する:

- 全遅延線を新しいサンプルレートで再初期化
- フィルター係数を再計算
- 分数ディレイの内部状態をリセット
- 唇の変位・速度、管内の圧力波など物理状態変数は可能な限り保持
- 保持できない状態（遅延線の過去サンプルなど）はゼロ初期化

サンプルレート変更後も発振状態を維持し、聴感上の途切れを最小化する。

## 7. インストール先

### macOS

```text
/Library/Audio/Plug-Ins/Components
/Library/Audio/Plug-Ins/VST3
```

ユーザー単位のインストールにも対応可能とする。

### Windows

```text
C:\Program Files\Common Files\VST3
```

## 8. 署名と配布

macOS:

- Developer ID署名
- Hardened Runtime
- 公証
- Universal Binary

Windows:

- x64ビルド
- コード署名
- インストーラー

## 9. CMakeターゲット

```text
VirtualTrumpet_Core
VirtualTrumpet_Tests
VirtualTrumpet_AU
VirtualTrumpet_VST3
VirtualTrumpet_Standalone
```

CoreライブラリはJUCEプラグイン層から分離し、単体テストとオフラインレンダリングから再利用する。

## 10. 互換性方針

- 64-bitのみ
- macOSはApple Siliconを主対象とし、必要に応じてIntelも提供
- VST2は新規対応しない
- プラグイン固有コードを最小限にし、AUとVST3の挙動差をPlugin Adapterで吸収する
