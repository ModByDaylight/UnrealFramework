cmake_minimum_required(VERSION 3.18)
project(UnrealFramework)

option(UNREAL_BUILD_SHARED_LIB "Build UnrealFramework as a shared library" ON)
option(UNREAL_VERSION "Unreal Version to Build For" 4272)

set(UNREAL_TARGET "UnrealFramework")
file(GLOB_RECURSE SOURCES src/**.cpp)
file(GLOB_RECURSE GENERATED_SOURCES generated_src/**.cpp)

if (UNREAL_BUILD_SHARED_LIB)
    add_library(${UNREAL_TARGET} SHARED ${SOURCES} ${GENERATED_SOURCES})
    target_compile_definitions(${UNREAL_TARGET} PRIVATE RC_UNREAL_EXPORTS RC_DYNAMIC_OUTPUT_EXPORTS RC_SIG_SCANNER_EXPORTS RC_FILE_EXPORTS)
else ()
    message("Creating static ${TARGET} library")
    add_library(${UNREAL_TARGET} STATIC ${SOURCES} ${GENERATED_SOURCES} include/Unreal/PlatformMisc.hpp)
    target_compile_definitions(${UNREAL_TARGET} PUBLIC RC_UNREAL_BUILD_STATIC RC_DYNAMIC_OUTPUT_BUILD_STATIC RC_SIG_SCANNER_BUILD_STATIC RC_FILE_BUILD_STATIC)
endif ()

target_compile_definitions(${UNREAL_TARGET} PUBLIC UNREAL_VERSION=${UNREAL_VERSION})
if (UNREAL_WITH_CASE_PRESERVING_NAME)
    target_compile_definitions(${UNREAL_TARGET} PUBLIC WITH_CASE_PRESERVING_NAME)
endif()

target_include_directories(${UNREAL_TARGET} PUBLIC include)
target_include_directories(${UNREAL_TARGET} PUBLIC generated_include)

set_target_properties(${UNREAL_TARGET} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
target_compile_options(${UNREAL_TARGET} PRIVATE $<$<CONFIG:RELEASE>:/Zi> /W4 /wd4005)
target_link_options(${UNREAL_TARGET} PRIVATE /DEBUG:FULL)
target_compile_features(${UNREAL_TARGET} PUBLIC cxx_std_20)

# PolyHook2 -> START
set(POLYHOOK_BUILD_SHARED_LIB ON CACHE BOOL "")
set(POLYHOOK_BUILD_STATIC_RUNTIME OFF CACHE BOOL "")

set(POLYHOOK2_DIR dependencies/stevemk14ebr/polyhook_2_0)
add_subdirectory(${POLYHOOK2_DIR})
target_include_directories(${UNREAL_TARGET} PUBLIC ${POLYHOOK2_DIR})

target_link_libraries(${UNREAL_TARGET} PUBLIC PolyHook_2)
# PolyHook2 -> END
