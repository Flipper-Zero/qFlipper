qt_add_plugin(flipperproto0
    CLASS_NAME "ProtobufPlugin"
)

file(GLOB PLUGIN_CXX_SOURCES
    "*.cpp"
)

file(GLOB PLUGIN_PROTO_FILES
    "flipperzero-protobuf/*.proto"
)

nanopb_generate_cpp(
    PLUGIN_PROTO_SOURCES
    PLUGIN_PROTO_HEADERS
    ${PLUGIN_PROTO_FILES}
)

set_source_files_properties(
    ${PLUGIN_PROTO_SOURCES}
    ${PLUGIN_PROTO_HEADERS}
    PROPERTIES GENERATED TRUE)

target_sources(flipperproto0
    PRIVATE
        ${PLUGIN_CXX_SOURCES}
        ${PLUGIN_PROTO_SOURCES}
)

target_include_directories(flipperproto0
    PRIVATE
        ${NANOPB_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_definitions(flipperproto0
    PRIVATE
        "-DPB_ENABLE_MALLOC"
)

target_link_libraries(flipperproto0
    PRIVATE
        protobufinterface
        Qt::Core
)
