cmake_minimum_required(VERSION 3.5)

#-----------------------------------------------------------------------------
set(EXTENSION_NAME SlicerRT)

# ------------------------------------------------------------------------
set(SLICERRT_VERSION_MAJOR "1")
set(SLICERRT_VERSION_MINOR "0")
set(SLICERRT_VERSION_PATCH "0")
set(SLICERRT_VERSION ${SLICERRT_VERSION_MAJOR}.${SLICERRT_VERSION_MINOR}.${SLICERRT_VERSION_PATCH})

#-----------------------------------------------------------------------------
# Extension meta-information
set(EXTENSION_HOMEPAGE "https://slicerrt.org")
set(EXTENSION_CATEGORY "Radiotherapy")
set(EXTENSION_CONTRIBUTORS "Csaba Pinter (PerkLab, Queen's University), Andras Lasso (PerkLab, Queen's University), Greg Sharp (Massachusetts General Hospital), Kevin Wang (Radiation Medicine Program, Princess Margaret Hospital, University Health Network Toronto)")
set(EXTENSION_DESCRIPTION "Toolkit for radiation therapy research. Features include DICOM-RT import/export, dose volume histogram, dose accumulation, external beam planning (TPS), structure comparison, isodose line/surface generation, etc. Version ${SLICERRT_VERSION}")
set(EXTENSION_ICONURL "https://www.slicer.org/slicerWiki/images/2/29/SlicerRT_Logo_3.0_128x128.png")
set(EXTENSION_SCREENSHOTURLS "https://www.slicer.org/slicerWiki/images/8/87/SlicerRT_0.10_IsocenterShiftingEvaluation.png https://www.slicer.org/slicerWiki/images/e/ef/SlicerRT_0.11_ProstateLoaded_Beams_ThresholdedDose.png https://www.slicer.org/slicerWiki/images/4/40/SlicerRtFundingSources.png")
set(EXTENSION_STATUS "Beta")
set(EXTENSION_DEPENDS "NA") # Specified as a space separated list or 'NA' if any
set(EXTENSION_BUILD_SUBDIRECTORY inner-build)

set(SUPERBUILD_TOPLEVEL_PROJECT inner)

# Prevent unnecessary re-linking on Linux
#
# See https://discourse.slicer.org/t/build-when-git-commit-is-changed/29607/12
#
# For example, on Linux, after modifying one implementation file
#
# $ touch ../../SlicerRT/SlicerRtCommon/vtkSlicerRtCommon.cxx
#
# We can observe that beside the rebuilding of libvtkSlicerRtCommon.so no relinking
# happen for any of the dependencies:
#
# $ make
# [...]
# [  1%] Building CXX object SlicerRtCommon/CMakeFiles/vtkSlicerRtCommon.dir/vtkSlicerRtCommon.cxx.o
# [  1%] Linking CXX shared library ../lib/Slicer-5.3/qt-loadable-modules/libvtkSlicerRtCommon.so
# [  2%] Built target vtkSlicerRtCommon
# [  2%] Built target vtkSlicerRtCommonHierarchy
# [  4%] Built target vtkSlicerRtCommonPython
# [...]
# [100%] Built target CompileIGRTWorkflow_SelfTestPythonFiles
#
if(NOT DEFINED CMAKE_LINK_DEPENDS_NO_SHARED AND UNIX AND NOT APPLE)
  set(CMAKE_LINK_DEPENDS_NO_SHARED ON)
endif()

#-----------------------------------------------------------------------------
# Extension dependencies
find_package(Slicer COMPONENTS ConfigurePrerequisites REQUIRED)
project(SlicerRT)
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})
mark_as_superbuild(Slicer_DIR)

#-----------------------------------------------------------------------------
option(SLICERRT_ENABLE_EXPERIMENTAL_MODULES "Enable the building of work-in-progress, experimental modules." OFF)
mark_as_superbuild(SLICERRT_ENABLE_EXPERIMENTAL_MODULES)

#-----------------------------------------------------------------------------
option(SLICERRT_DOCUMENTATION "Enable the building of the SlicerRt documentation via doxygen." OFF)

#-----------------------------------------------------------------------------
# SuperBuild setup
option(${EXTENSION_NAME}_SUPERBUILD "Build ${EXTENSION_NAME} and the projects it depends on." ON)
mark_as_advanced(${EXTENSION_NAME}_SUPERBUILD)
if(${EXTENSION_NAME}_SUPERBUILD)
  include("${CMAKE_CURRENT_SOURCE_DIR}/SuperBuild.cmake")
  return()
endif()

#-----------------------------------------------------------------------------
# Plastimatch_DIR is only used now by experimental modules. Print the
# Plastimatch_DIR, to avoid the warning "Manually-specified variables were not
# used by the project: Plastimatch_DIR" warning
message(STATUS "Plastimatch_DIR: " ${Plastimatch_DIR} )
message(STATUS "SLICERRT_ENABLE_EXPERIMENTAL_MODULES: " ${SLICERRT_ENABLE_EXPERIMENTAL_MODULES} )

#-----------------------------------------------------------------------------
add_subdirectory(SlicerRtCommon)
add_subdirectory(PlmCommon)

add_subdirectory(Beams)
add_subdirectory(Isodose)
add_subdirectory(PlanarImage)
add_subdirectory(DicomRtImportExport)
add_subdirectory(DoseAccumulation)
add_subdirectory(DoseVolumeHistogram)
add_subdirectory(DvhComparison)
add_subdirectory(DoseComparison)
add_subdirectory(SegmentComparison)
add_subdirectory(ExternalBeamPlanning)
add_subdirectory(PlmProtonDoseEngine)
add_subdirectory(VffFileReader)
add_subdirectory(DosxyzNrc3dDoseFileReader)
add_subdirectory(DicomSroImportExport)
add_subdirectory(BatchProcessing)
add_subdirectory(RoomsEyeView)
add_subdirectory(DrrImageComputation)

add_subdirectory(PlastimatchPy)
add_subdirectory(PlmBspline)
add_subdirectory(PlmDrr)
add_subdirectory(PlmLandwarp)
add_subdirectory(PlmRegister)

if (SLICERRT_ENABLE_EXPERIMENTAL_MODULES)
  # In development
  add_subdirectory(PlmThreshbox)
  add_subdirectory(PinnacleDvfReader)

  # Registration validation routines
  add_subdirectory(PlmSynth)
  add_subdirectory(PlmVectorFieldAnalysis)
endif()

if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()

if (SLICERRT_DOCUMENTATION)
  add_subdirectory(Docs)
endif()

#-----------------------------------------------------------------------------
set(EXTENSION_CPACK_INSTALL_CMAKE_PROJECTS)
list(APPEND EXTENSION_CPACK_INSTALL_CMAKE_PROJECTS "${vtkIECTransformLogic_DIR};vtkIECTransformLogic;RuntimeLibraries;/")
set(${EXTENSION_NAME}_CPACK_INSTALL_CMAKE_PROJECTS "${EXTENSION_CPACK_INSTALL_CMAKE_PROJECTS}" CACHE STRING "List of external projects to install" FORCE)
list(APPEND CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};${EXTENSION_NAME};ALL;/")
list(APPEND CPACK_INSTALL_CMAKE_PROJECTS "${${EXTENSION_NAME}_CPACK_INSTALL_CMAKE_PROJECTS}")

include(${Slicer_EXTENSION_GENERATE_CONFIG})
include(${Slicer_EXTENSION_CPACK})
