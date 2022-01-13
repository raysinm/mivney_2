# The first line states the minimum CMake version to use with this project
# and is requires by CMake.
cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 11)

# Set <PROJNAME> to be your project name. We set CXX (C++) here as the target language.
project(<PROJNAME> VERSION 0.1.0 LANGUAGES CXX)

# If you want to see "under the hood" of CMake, uncomment this:
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Define variables containing compiler flags for gcc
set(MIVNEY_FLAGS_DEBUG "-std=c++11 --pedantic-errors -Wall -Werror")
set(MIVNEY_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")

# Set the flags for gcc
set(CMAKE_CPP_FLAGS ${MIVNEY_FLAGS_DEBUG})


# Add the names of your source files here
set(CPP_FILES_1 "./hash_tests/DynamicHashtable.h" "./hash_tests/minimain.cpp")
set(CPP_FILES_1_1 "./hash_tests/DynamicHashtable.h" "./hash_tests/HashTest/main.cpp")

set(CPP_FILES_2 "./unionfind_tests/UnionFind.h" "./unionfind_tests/main.cpp")

set(CPP_FILES_3 "./course_files/library2.h" "./PlayersManager.h" "./DynamicHashtable.h" "./UnionFind.h" "./course_files/main2.cpp" "./PlayersManager.cpp" "./library2.cpp")

add_executable(hash_mini_test ${CPP_FILES_1})
add_executable(hash_big_test ${CPP_FILES_1_1})
add_executable(uf_test ${CPP_FILES_2})
add_executable(pm_main ${CPP_FILES_3})
