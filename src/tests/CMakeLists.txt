Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/catch2.git
  GIT_TAG v2.13.1)

FetchContent_MakeAvailable(Catch2)

add_executable(khet_test
  MakeMoveTests.cpp
  MoveGenTests.cpp
  TerminationTests.cpp
  catch_main.cpp)

target_link_libraries(khet_test
  PRIVATE
    khet_core
    Catch2::Catch2)
