
if (MSVC)
else()
  add_compile_options(
    -Wall -Wpedantic -Werror -std=c++11 -O3)
endif()

add_subdirectory(
  "./tests")

add_subdirectory(
  "./core")

add_executable(khet
  CommsHandler.cpp
  main.cpp)

target_link_libraries(khet
  PRIVATE
    khet_core)
