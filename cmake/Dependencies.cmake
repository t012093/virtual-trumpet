include(FetchContent)

message(STATUS "Fetching JUCE...")

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.0
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(JUCE)

message(STATUS "JUCE fetched successfully.")
