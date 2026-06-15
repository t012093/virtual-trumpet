# Virtual Trumpet Design Documents

物理モデルを中核にしたトランペット音源の設計資料です。

GarageBand、Logic Pro、Ableton Liveなどで利用できるAU/VST3プラグインを想定し、まず演奏可能なMVPを完成させ、その後に精密な物理モデルとAI演奏支援を追加します。

## 推奨方針

- 音源の中核はリアルタイム物理モデルとする
- JUCEとCMakeでAU、VST3、Standaloneを同一コードから生成する
- Faustは物理モデルの試作と数式検証に利用する
- 完成版のDSPは、必要に応じてFaust生成コードまたは独自C++として組み込む
- AIは音声全体の生成ではなく、MIDIから演奏状態を推定する補助層として導入する
- AIを無効にしても演奏できる構造を維持する

## 文書一覧

1. [製品要件](01_product_requirements.md)
2. [システムアーキテクチャ](02_system_architecture.md)
3. [物理モデル仕様](03_physical_model_spec.md)
4. [MIDI・演奏制御仕様](04_midi_performance_spec.md)
5. [プラグイン・対応環境仕様](05_plugin_platform_spec.md)
6. [実装フローとロードマップ](06_implementation_plan.md)
7. [テスト・検証計画](07_test_validation_plan.md)
8. [AI・研究拡張計画](08_ai_research_roadmap.md)
9. [推奨ディレクトリ構造](09_repository_structure.md)
10. [主要な設計判断](10_architecture_decisions.md)
11. [参考資料と未決事項](11_references_and_open_questions.md)
12. [UIアーキテクチャ](12_ui_architecture.md)

## MVPの完成条件

- MIDIノートで単音演奏できる
- ベロシティでタンギングの強さが変わる
- CC2またはMod Wheelで息圧を連続操作できる
- Pitch Bendで自然なリップベンドができる
- レガートと再タンギングを判別できる
- 唇と管内圧が相互作用して自励振動する
- AUとVST3の両方で状態保存と再読込ができる
- 48 kHz、128サンプル以下のバッファで安定動作する
- Width=0 で完全 Mono 出力を保証できる

## 実装優先順位

```text
JUCEプラグイン基盤 (Phase 0 — 即時着手可)
  -> 非線形流量式・積分法の確定 (Phase 1 開始前)
  -> 1質点唇モデル (Newmark/台形則)
  -> 管体ウェーブガイド (Lagrange/Thiran比較)
  -> MIDI状態機械
  -> マウスピースとベル
  -> バルブと散乱係数補間 (Phase 3)
  -> オーバーサンプリング品質選定 (Phase 3)
  -> Room/Spatializer (Phase 4)
  -> 実測補正 (Phase 5)
  -> AI演奏支援 (Phase 6)
```
