cmake_minimum_required(VERSION 3.10)
project(Kalculator)

set(CMAKE_CXX_STANDARD 11)


find_package( FLEX )
find_package( BISON )

FLEX_TARGET( KalcLexer sources/lexer.l
        ${CMAKE_BINARY_DIR}/kLexer.cpp
        COMPILE_FLAGS --header=${CMAKE_BINARY_DIR}/kLexer.h )

BISON_TARGET( KalcParser sources/parser.y
        ${CMAKE_BINARY_DIR}/kParser.cpp
        VERBOSE
        )

add_executable( Kalculator
        ${CMAKE_SOURCE_DIR}/main.cpp
        ${FLEX_KalcLexer_OUTPUTS}
        ${BISON_KalcParser_OUTPUTS}
        )
target_include_directories( Kalculator PRIVATE ${CMAKE_BINARY_DIR} )


enable_testing()

file( GLOB test_samples "samples/*.test" )
foreach( file ${test_samples} )
    get_filename_component( nn ${file} NAME )

    add_test( NAME Print_${nn} COMMAND ${CMAKE_BINARY_DIR}/Kalculator print ${file} )
    add_test( NAME Lexer_${nn} COMMAND ${CMAKE_BINARY_DIR}/Kalculator lexer ${file} )
    add_test( NAME Calc_${nn} COMMAND ${CMAKE_BINARY_DIR}/Kalculator calc ${file} )
endforeach()

set_tests_properties( Calc_complex.test PROPERTIES PASS_REGULAR_EXPRESSION "Result is 7" )


add_test( NAME Print_Invalid COMMAND ${CMAKE_BINARY_DIR}/Kalculator print ssd )
set_tests_properties( Print_Invalid PROPERTIES WILL_FAIL true )