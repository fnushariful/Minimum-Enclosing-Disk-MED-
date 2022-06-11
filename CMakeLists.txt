cmake_minimum_required(VERSION 3.21)
project(FinalExam_MED)

include_directories(/opt/homebrew/Cellar)
link_directories(/opt/homebrew/lib/)
link_directories(/opt/homebrew/Cellar/)
#link_directories(opt/homebrew/include)

find_package(CGAL)
find_package(OpenMP)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6Quick REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Xpreprocessor -fopenmp -std=c++11 -fexceptions -Wall -Wextra -O2 -stdlib=libc++ -Wno-unused-parameter -Wno-macro-redefined")
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.2.2/lib/cmake/")
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.2.2/QtWidgets/")

add_executable(FinalExam_MED main.cpp CGALComponents-1.h MED.h MEDwithSpatialSortVSwithoutSpatialsort.h SmallestEnclosingCircle.h)

target_link_libraries(FinalExam_MED libgmp.a)
target_link_libraries(FinalExam_MED libmpfr.a)
target_link_libraries(FinalExam_MED CGAL)
target_link_libraries(FinalExam_MED libomp.a)
target_link_libraries(FinalExam_MED Qt6::Core)
target_link_libraries(FinalExam_MED Qt6::Widgets)
target_link_libraries(FinalExam_MED Qt6::Quick)