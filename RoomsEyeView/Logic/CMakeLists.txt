project(vtkSlicer${MODULE_NAME}ModuleLogic)

find_package(vtkIECTransformLogic REQUIRED)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_SLICER_${MODULE_NAME_UPPER}_LOGIC_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${SlicerRtCommon_INCLUDE_DIRS}
  ${vtkSlicerSubjectHierarchyModuleLogic_INCLUDE_DIRS}
  ${vtkSlicerBeamsModuleMRML_INCLUDE_DIRS}
  ${vtkSlicerBeamsModuleLogic_INCLUDE_DIRS}
  ${RapidJSON_INCLUDE_DIR}
  ${vtkIECTransformLogic_INCLUDE_DIRS}
  )

set(${KIT}_SRCS
  vtkMRML${MODULE_NAME}Node.cxx
  vtkMRML${MODULE_NAME}Node.h
  vtkSlicer${MODULE_NAME}ModuleLogic.cxx
  vtkSlicer${MODULE_NAME}ModuleLogic.h
  )

set(${KIT}_TARGET_LIBRARIES
  vtkSlicerRtCommon
  vtkSlicerSubjectHierarchyModuleLogic
  vtkSlicerModelsModuleLogic
  vtkSlicerBeamsModuleMRML
  vtkSlicerBeamsModuleLogic
  ${ITK_LIBRARIES}
  ${VTK_LIBRARIES}
  vtkIECTransformLogic
  )

#-----------------------------------------------------------------------------
SlicerMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )

