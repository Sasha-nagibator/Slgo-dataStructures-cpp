# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/aleksandr/Documents/6sem/baza/garbage/Algo-DataStructures/hash/cmake-build-debug/_deps/doctest-src"
  "/home/aleksandr/Documents/6sem/baza/garbage/Algo-DataStructures/hash/cmake-build-debug/_deps/doctest-build"
  "/home/aleksandr/Documents/6sem/baza/garbage/Algo-DataStructures/hash/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix"
  "/home/aleksandr/Documents/6sem/baza/garbage/Algo-DataStructures/hash/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix/tmp"
  "/home/aleksandr/Documents/6sem/baza/garbage/Algo-DataStructures/hash/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp"
  "/home/aleksandr/Documents/6sem/baza/garbage/Algo-DataStructures/hash/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix/src"
  "/home/aleksandr/Documents/6sem/baza/garbage/Algo-DataStructures/hash/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/aleksandr/Documents/6sem/baza/garbage/Algo-DataStructures/hash/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/${subDir}")
endforeach()
