cmake_minimum_required(VERSION 3.7)
project(fsin_parser_new)

set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.8.0/5.8/msvc2015_64")
find_package(Qt5Core)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include("${CMAKE_CURRENT_SOURCE_DIR}/../external/xlnt/xlnt.cmake")

SET(SOURCES 
    main.cpp person.cpp person.h realty.cpp realty.h transport.cpp transport.h parser.cpp parser.h 
    excel_adapter.cpp excel_adapter.h organization.cpp organization.h
)

add_executable(fsin_parser ${SOURCES})

target_link_libraries(fsin_parser Qt5::Core)
target_link_libraries(fsin_parser xlnt)

add_custom_command(
    TARGET fsin_parser POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Core>
        $<TARGET_FILE_DIR:fsin_parser>
)