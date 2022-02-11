# Collect all source files to DIR_LIB_SRCS
aux_source_directory(. DIR_LIB_SRCS)

# build a library target
add_library (ccanimation ${DIR_LIB_SRCS})

install(TARGETS ccanimation
        ARCHIVE DESTINATION lib)
