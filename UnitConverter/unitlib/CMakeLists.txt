add_library(unitlib STATIC
    src/unitlib.cpp
)

# PUBLIC：unitlib 自己用，链接它的目标（unit_converter）也自动获得这个路径
target_include_directories(unitlib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(unitlib
    PRIVATE
        UNITLIB_VERSION="1.0.0"
)

set_target_properties(unitlib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)

install(TARGETS unitlib ARCHIVE DESTINATION lib)
install(FILES include/unitlib.h DESTINATION include/unitlib)

message(STATUS "unitlib 源码目录: ${CMAKE_CURRENT_SOURCE_DIR}")
