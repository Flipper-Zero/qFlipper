execute_process(COMMAND "git"
    "describe"
    "--tags"
    "--abbrev=0"
    WORKING_DIRECTORY
        "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE
        GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(GIT_TAG STREQUAL "")
    set(GIT_TAG "unknown")
endif()

execute_process(COMMAND "git"
    "rev-parse"
    "--short=8"
    "HEAD"
    WORKING_DIRECTORY
        "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE
        GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(GIT_COMMIT STREQUAL "")
    set(GIT_COMMIT "unknown")
endif()

execute_process(COMMAND "git"
    "log"
    "-1"
    "--pretty=format:%ct"
    WORKING_DIRECTORY
        "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE
        GIT_TIMESTAMP
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(GIT_TIMESTAMP STREQUAL "")
    set(GIT_TIMESTAMP "unknown")
endif()
