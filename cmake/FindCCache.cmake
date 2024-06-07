find_program(CCACHE_FOUND ccache)

if(CCACHE_FOUND)
    message(STATUS "Found CCache: ${CCACHE_FOUND}")
    set_property(GLOBAL PROPERTY CXX_COMPILER_LAUNCHER ${CCACHE_FOUND})
endif()
