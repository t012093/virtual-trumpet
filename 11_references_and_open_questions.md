# 参考資料と未決事項

## 1. 物理モデル

### Time-domain numerical modeling of brass instruments including nonlinear wave propagation, viscothermal losses, and lips vibration

- Authors: Harold Berjamin, Bruno Lombard, Christophe Vergez, Emmanuel Cottanceau
- Year: 2015 preprint / 2017 journal publication
- URL: https://arxiv.org/abs/1511.04247
- 利用箇所: 唇と管内圧の結合、非線形伝搬、粘性・熱損失

### An Algorithm for a Valved Brass Instrument Synthesis Environment using Finite-Difference Time-Domain Methods with Performance Optimisation

- Authors: Reginald L. Harrison, Stefan Bilbao, James Perry
- Year: 2015
- URL: https://www.dafx.de/paper-archive/2015/DAFx-15_submission_55.pdf
- 利用箇所: バルブ管路、FDTD、高精度版の検討

### Digital Sound Synthesis of Brass Instruments by Physical Modeling

- Year: 2004
- URL: https://www.dafx.de/paper-archive/2004/P_101.PDF
- 利用箇所: 唇、マウスピース、管体を分離した構成

### Embouchure Interaction Model for Brass Instruments

- Year: 2020
- URL: https://vbn.aau.dk/ws/files/335384955/SMCCIM_2020_paper_63.pdf
- 利用箇所: アンブシュアの独立性、過渡応答、リアルタイム実装

### A Lip Vibration Model Using Mechanical Properties of Flesh

- Author: Michael T. Strauss
- Year: 2024 preprint / 2025 journal publication
- URL: https://arxiv.org/abs/2404.05056
- 利用箇所: 可変唇質量、口腔・声道、独立したマウスピース共鳴

## 2. AI音源

### MIDI-DDSP: Detailed Control of Musical Performance via Hierarchical Modeling

- Year: 2021
- URL: https://arxiv.org/abs/2112.09312
- 利用箇所: ノート、演奏表現、合成パラメータを分ける階層設計

### Designing Neural Synthesizers for Low-Latency Interaction

- Year: 2025
- URL: https://arxiv.org/abs/2503.11562
- 利用箇所: 因果モデル、短いブロック、低遅延推論、ジッター評価

### Learning Control of Neural Sound Effects Synthesis from Physically Inspired Models

- Year: 2025
- URL: https://arxiv.org/abs/2503.08806
- 利用箇所: 物理モデルの操作体系を保持したニューラル補正

## 3. 実装基盤

### JUCE

- URL: https://github.com/juce-framework/JUCE
- 用途: AU、VST3、Standalone、MIDI、UI、状態保存

### Faust

- URL: https://faust.grame.fr/
- 物理モデルライブラリ: https://faustlibraries.grame.fr/libs/physmodels/
- 用途: DSP試作、比較、生成コード

### VST3 Developer Portal

- URL: https://steinbergmedia.github.io/vst3_dev_portal/
- 用途: VST3仕様、パラメータ、イベント、検証

## 4. DAW対応資料

- Ableton Live対応形式:
  https://help.ableton.com/hc/en-us/articles/5937501570460-Supported-Plug-in-Formats
- Apple Audio Units:
  https://support.apple.com/102239

## 5. 未決事項

### 即決（Phase 0 中）

| # | 項目 | 備考 |
|---|------|------|
| 1 | JUCEのライセンス方針 | リポジトリ公開範囲・商用利用可否に直結 |
| 2 | オープンソース公開範囲 | GPL / 商用デュアル / 独自ライセンス |
| 3 | Faust生成コードを本番DSPに使用するか | Phase 1 の実装方針に影響 |

### Phase 1 開始前

| # | 項目 | 備考 |
|---|------|------|
| 4 | 最初に対象とするトランペットの種類 | B♭で仮決め済み。確定が必要 |
| 5 | 対象音域 | 校正データ収集範囲を決める |
| 6 | ブレスコントローラーなしでの標準操作方法 | CC2 を息圧の第一候補と仮決め |

### Phase 1 完了前

| # | 項目 | 備考 |
|---|------|------|
| 7 | 管体を純粋なウェーブガイドにするか、実測モードを併用するか | 分数ディレイ比較検証の結果を踏まえる |
| 8 | 実物測定を行うか、公開データから開始するか | Phase 5 の計画に影響 |

### Phase 3 まで

| # | 項目 | 備考 |
|---|------|------|
| 9 | バルブ操作をMIDIノートから自動決定するか、直接入力にも対応するか | MIDI→運指マッピングの設計 |
| 10 | AI学習データの収録方法と利用許諾 | Phase 6 の準備

## 6. MVPでの暫定決定

- B♭トランペット相当
- モノフォニック
- 標準的な実用音域から開始
- CC2を息圧の第一候補とする
- 1質点唇モデル
- 双方向ウェーブガイド
- 簡易マウスピースとベル
- MIDIノートから運指と倍音域を自動決定
- AIはMVP後に追加
- AUv2、VST3、Standaloneを提供
