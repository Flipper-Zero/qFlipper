qt_add_library(dfu STATIC)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(USB_BACKEND "win32")
    set(USB_BACKEND_LIBS setupapi)
else()
    set(USB_BACKEND "libusb")
    set(USB_BACKEND_LIBS PkgConfig::libusb)
endif()

file(GLOB DFU_SOURCES
    "*.cpp"
    "device/*.cpp"
    "device/*/*.cpp"
    "${USB_BACKEND}/*.cpp"
)

target_sources(dfu
    PRIVATE
        ${DFU_SOURCES}
)

target_include_directories(dfu
    PUBLIC
        "."
        "./${USB_BACKEND}"
)

target_compile_definitions(dfu
    PUBLIC
        "-DUSB_BACKEND_$<UPPER_CASE:${USB_BACKEND}>"
)

target_link_libraries(dfu
    PUBLIC
        Qt6::Core
        Qt6::Core5Compat
        ${USB_BACKEND_LIBS}
)
