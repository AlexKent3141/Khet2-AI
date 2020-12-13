
add_library(khet_core
  SHARED
    Board.cpp
    Calculator.cpp
    Evaluator.cpp
    Laser.cpp
    MoveGenerator.cpp
    Search.cpp
    Utils.cpp)

if (MSVC)
else()
  target_link_libraries(khet_core
    pthread)
endif()
