project(vtkSlicerRtCommon)

# --------------------------------------------------------------------------
# Configure headers
# --------------------------------------------------------------------------
set(configure_header_file vtkSlicerRtCommonConfigure.h)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/${configure_header_file}.in
  ${CMAKE_CURRENT_BINARY_DIR}/${configure_header_file}
  )

# --------------------------------------------------------------------------
# Sources
# --------------------------------------------------------------------------

SET (SlicerRtCommon_SRCS
  vtkSlicerRtCommon.cxx
  vtkSlicerRtCommon.h
  vtkSlicerRtCommon.txx
  vtkLabelmapToModelFilter.cxx
  vtkLabelmapToModelFilter.h
  vtkPolyDataToLabelmapFilter.cxx
  vtkPolyDataToLabelmapFilter.h
  vtkSlicerAutoWindowLevelLogic.cxx
  vtkSlicerAutoWindowLevelLogic.h
  vtkFractionalImageAccumulate.cxx
  vtkFractionalImageAccumulate.h
  vtkSlicerDicomReaderBase.cxx
  vtkSlicerDicomReaderBase.h
  vtkSlicerDicomReaderBase.txx
  )

SET (SlicerRtCommon_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${Slicer_Libs_INCLUDE_DIRS} ${vtkSegmentationCore_INCLUDE_DIRS} CACHE INTERNAL "" FORCE)

# --------------------------------------------------------------------------
# Build the library
# --------------------------------------------------------------------------

set(lib_name ${PROJECT_NAME})
SET (SlicerRtCommon_LIBS ${SlicerRtCommon_LIBS}
  ${VTK_LIBRARIES}
  MRMLCore
  vtkSegmentationCore
  )

INCLUDE_DIRECTORIES( ${SlicerRtCommon_INCLUDE_DIRS} )
ADD_LIBRARY(${lib_name} ${SlicerRtCommon_SRCS})
TARGET_LINK_LIBRARIES( ${lib_name} ${SlicerRtCommon_LIBS} )

# Set loadable modules output
set_target_properties(${lib_name} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_BIN_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_LIB_DIR}"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_LIB_DIR}"
  )

IF (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" AND NOT WIN32)
  SET_TARGET_PROPERTIES(${lib_name} PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF()

set_property(GLOBAL APPEND PROPERTY Slicer_TARGETS ${lib_name})

# --------------------------------------------------------------------------
# Install library
# --------------------------------------------------------------------------
install(TARGETS ${lib_name}
  RUNTIME DESTINATION ${Slicer_INSTALL_QTLOADABLEMODULES_BIN_DIR} COMPONENT RuntimeLibraries
  LIBRARY DESTINATION ${Slicer_INSTALL_QTLOADABLEMODULES_LIB_DIR} COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION ${Slicer_INSTALL_QTLOADABLEMODULES_LIB_DIR} COMPONENT Development
  )

# --------------------------------------------------------------------------
# Python wrapping
# --------------------------------------------------------------------------
if(VTK_WRAP_PYTHON AND BUILD_SHARED_LIBS)

  set(Slicer_Wrapped_LIBRARIES
    )

  if(${VTK_VERSION} VERSION_LESS "8.90")
    foreach(library ${lib_name})
      if(TARGET ${library}PythonD)
        list(APPEND Slicer_Wrapped_LIBRARIES ${library}PythonD)
      endif()
    endforeach()
  endif()

  SlicerMacroPythonWrapModuleVTKLibrary(
    NAME ${PROJECT_NAME}
    SRCS ${SlicerRtCommon_SRCS}
    WRAPPED_TARGET_LIBRARIES ${Slicer_Wrapped_LIBRARIES}
    RELATIVE_PYTHON_DIR "."
    )

  # Set python module logic output
  set_target_properties(${PROJECT_NAME}Python ${PROJECT_NAME}Python PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_BIN_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_LIB_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_LIB_DIR}"
    )
  if(${VTK_VERSION} VERSION_LESS "8.90")
    set_target_properties(${PROJECT_NAME}PythonD PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_BIN_DIR}"
      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_LIB_DIR}"
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_LIB_DIR}"
      )
  endif()

  # Export target
  set_property(GLOBAL APPEND PROPERTY Slicer_TARGETS ${PROJECT_NAME}Python)
  if(${VTK_VERSION} VERSION_LESS "8.90")
    set_property(GLOBAL APPEND PROPERTY Slicer_TARGETS ${PROJECT_NAME}PythonD)
  endif()

endif()
