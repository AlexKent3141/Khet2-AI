
add_executable(khet_test
  test.cpp)

target_link_libraries(khet_test
  PRIVATE
    khet_core)
