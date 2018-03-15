set(qgsqtestapp_SRCS
    main.cpp
)

add_executable(qgsqtestapp ${qgsqtestapp_SRCS})

target_include_directories(qgsqtestapp
    PRIVATE
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(qgsqtestapp
    PRIVATE
        VERSION="${PROJECT_VERSION}"
)

target_link_libraries(qgsqtestapp
    PRIVATE
        Qt5::Core
        Qt5::Network
        QGSQ
)

if (ENABLE_ASAN)
    target_compile_options(qgsqtestapp
        PRIVATE
            -fsanitize=address
            -fno-omit-frame-pointer
            -Wformat
            -Werror=format-security
            -Werror=array-bounds
            -g
            -ggdb
    )
    set_target_properties(qgsqtestapp PROPERTIES
        LINK_FLAGS -fsanitize=address
    )
endif (ENABLE_ASAN)
