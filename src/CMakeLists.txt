
if (MSVC)
# TODO
else()
  add_compile_options(
    -Wall -Wpedantic -Werror -std=c++11 -O3)
endif()

add_executable(khet
  Board.cpp
  Calculator.cpp
  CommsHandler.cpp
  Evaluator.cpp
  Laser.cpp
  MoveGenerator.cpp
  Search.cpp
  Utils.cpp
  main.cpp)

if (MSVC)
# TODO
else()
  target_link_libraries(khet
    pthread)
endif()
