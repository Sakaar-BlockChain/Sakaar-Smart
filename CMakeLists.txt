cmake_minimum_required(VERSION 3.24)
project(Sakaar_Smart C)

set(CMAKE_C_STANDARD 99)

FILE(GLOB SOURCE_FILES_DIR_DIR "src/*/*/*.c")
FILE(GLOB SOURCE_FILES_DIR "src/*/*.c")
FILE(GLOB SOURCE_FILES "src/*.c")
add_library(${PROJECT_NAME}_lib ${SOURCE_FILES} ${SOURCE_FILES_DIR} ${SOURCE_FILES_DIR_DIR})
target_include_directories(${PROJECT_NAME}_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)
MESSAGE(STATUS "Created: ${PROJECT_NAME}_lib")

target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_Struct_lib)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_Network_lib)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_HashCode_lib)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_CryptoCode_lib)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_CryptoBase_lib)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)