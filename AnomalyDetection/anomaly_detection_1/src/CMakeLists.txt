
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Utilities)
include_directories(${CMAKE_SOURCE_DIR}/model)

add_executable(main_model main_model.cpp)
add_executable(main_detection main_detection.cpp)

#message("-- Message LAPACK libs: " ${LAPACK_LIBRARIES})
#message("-- LAPACK incs: " ${LAPACK_INCLUDES})
include_directories(${LAPACK_INCLUDES})
include_directories(${CBLAS_INCLUDE_DIR})
include_directories(${FFTW_INCLUDEs})

link_directories(${CMAKE_BINARY_DIR}/lib)
target_link_libraries(main_model modelLibrary png ${LAPACK_LIBRARIES} ${CBLAS_LIBRARIES} ${FFTW_LIBRARIES})
target_link_libraries(main_detection modelLibrary png ${LAPACK_LIBRARIES} ${CBLAS_LIBRARIES} ${FFTW_LIBRARIES})

# Copy the script files to the binaries folder
configure_file(Utilities/visu.py ${CMAKE_BINARY_DIR}/bin/visu.py COPYONLY)
configure_file(Utilities/decompose.py ${CMAKE_BINARY_DIR}/bin/decompose.py COPYONLY)
configure_file(Utilities/convert.py ${CMAKE_BINARY_DIR}/bin/convert.py COPYONLY)

configure_file(Utilities/load_network.py ${CMAKE_BINARY_DIR}/bin/load_network.py COPYONLY)
configure_file(Utilities/output_from_network.py ${CMAKE_BINARY_DIR}/bin/output_from_network.py COPYONLY)
configure_file(Utilities/project_pca_upper.py ${CMAKE_BINARY_DIR}/bin/project_pca_upper.py COPYONLY)

configure_file(anomaly_detection.sh ${CMAKE_BINARY_DIR}/bin/anomaly_detection.sh COPYONLY)
