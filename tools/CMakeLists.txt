set(OLLVM_DRIVERS
        bcf
        sub
        split
        fla)

set(fla_SOURCES
        StaticFlattening.cpp
        ../src/Flattening.cpp)

set(sub_SOURCES
        StaticSubstitution.cpp
        ../src/Substitution.cpp)

set(bcf_SOURCES
        StaticBogusControlFlow.cpp
        ../src/BogusControlFlow.cpp)

set(split_SOURCES
        StaticSplitBasicBlocks.cpp
        ../src/SplitBasicBlocks.cpp)

foreach(OLLVM_DRIVER ${OLLVM_DRIVERS})
    add_executable(${OLLVM_DRIVER}
            ${${OLLVM_DRIVER}_SOURCES})

    target_link_libraries(${OLLVM_DRIVER}
            LLVMPasses
            utils
            cryptoutils)

    target_include_directories(${OLLVM_DRIVER}
            PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/../include")

    install(TARGETS ${OLLVM_DRIVER}
            RUNTIME DESTINATION bin)
endforeach()

