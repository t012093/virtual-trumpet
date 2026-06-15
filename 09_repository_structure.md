# 推奨ディレクトリ構造

```text
virtual-trumpet/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── cmake/
│   ├── Dependencies.cmake
│   ├── CompilerOptions.cmake
│   └── Packaging.cmake
├── external/
│   ├── JUCE/
│   └── RTNeural/
├── Source/
│   ├── Plugin/
│   │   ├── PluginProcessor.cpp
│   │   ├── PluginProcessor.h
│   │   ├── PluginEditor.cpp
│   │   └── PluginEditor.h
│   ├── Core/
│   │   ├── SynthEngine.cpp
│   │   ├── SynthEngine.h
│   │   ├── Parameters.cpp
│   │   └── Parameters.h
│   ├── Performance/
│   │   ├── MidiInterpreter.cpp
│   │   ├── ArticulationState.cpp
│   │   ├── FingeringMap.cpp
│   │   ├── ExpressionMapper.cpp
│   │   └── PerformanceState.h
│   ├── Physics/
│   │   ├── PhysicalEngine.cpp
│   │   ├── LipModel.cpp
│   │   ├── OralCavity.cpp
│   │   ├── MouthpieceModel.cpp
│   │   ├── BoreWaveguide.cpp
│   │   ├── ValveNetwork.cpp
│   │   ├── BellRadiation.cpp
│   │   └── AirLossFilter.cpp
│   ├── DSP/
│   │   ├── FractionalDelay.h
│   │   ├── ParameterSmoother.h
│   │   ├── NoiseGenerator.h
│   │   ├── SafetyLimiter.h
│   │   └── MathUtils.h
│   ├── AI/
│   │   ├── AIController.cpp
│   │   ├── FeatureExtractor.cpp
│   │   ├── ModelRunner.cpp
│   │   └── FallbackController.cpp
│   ├── Presets/
│   │   ├── PresetManager.cpp
│   │   └── StateSerializer.cpp
│   └── UI/
│       ├── MainPanel.cpp
│       ├── PhysicalControls.cpp
│       ├── PerformanceControls.cpp
│       ├── Visualizer.cpp
│       └── LookAndFeel.cpp
├── Faust/
│   ├── prototypes/
│   │   ├── brass_minimal.dsp
│   │   └── lip_waveguide.dsp
│   └── generated/
├── Models/
│   ├── trumpet_controller.onnx
│   └── model_metadata.json
├── Resources/
│   ├── Presets/
│   ├── Measurements/
│   │   ├── bore_impedance/
│   │   └── bell_responses/
│   └── Images/
├── Tests/
│   ├── Unit/
│   ├── Integration/
│   ├── Audio/
│   └── Plugin/
├── Tools/
│   ├── offline_renderer/
│   ├── measure_latency/
│   ├── analyze_recordings/
│   └── train_controller/
├── Docs/
└── installer/
    ├── macOS/
    └── Windows/
```

## モジュール境界

### `Source/Plugin`

JUCEとDAWへの接続だけを担当する。物理計算を直接記述しない。

### `Source/Core`

音源全体の処理順序、初期化、状態の受け渡しを担当する。

### `Source/Performance`

MIDIを演奏意図と物理パラメータへ変換する。

### `Source/Physics`

トランペットの発音機構を計算する。可能な限りJUCEへ依存しない。

### `Source/DSP`

遅延線、フィルター、平滑化、数学関数など再利用可能な部品を置く。

### `Source/AI`

AI推論とルールベースのFallback Controllerを置く。AIなしでもビルド可能にする。

### `Faust`

実験コードと生成コードを分離する。生成物を手作業で直接編集しない。

### `Resources/Measurements`

測定元、単位、サンプルレート、ライセンス、変換履歴をメタデータとして併記する。

## MVPで最初に作るファイル

```text
CMakeLists.txt
Source/Plugin/PluginProcessor.*
Source/Plugin/PluginEditor.*
Source/Core/SynthEngine.*
Source/Core/Parameters.*
Source/Performance/MidiInterpreter.*
Source/Performance/ArticulationState.*
Source/Performance/ExpressionMapper.*
Source/Physics/PhysicalEngine.*
Source/Physics/LipModel.*
Source/Physics/BoreWaveguide.*
Source/Physics/BellRadiation.*
Source/DSP/FractionalDelay.h
Source/DSP/ParameterSmoother.h
Source/DSP/SafetyLimiter.h
Tests/Unit/
Tests/Audio/
```

AI、口腔、精密バルブ、インストーラーはMVPの音が安定してから追加する。
