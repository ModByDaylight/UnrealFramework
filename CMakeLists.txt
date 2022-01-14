cmake_minimum_required(VERSION 3.18)
message("Project: Unreal")

set(RC_VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/local_repo_path.build_option")
    # Path to RC is not defined, so we'll assume that git submodules are being used
    set(RC_LOCAL_REPO_PATH_INTERNAL ${RC_VENDOR_DIR}/RC)
    set(RC_COMMON_CMAKE_FILE ${RC_VENDOR_DIR}/RC/CMake/RC_Common.cmake)
    message("Path to RC repo not defined. Using default: '${RC_LOCAL_REPO_PATH_INTERNAL}'")
else()
    file(READ ${CMAKE_SOURCE_DIR}/local_repo_path.build_option RC_LOCAL_REPO_PATH_FROM_FILE)
    if (IS_ABSOLUTE ${RC_LOCAL_REPO_PATH_FROM_FILE})
        set(RC_LOCAL_REPO_PATH_INTERNAL ${RC_LOCAL_REPO_PATH_FROM_FILE})

    else ()
        set(RC_LOCAL_REPO_PATH_INTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/${RC_LOCAL_REPO_PATH_FROM_FILE})
    endif ()
    set(RC_COMMON_CMAKE_FILE ${RC_LOCAL_REPO_PATH_INTERNAL}/CMake/RC_Common.cmake)
    message("Path for the 'RC' repo overridden, RC_LOCAL_REPO_PATH=${RC_LOCAL_REPO_PATH_INTERNAL}")
endif()

set(TARGET Unreal)
project(${TARGET})

include(${RC_COMMON_CMAKE_FILE})

set(SOURCES
        "src/StaticOffsetFinder.cpp"
        "src/Signatures.cpp"
        "src/VersionedContainer/Derived412.cpp"
        "src/VersionedContainer/UObject412.cpp"
        "src/VersionedContainer/Flags412.cpp"
        "src/VersionedContainer/Derived415.cpp"
        "src/VersionedContainer/UObject415.cpp"
        "src/VersionedContainer/Flags415.cpp"
        "src/VersionedContainer/Derived422.cpp"
        "src/VersionedContainer/UObject422.cpp"
        "src/VersionedContainer/Flags422.cpp"
        "src/VersionedContainer/Derived425.cpp"
        "src/VersionedContainer/UObject425.cpp"
        "src/VersionedContainer/Flags425.cpp"
        "src/NameTypes.cpp"
        "src/UnrealVersion.cpp"
        "src/UObjectArray.cpp"
        "src/UObjectGlobals.cpp"
        "src/UObjectBaseUtility.cpp"
        "src/UObject.cpp"
        "src/UPackage.cpp"
        "src/UFunction.cpp"
        "src/AActor.cpp"
        "src/UClass.cpp"
        "src/FField.cpp"
        "src/UField.cpp"
        "src/UStruct.cpp"
        "src/UScriptStruct.cpp"
        "src/UnrealInitializer.cpp"
        "src/TypeChecker.cpp"
        "src/XProperty.cpp"
        "src/TArray.cpp"
        "src/UEnum.cpp"
        "src/XStructData.cpp"
        "src/FWeakObjectPtr.cpp"
        "src/FText.cpp"
        "src/FString.cpp"
        "src/CustomType.cpp"
        "src/PropertyReaders.cpp"
        "src/Property/XNumericProperty.cpp"
        "src/Property/XArrayProperty.cpp"
        "src/Property/XStructProperty.cpp"
        "src/Property/XObjectProperty.cpp"
        "src/Property/XWeakObjectProperty.cpp"
        "src/Property/XLazyObjectProperty.cpp"
        "src/Property/XSoftObjectProperty.cpp"
        "src/Property/XClassProperty.cpp"
        "src/Property/XSoftClassProperty.cpp"
        "src/Property/XByteProperty.cpp"
        "src/Property/XEnumProperty.cpp"
        "src/Property/XMapProperty.cpp"
        "src/Property/XTextProperty.cpp"
        "src/Property/XStrProperty.cpp"
        "src/Property/XDelegateProperty.cpp"
        "src/Property/XMulticastInlineDelegateProperty.cpp"
        "src/Property/XMulticastSparseDelegateProperty.cpp"
        "src/Property/XInterfaceProperty.cpp"
        "src/Property/XFieldPathProperty.cpp"
        "src/Property/XSetProperty.cpp"
        "src/Hooks.cpp"
        "src/ReflectedFunction.cpp"
        "src/UAssetRegistry.cpp"
        "src/UAssetRegistryHelpers.cpp"
        "src/FMemory.cpp"
        )

if (${RC_UNREAL_SHARED_LIBS} EQUAL 1)
    message("Creating shared ${TARGET} library")
    add_library(${TARGET} SHARED ${SOURCES})
    target_compile_definitions(${TARGET} PUBLIC RC_EXPORT)
else ()
    message("Creating static ${TARGET} library")
    add_library(${TARGET} OBJECT ${SOURCES})
    target_compile_options(${TARGET} PUBLIC /MT$<$<CONFIG:DEBUG>:d>)
    target_link_options(${TARGET} PUBLIC /NODEFAULTLIB:MSVCRT)
endif ()


add_rc_dependency_to_target(${TARGET} Helpers)
add_rc_dependency_to_target(${TARGET} File)
add_rc_dependency_to_target(${TARGET} Function)
add_rc_dependency_to_target(${TARGET} Constructs)
add_rc_dependency_to_target(${TARGET} SinglePassSigScanner)
add_rc_dependency_to_target(${TARGET} ScopedTimer)
add_rc_dependency_to_target(${TARGET} DynamicOutput)

target_include_directories(${TARGET} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_options(${TARGET} PRIVATE /MP)
target_compile_features(${TARGET} PUBLIC cxx_std_20)

# Third-Party Dependencies
if (${RC_UNREAL_SHARED_LIBS} EQUAL 1)
    set(RC_UNREAL_BUILD_AS_SHARED_LIB ON)
    set(RC_UNREAL_BUILD_AS_STATIC_LIB OFF)
else ()
    set(RC_UNREAL_BUILD_AS_SHARED_LIB OFF)
    set(RC_UNREAL_BUILD_AS_STATIC_LIB ON)
endif ()

# PolyHook2 -> START
set(POLYHOOK_BUILD_DLL ON CACHE BOOL "Build dll & lib instead of tests" FORCE)
set(POLYHOOK_BUILD_SHARED_LIB ${RC_UNREAL_BUILD_AS_SHARED_LIB} CACHE BOOL "Build polyhook as shared libary" FORCE)
set(POLYHOOK_BUILD_SHARED_ASMJIT ${RC_UNREAL_BUILD_AS_SHARED_LIB} CACHE BOOL "Build polyhook as shared libary" FORCE)
set(POLYHOOK_BUILD_SHARED_CAPSTONE ${RC_UNREAL_BUILD_AS_SHARED_LIB} CACHE BOOL "Build polyhook as shared libary" FORCE)
set(POLYHOOK_BUILD_SHARED_ZYDIS ${RC_UNREAL_BUILD_AS_SHARED_LIB} CACHE BOOL "Build polyhook as shared libary" FORCE)
set(POLYHOOK_BUILD_STATIC_RUNTIME ${RC_UNREAL_BUILD_AS_STATIC_LIB} CACHE BOOL "Use static runtime" FORCE)

set(POLYHOOK_DISASM_CAPSTONE OFF CACHE BOOL "Enable capstone support" FORCE)
set(POLYHOOK_DISASM_CAPSTONE_FULL OFF CACHE BOOL "Build all features of capstone." FORCE)
set(POLYHOOK_DISASM_ZYDIS ON CACHE BOOL "Enable Zydis support." FORCE)

set(PolyHook2Loc ${RC_VENDOR_DIR}/stevemk14ebr/PolyHook_2_0)
add_subdirectory(${PolyHook2Loc} ${CMAKE_CURRENT_BINARY_DIR}/vendor/stevemk14ebr/PolyHook_2_0)
target_include_directories(${TARGET} PUBLIC ${PolyHook2Loc})
# For some reason, PolyHook isn't adding the capstone or zydis include directories
target_include_directories(${TARGET} PUBLIC ${PolyHook2Loc}/capstone/include)
target_include_directories(${TARGET} PUBLIC ${PolyHook2Loc}/zydis/include)
target_include_directories(${TARGET} PUBLIC ${PolyHook2Loc}/zydis/dependencies/zycore/include)
target_include_directories(${TARGET} PUBLIC ${PolyHook2Loc}/zydis/msvc)
target_link_libraries(${TARGET} PRIVATE PolyHook_2)

if (${RC_UNREAL_SHARED_LIBS} EQUAL 1)
    copy_bin(${TARGET} "${CMAKE_CURRENT_BINARY_DIR}/vendor/stevemk14ebr/PolyHook_2_0/PolyHook_2.dll" "${RC_OUTPUT_DIR}/PolyHook_2.dll")
endif ()
# PolyHook2 -> END
