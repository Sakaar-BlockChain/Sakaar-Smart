cmake_minimum_required(VERSION 3.24)
project(Sakaar_SmartContract C)

set(CMAKE_C_STANDARD 99)

if(NOT TARGET Sakaar_Memory_lib)
    add_subdirectory(lib/Sakaar-Memory)
    link_directories(lib/Sakaar-Memory)
    MESSAGE(STATUS "Connected: Sakaar_Memory_lib")
endif()

if(NOT TARGET Sakaar_Struct_lib)
    add_subdirectory(lib/Sakaar-Struct)
    link_directories(lib/Sakaar-Struct)
    MESSAGE(STATUS "Connected: Sakaar_Struct_lib")
endif()

#if(NOT TARGET Sakaar_CryptoCode_lib)
#    add_subdirectory(lib/Sakaar-CryptoCode)
#    link_directories(lib/Sakaar-CryptoCode)
#    MESSAGE(STATUS "Connected: Sakaar_CryptoCode_lib")
#endif()
#
#if(NOT TARGET Sakaar_HashCode_lib)
#    add_subdirectory(lib/Sakaar-HashCode)
#    link_directories(lib/Sakaar-HashCode)
#    MESSAGE(STATUS "Connected: Sakaar_HashCode_lib")
#endif()
#
#if(NOT TARGET Sakaar_CryptoBase_lib)
#    add_subdirectory(lib/Sakaar-CryptoBase)
#    link_directories(lib/Sakaar-CryptoBase)
#    MESSAGE(STATUS "Connected: Sakaar_CryptoBase_lib")
#endif()
#
#if(NOT TARGET Sakaar_Network_lib)
#    add_subdirectory(lib/Sakaar-NetWork)
#    link_directories(lib/Sakaar-NetWork)
#    MESSAGE(STATUS "Connected: Sakaar_Network_lib")
#endif()
#
#if(NOT TARGET Sakaar_lib)
#    add_subdirectory(lib/Sakaar)
#    link_directories(lib/Sakaar)
#    MESSAGE(STATUS "Connected: Sakaar_lib")
#endif()


FILE(GLOB SOURCE_FILES_DIR_DIR "src/*/*/*.c")
FILE(GLOB SOURCE_FILES_DIR "src/*/*.c")
FILE(GLOB SOURCE_FILES "src/*.c")
add_library(${PROJECT_NAME}_lib ${SOURCE_FILES} ${SOURCE_FILES_DIR} ${SOURCE_FILES_DIR_DIR})
target_include_directories(${PROJECT_NAME}_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)
MESSAGE(STATUS "Connected: Sakaar_Struct_lib")

target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_Memory_lib)
target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_Struct_lib)
#target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_CryptoCode_lib)
#target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_HashCode_lib)
#target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_CryptoBase_lib)
#target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_Network_lib)
#target_link_libraries(${PROJECT_NAME}_lib PUBLIC Sakaar_lib)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)