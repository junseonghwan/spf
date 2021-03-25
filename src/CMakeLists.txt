file( GLOB SRCS *.cpp *.hpp)
add_library(SPF STATIC ${SRCS})
#add_library(SPF SHARED ${SRCS})

find_package (PkgConfig REQUIRED)
find_package(Boost 1.67 COMPONENTS regex unit_test_framework REQUIRED)
find_package (GSL REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
endif()
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

target_link_libraries(SPF GSL::gsl GSL::gslcblas OpenMP::OpenMP_CXX)

file( GLOB PUBLIC_INCLUDE *.hpp) # grab all hpp files
install(FILES ${PUBLIC_INCLUDE} DESTINATION include/spf) # copy it to include/spf
install(TARGETS SPF DESTINATION bin) # install .a file to bin/
