add_executable(hello_animation hello_animation.cc)
target_link_libraries (hello_animation ccanimation)

add_test (NAME hello_animation COMMAND hello_animation)