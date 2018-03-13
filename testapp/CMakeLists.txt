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

target_link_libraries(qgsqtestapp
    PRIVATE
        Qt5::Core
        Qt5::Network
        QGSQ
)
