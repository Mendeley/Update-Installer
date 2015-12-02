
# Set the deployment target
# According to
#   https://cmake.org/cmake/help/v3.0/variable/CMAKE_OSX_DEPLOYMENT_TARGET.html
# this has to be done before the first project() invocation.
#
# However, inside the mendeley desktop build system this means that the following
# line will actually be ignored, since we're doing that already on the top level
# of the build system.

if(SUPPORT_OSX_SNOW_LEOPARD)
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.6)
else()
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
endif()

project(updater)

cmake_minimum_required(VERSION 2.6)
enable_testing()
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# If the SIGN_UPDATER option is enabled, the updater.exe
# binary is digitally signed on Windows after it has been built.
#
# The script used to sign the binary is specified by BINARY_SIGNING_TOOL.
# This tool must take a single argument which is the filename of the
# binary to sign.
option(SIGN_UPDATER "Enable signing of the updater binary" OFF)
set(BINARY_SIGNING_TOOL sign-updater.bat CACHE PATH "Path to the tool used to sign the updater")

include_directories(external)
include_directories(external/TinyThread/source)

if (WIN32)
    include_directories(external/zlib/)
    include_directories(external/bzip2)
    include_directories(external/win32cpp/include)

    # - Link the updater binary statically with the Visual C++ runtime
    #   so that the executable can function standalone.
    # - Enable PDB generation for release builds
    #
    # The /MT flag is specified separately for each build
    # type rather than setting it in the CMAKE_(C|CXX)_FLAGS vars
    # because otherwise they are overridden by the /MT switch in the
    # default flags for CMAKE_(C|CXX)_FLAGS_(DEBUG|RELWITHDEBINFO)
    set(CMAKE_CXX_FLAGS_DEBUG "/MT")
    set(CMAKE_C_FLAGS_DEBUG "/MT")

    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "/MT")

    set(CMAKE_CXX_FLAGS_RELEASE "/MT /Zi /O2 /Ob2 /D NDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "/MT /Zi /O2 /Ob2 /D NDEBUG")

    remove_definitions(-DUNICODE -D_UNICODE)
else()
    # optimize for reduced code size
    set(CMAKE_CXX_FLAGS_RELEASE "-Os")
    set(CMAKE_C_FLAGS_RELEASE "-Os")
endif()

if (APPLE)
    # Build the updater as a dual 32/64bit binary.  If only one architecture
    # is required, removing the other architecture will reduce the size
    # of the updater binary
    set(CMAKE_OSX_ARCHITECTURES i386;x86_64)
endif()

add_subdirectory(src)
add_subdirectory(external/AnyOption)
add_subdirectory(external/minizip)
add_subdirectory(external/tinyxml)
add_subdirectory(external/TinyThread)

