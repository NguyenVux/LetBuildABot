add_library(UI STATIC
    src/dummy.cpp
    src/Widget.cpp
    src/UIManager.cpp
    src/Helper.cpp
)

add_library(${NAMESPACE}::UI ALIAS UI)

target_link_libraries(UI PUBLIC reflectcpp raylib)
target_include_directories(UI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/public
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/private
)
