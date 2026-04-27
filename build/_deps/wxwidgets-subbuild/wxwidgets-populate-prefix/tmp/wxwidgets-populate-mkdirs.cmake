# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/Program/my_projects/sem2_lab2/build/_deps/wxwidgets-src")
  file(MAKE_DIRECTORY "D:/Program/my_projects/sem2_lab2/build/_deps/wxwidgets-src")
endif()
file(MAKE_DIRECTORY
  "D:/Program/my_projects/sem2_lab2/build/_deps/wxwidgets-build"
  "D:/Program/my_projects/sem2_lab2/build/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix"
  "D:/Program/my_projects/sem2_lab2/build/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix/tmp"
  "D:/Program/my_projects/sem2_lab2/build/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix/src/wxwidgets-populate-stamp"
  "D:/Program/my_projects/sem2_lab2/build/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix/src"
  "D:/Program/my_projects/sem2_lab2/build/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix/src/wxwidgets-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Program/my_projects/sem2_lab2/build/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix/src/wxwidgets-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Program/my_projects/sem2_lab2/build/_deps/wxwidgets-subbuild/wxwidgets-populate-prefix/src/wxwidgets-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
