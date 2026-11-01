cmake_minimum_required(VERSION 3.20)

project(minty_v2 LANGUAGES CXX)

find_package(SQLite3 REQUIRED)

add_executable(minty
  main.cpp
  ./src/livro.cpp
  ./src/database.cpp
)

target_link_libraries(minty
  PRIVATE
  SQLite3::SQLite3
)
