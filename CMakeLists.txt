cmake_minimum_required(VERSION 3.18)
project(text_editor VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion")
include(CTest)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()

set(CPP_SRCS
  app.cpp
  frame.cpp
  editor.cpp
)

add_executable(${PROJECT_NAME} ${CPP_SRCS})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})


set(TEST_SRCS
  tests/editor_test.cpp
  tests/catch_amalgamated.cpp
  editor.cpp
)

add_executable(editor_tests ${TEST_SRCS})
target_include_directories(editor_tests PRIVATE tests .)
target_link_libraries(editor_tests ${wxWidgets_LIBRARIES})

