set(OLLVM_PLUGINS
        BogusControlFlow
        SplitBasicBlocks
        Flattening
        Substitution)

set(OLLVM_HEADERS
        ../include/BogusControlFlow.h
        ../include/Flattening.h
        ../include/SplitBasicBlocks.h
        ../include/Substitution.h
        ../include/CryptoUtils.h)

set(BogusControlFlow_SOURCES
        BogusControlFlow.cpp)

set(Flattening_SOURCES
        Flattening.cpp)

set(SplitBasicBlocks_SOURCES
        SplitBasicBlocks.cpp)

set(Substitution_SOURCES
        Substitution.cpp)

add_library(utils OBJECT Utils.cpp)
target_include_directories(utils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_library(cryptoutils OBJECT CryptoUtils.cpp)
target_include_directories(cryptoutils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

foreach(OLLVM_PLUGIN ${OLLVM_PLUGINS})
    add_library(${OLLVM_PLUGIN}
            SHARED
            ${${OLLVM_PLUGIN}_SOURCES})

    target_include_directories(
            ${OLLVM_PLUGIN}
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../include)

    target_link_libraries(
            ${OLLVM_PLUGIN}
            PRIVATE
                utils
                cryptoutils
            PUBLIC
                "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>")

    install(TARGETS ${OLLVM_PLUGIN}
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endforeach(OLLVM_PLUGIN)

