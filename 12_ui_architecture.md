# UI アーキテクチャ仕様

## 1. 基本方針

UI は次の3層で構成し、段階的に背景画像依存を減らして JUCE ネイティブ描画へ移行する。

```text
Layer 3: JUCE ネイティブ描画（パネル、テキスト、波形、メーター）
Layer 2: JUCE コンポーネント（Slider, Button, ComboBox）
Layer 1: 背景画像（PNG/SVG, トランペット図、パネル枠）
```

**MVP（Phase 4）**: Layer 1 + Layer 2 併用。背景画像の上に JUCE コンポーネントを重ねる。

**Version 1 以降**: Layer 3 を拡充し、背景画像への依存を減らす。パネル枠、ラベル、装飾を JUCE の `paint()` で描画。

## 2. データフロー

```text
操作（マウス/タッチ/MIDI Learn/DAWオートメーション）
  ↓
PluginEditor
  Slider / Button / ComboBox
  ↓ SliderAttachment / ButtonAttachment / ComboBoxAttachment
AudioProcessorValueTreeState (APVTS)
  ↓ getRawParameterValue()->load()
ParameterSmoother (1次IIR)
  ↓
TrumpetEngine
  Lip / Breath / Bore / Valve / Bell
  ↓
Audio Output
```

### SliderAttachment の役割

`AudioProcessorValueTreeState::SliderAttachment` により、以下の3経路が単一のパラメータ値に同期する:

1. **UI操作**: ユーザーがノブ/スライダーを動かす
2. **DAWオートメーション**: ホストがパラメータを書き換える
3. **プリセット保存/読込**: `ValueTree` 経由でシリアライズされる

```cpp
// Processor 側：パラメータ定義
juce::AudioProcessorValueTreeState parameters;

layout.add(std::make_unique<juce::AudioParameterFloat>(
    "breath",
    "Breath",
    juce::NormalisableRange<float>(0.0f, 1.0f),
    0.5f));

// Editor 側：Slider とパラメータの接続
juce::Slider breathSlider;

using SliderAttachment =
    juce::AudioProcessorValueTreeState::SliderAttachment;

std::unique_ptr<SliderAttachment> breathAttachment;

breathAttachment = std::make_unique<SliderAttachment>(
    processor.getAPVTS(),
    "breath",
    breathSlider);
```

### DSP 側での値取得

```cpp
// Audio Thread — リアルタイム安全
const float targetBreath =
    parameters.getRawParameterValue("breath")->load();

lipModel.setMouthPressure(
    breathSmoother.process(targetBreath));
```

`getRawParameterValue()` が返す `std::atomic<float>*` を `load()` することで、lock-free に audio thread から値を読める。

## 3. コンポーネント分解

### 3.1 背景とトランペット図

| 要素 | 方式 | 備考 |
|------|------|------|
| 背景（筐体パネル） | PNG 画像 | `juce::DrawableImage` または `g.drawImage()` |
| トランペットの図 | PNG または SVG | 装飾目的。音響とは非連動 |
| パネル枠・区切り線 | 画像 → 将来 `paint()` に移行 | Version 1 で JUCE 描画に置換 |

画像は `Resources/Images/` に配置し、`BinaryData` としてバイナリに埋め込む。

### 3.2 操作コンポーネント

| パラメータ | コンポーネント種別 | 配置 |
|------------|-------------------|------|
| Breath | Slider（縦ノブ） | 左側パネル |
| Tongue | Slider（縦ノブ） | 左側パネル |
| Lip Tension | Slider（縦ノブ） | 左側パネル |
| Lip Aperture | Slider（縦ノブ） | 左側パネル |
| Vibrato Depth | Slider（横ノブ） | 上部 |
| Vibrato Rate | Slider（横ノブ） | 上部 |
| Expression Assist | ToggleButton | 左側パネル |
| Legato Mode | ToggleButton | 左側パネル |
| Mouthpiece | ComboBox | 中央 |
| Bore Profile | ComboBox | 中央 |
| Bell | ComboBox | 中央 |
| Valve Tuning | Slider（横） | 中央 |
| Mute | ToggleButton | 中央 |
| Tuning | Slider（横） | 中央 |
| Valve 1/2/3 | Slider ×3 | バルブ位置（トランペット図に重ねる） |
| Output Gain | Slider（縦ノブ） | 右側パネル |
| Width | Slider（縦ノブ） | 右側パネル |
| Room Amount | Slider（縦ノブ） | 右側パネル |
| Safety Limiter | ToggleButton | 右側パネル |

### 3.3 表示コンポーネント

| 要素 | 方式 | 更新レート |
|------|------|------------|
| 波形表示 | `paint()` によるオシロスコープ | 30〜60 Hz（Timer） |
| 管内圧表示 | `paint()` によるレベルメーター | 30〜60 Hz |
| 鍵盤 | `juce::MidiKeyboardComponent` または独自 `Component` | イベント駆動 |
| CPU メーター | `Timer` でラベル更新のみ | 10 Hz |
| プリセット名 | `juce::Label` | プリセット切替時 |

### 3.4 プリセット管理

```cpp
// プリセットブラウザの状態は APVTS::state で管理
auto presetTree = processor.getAPVTS().state;

// ファクトリープリセットの読込
auto xml = juce::parseXML (BinaryData::factory_presets_xml);
presetTree.copyPropertiesAndChildrenFrom (
    juce::ValueTree::fromXml (*xml), nullptr);
```

## 4. LookAndFeel カスタマイズ

JUCE の `LookAndFeel_V4` を継承した独自クラスで、以下の要素を統一描画する:

| 対象 | カスタマイズ内容 |
|------|-----------------|
| ノブ（Rotary Slider） | 金属調の円形ノブ。ドラッグ方向、ポインター色 |
| 横スライダー | つまみ形状、トラック色、目盛り |
| ボタン | 角丸、ホバー時ハイライト、トグル時の色変化 |
| ComboBox | ドロップダウン背景、矢印アイコン |
| パネル背景 | グラデーション、枠線 |
| ラベル | フォント、色、整列 |

```cpp
class TrumpetLookAndFeel : public juce::LookAndFeel_V4 {
public:
    TrumpetLookAndFeel() {
        // ノブ色
        setColour (juce::Slider::rotarySliderFillColourId,
                   juce::Colour (0xffd4a854)); // 金管色
        setColour (juce::Slider::thumbColourId,
                   juce::Colour (0xffe8c97a));
    }

    void drawRotarySlider (juce::Graphics& g, int x, int y,
                           int width, int height,
                           float sliderPos, float startAngle,
                           float endAngle,
                           juce::Slider& slider) override {
        // カスタムノブ描画
    }
};
```

## 5. 画像の埋め込みと配置

### BinaryData への埋め込み

```cmake
# CMakeLists.txt
juce_add_binary_data (VirtualTrumpetBinaryData
    Resources/Images/background.png
    Resources/Images/trumpet_diagram.png
)
```

### 背景画像の描画

```cpp
void paint (juce::Graphics& g) override {
    // Layer 1: 背景画像
    auto bg = juce::ImageCache::getFromMemory (
        BinaryData::background_png,
        BinaryData::background_pngSize);
    g.drawImage (bg, getLocalBounds().toFloat());

    // Layer 2: コンポーネントは addAndMakeVisible() で自動描画される
    // Layer 3: 将来はここでパネルや枠線を g.drawRoundedRectangle() 等で描画
}
```

## 6. 段階的移行計画

| Phase | Layer 1（画像） | Layer 2（JUCE部品） | Layer 3（JUCE描画） |
|-------|----------------|---------------------|---------------------|
| Phase 4 (MVP) | 背景 + トランペット図 | 全操作コンポーネント | なし |
| Version 1 | トランペット図のみ | 全操作コンポーネント | パネル枠、ラベル |
| Version 2 | なし | 全操作コンポーネント | パネル、テキスト、波形、メーター |

## 7. MIDI Learn

ユーザーがパラメータを右クリック →「Learn MIDI CC」で任意の CC 番号に割り当て可能。

```cpp
// マッピングは APVTS::state の子ノードとして保存
juce::ValueTree midiMappings ("MidiMappings");
// 例: breath パラメータに CC2 を割り当て
auto map = juce::ValueTree ("Mapping");
map.setProperty ("paramID", "breath", nullptr);
map.setProperty ("ccNumber", 2, nullptr);
midiMappings.appendChild (map, nullptr);
```

割り当て情報は DAW プロジェクト保存時に `getStateInformation()` で永続化される。

## 8. 参考資料

- JUCE Tutorial: AudioProcessorValueTreeState
  https://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html
- JUCE LookAndFeel customisation
  https://docs.juce.com/master/tutorial_look_and_feel_customisation.html
- JUCE SliderAttachment
  https://docs.juce.com/master/classAudioProcessorValueTreeState_1_1SliderAttachment.html
