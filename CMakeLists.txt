cmake_minimum_required(VERSION 3.20.0)
project(assembly VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the compiler flags we want to use
set(CMAKE_CXX_FLAGS "-Wall")

# If your LSP does not find the path to your local libs/includes, you can use something like this
# to force the compile_flags.json to direct an include path to it
# set(CMAKE_CXX_FLAGS "-Wall -I/usr/include/c++/11 -I/usr/include/x86_64-linux-gnu/c++/11")

# Source files for the assembly
# NOTE: you may need to add subfolders directly here
#       I use the .cc notation for C++ source files. You can change this to .cpp
file(GLOB ASSEMBLY_SOURCES
    "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.cc"
)

file(GLOB ASSEMBLY_HEADERS
    "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.h"
)

# Build dependencies if you have any

# Set the include path
set(ASSEMBLY_INCLUDE_DIR 
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}"
)

# Compile as either a library or an executable
# Library: note that you can remove the SHARED or change it to STATIC to compile to a static library rather than dynamic
# add_library(${PROJECT_NAME} SHARED ${ASSEMBLY_SOURCES} ${ASSEMBLY_HEADERS})

# Executable
add_executable(${PROJECT_NAME} 
    "${ASSEMBLY_SOURCES}"
    "${ASSEMBLY_HEADERS}"
)

# Set the include directory
target_include_directories(${PROJECT_NAME}
    PUBLIC
    "${ASSEMBLY_INCLUDE_DIR}"
    # include any external dependency include directories here as well...
)

# Build examples
# Here you would do something similar to the above for each example executable you want.
# However, you can either create a CMakeLists.txt in the directory of that example and 
# use add_subdirectory(...) or you can just compile them here directly like we just did above.
# Remember to link any libraries you might need

#Generate compiler commands for using clangd LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
