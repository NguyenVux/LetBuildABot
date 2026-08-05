add_executable(LetBuildABot2
    src/main.cpp
)

target_link_libraries(LetBuildABot2 raylib ${NAMESPACE}::UI )
target_include_directories(LetBuildABot2
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/private
)
