add_executable(VirtualTrumpet_Tests)

target_sources(VirtualTrumpet_Tests PRIVATE
    Unit/UnitTestMain.cpp
)

target_link_libraries(VirtualTrumpet_Tests PRIVATE
    VirtualTrumpet_Core
    juce::juce_audio_processors
)

