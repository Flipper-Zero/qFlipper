set(CLI_TARGET "${CMAKE_PROJECT_NAME}_cli")

qt_add_executable(${CLI_TARGET})

file(GLOB CLI_SOURCES
    "*.cpp"
)

target_sources(${CLI_TARGET}
    PRIVATE
        ${CLI_SOURCES}
)

target_link_libraries(${CLI_TARGET}
    PRIVATE
        backend
        dfu
)
