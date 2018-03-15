set(qgsq_SRC
    qgsq.cpp
    Valve/Source/serverquery.cpp
    Valve/Source/serverquery_p.h
    Valve/Source/response.cpp
    Valve/Source/serverinfo.cpp
    Valve/Source/serverinfo_p.h
    Valve/Source/player.cpp
    Valve/Source/player_p.h
)

set(qgsq_HEADERS
    qgsq_global.h
    qgsq.h
    Valve/Source/serverquery.h
    Valve/Source/serverinfo.h
    Valve/Source/player.h
)

set(qgsq_PRIVATE_HEADERS
    Valve/Source/response.h
)

add_library(qgsq SHARED
    ${qgsq_SRC}
    ${qgsq_HEADERS}
    ${qgsq_PRIVATE_HEADERS}
)

add_library(QGSQ ALIAS qgsq)

target_compile_features(qgsq
    PRIVATE
        cxx_auto_type
        cxx_lambdas
        cxx_nonstatic_member_init
        cxx_range_for
        cxx_long_long_type
        cxx_override
        cxx_right_angle_brackets
    PUBLIC
        cxx_nullptr
)

target_include_directories(qgsq
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/QGSQ>
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_definitions(qgsq
    PRIVATE
        QT_NO_KEYWORDS
        QT_NO_CAST_TO_ASCII
        QT_NO_CAST_FROM_ASCII
        QT_STRICT_ITERATORS
        QT_NO_URL_CAST_FROM_STRING
        QT_NO_CAST_FROM_BYTEARRAY
        QT_USE_QSTRINGBUILDER
        QT_NO_SIGNALS_SLOTS_KEYWORDS
        QT_USE_FAST_OPERATOR_PLUS
        QT_DISABLE_DEPRECATED_BEFORE=0x050900
)

set_target_properties(qgsq PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${QGSQ_API_LEVEL}
)

target_link_libraries(qgsq
    PUBLIC
        Qt5::Core
        Qt5::Network
)

if (ENABLE_ASAN)
    target_compile_options(qgsq
        PRIVATE
            -fsanitize=address
            -fno-omit-frame-pointer
            -Wformat
            -Werror=format-security
            -Werror=array-bounds
            -g
            -ggdb
    )
    set_target_properties(qgsq PROPERTIES
        LINK_FLAGS -fsanitize=address
    )
endif (ENABLE_ASAN)

set_property(TARGET qgsq PROPERTY PUBLIC_HEADER ${qgsq_HEADERS})

install(TARGETS qgsq
    EXPORT QGSQTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION include/QGSQ
)
