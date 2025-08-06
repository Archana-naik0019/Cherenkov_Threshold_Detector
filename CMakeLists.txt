
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(Threshold_Cherenkov_copy)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources *.cc)
file(GLOB headers *.hh)

add_executable(Threshold_Cherenkov_copy main.cc ${sources} ${headers})

target_link_libraries(Threshold_Cherenkov_copy ${Geant4_LIBRARIES})

file(GLOB MACRO_FILES macros *.mac)

#file(GLOB DATA_FILES data *.csv)
file(GLOB DATA_FILES data *.dat)
file(COPY ${MACRO_FILES} ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})

#add_custom_target(Threshold_Cherenkov_copy DEPENDS Threshold_Cherenkov_copy)
