qt_add_library(protobufinterface INTERFACE)
target_include_directories(protobufinterface
    INTERFACE
        "${CMAKE_CURRENT_LIST_DIR}"
)
