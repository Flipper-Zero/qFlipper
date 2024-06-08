set(CLI_TARGET "${CMAKE_PROJECT_NAME}_cli")

qt_add_executable(${CLI_TARGET})

target_sources(${CLI_TARGET}
    PRIVATE
        "main.cpp"
        "cli.cpp"
)

target_link_libraries(${CLI_TARGET}
    PRIVATE
        backend
        dfu
)
