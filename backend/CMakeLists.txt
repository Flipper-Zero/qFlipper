qt_add_library(backend STATIC)

file(GLOB BACKEND_SOURCES
    "*.cpp"
    "flipperzero/*.cpp"
    "flipperzero/*/*.cpp"
)

target_sources(backend
    PRIVATE
        ${BACKEND_SOURCES}
)

target_include_directories(backend
    PUBLIC
        "."
)

target_compile_definitions(backend
    PUBLIC
        "-DAPP_NAME=\"${CMAKE_PROJECT_NAME}\""
        "-DAPP_VERSION=\"${GIT_TAG}\""
        "-DAPP_COMMIT=\"%{GIT_COMMIT}\""
        "-DAPP_TIMESTAMP=\"${GIT_TIMESTAMP}\""
)

target_link_libraries(backend
    PUBLIC
        Qt::Network
        Qt::SerialPort
        PkgConfig::zlib

    PRIVATE
        dfu
        protobufinterface
)
