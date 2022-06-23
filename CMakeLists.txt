cmake_minimum_required(VERSION 3.23.1)

project(ClinicMP VERSION 0.1.0)

add_library(clinic include/clinic.h)
target_include_directories(clinic PUBLIC include)
set_target_properties(clinic PROPERTIES LINKER_LANGUAGE C)

add_executable(ClinicMP src/main.c)
target_compile_features(ClinicMP PUBLIC c_std_99)

target_link_libraries(ClinicMP PUBLIC clinic)
