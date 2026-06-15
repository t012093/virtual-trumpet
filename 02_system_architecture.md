# システムアーキテクチャ

## 1. 全体構成

```text
DAW / Standalone Host
        |
        v
Plugin Adapter (JUCE)
        |
        +--> Parameter Store / Presets / Automation
        |
        v
MIDI Interpreter
        |
        v
Performance State Machine
        |
        v
Expression Mapper --------> Optional AI Controller
        |                            |
        +-------------+--------------+
                      v
              Physical Engine
        +-------------+-------------+
        |             |             |
    Oral Cavity    Lip Model    Mouthpiece
                                      |
                                      v
                           Bore / Valve Network
                                      |
                                      v
                              Bell Radiation
                                      |
                                      v
                              Post Processor
                                      |
                                      v
                              Room / Spatializer
                                      |
                                      v
                                Audio Output
```

## 2. レイヤーの責務

### Plugin Adapter

- AU/VST3/Standaloneの差を吸収する
- DAWからMIDI、テンポ、サンプルレート、バッファサイズを受け取る
- 公開パラメータと状態保存を管理する
- DSP内部へホスト固有APIを持ち込まない

### MIDI Interpreter

- Note、Velocity、CC、Aftertouch、Pitch Bend、MPEを正規化する
- MIDIチャンネルとコントローラー割り当てを処理する
- タイムスタンプを保ったイベント列をPerformance層へ渡す

### Performance State Machine

- 無音、アタック、持続、レガート、リリースを管理する
- 新しいノートがタンギングかスラーか判定する
- 運指、倍音番号、目標音程を決定する

### Expression Mapper

- 演奏入力を物理パラメータの目標値へ変換する
- 音域ごとの基準値とカーブを保持する
- パラメータを制御レートで平滑化する
- AIが無効な場合の決定論的な代替動作を提供する

### Physical Engine

- 唇と管内圧の閉ループ相互作用をサンプル単位で計算する
- 口腔、マウスピース、管体、バルブ、ベルを合成する
- 音声スレッドだけで完結するリアルタイム安全な実装とする

### Post Processor

- DC除去
- 最終EQ
- 微量の息ノイズ
- 出力ゲイン
- 安全リミッター

### Room / Spatializer

物理モデルの出力は Mono。Stereo 化は独立したモジュールで行う。

- 物理モデル出力: Mono
- 初期反射・Room モジュール: 独立したステレオ出力を持つ
- Width パラメータ: 0 で完全 Mono、1.0 で最大ステレオ幅
- Mono compatibility: Width=0 時の L/R が完全に同一であることをテストで保証する
- 左右の位相差（数サンプルずらし）による簡易ステレオ化は禁止（モノラル合成時にコムフィルターを生むため）

AIを導入する場合も、AI補正後の出力に過大な変化が出ないよう制限する。

## 3. 処理レート

| 処理 | 推奨レート |
|---|---:|
| 唇・管体・ベル | オーディオサンプルレート |
| パラメータ平滑化 | オーディオまたは1～2 kHz |
| 演奏状態更新 | MIDIイベント単位 |
| AI演奏推定 | 200～500 Hz |
| UI表示 | 30～60 Hz |

## 4. スレッドモデル

### Audio Thread

- MIDIイベント適用
- 演奏状態更新
- 物理モデル計算
- 軽量AI推論
- 音声出力

禁止事項:

- 動的メモリ確保
- mutex取得
- ログファイル書き込み
- モデルやプリセットの読み込み

### Message/UI Thread

- UI描画
- パラメータ編集
- ファイルダイアログ
- プリセット管理

### Background Thread

- AIモデル読み込み
- 測定データ解析
- ファクトリープリセット探索
- 重い事前計算

## 5. 状態データ

```text
PluginState
├── PublicParameters
├── MidiMapping
├── InstrumentDefinition
├── ModelCalibration
├── PresetMetadata
└── Version
```

保存形式にはJUCE `ValueTree`を使用し、将来の移行に備えて必ずバージョン番号を持たせる。

## 6. 依存方向

```text
Plugin -> Core -> Performance -> Physics -> DSP
                    |
                    +-> AI
```

- PhysicsとDSPはJUCEに依存させない
- AIがなくてもCoreをビルドできる
- テストからPhysicsを単独実行できる
- Faust生成コードを使用する場合もPhysics内部の交換可能な実装として扱う
