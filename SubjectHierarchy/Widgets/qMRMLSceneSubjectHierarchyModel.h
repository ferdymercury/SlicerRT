/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

#ifndef __qMRMLSceneSubjectHierarchyModel_h
#define __qMRMLSceneSubjectHierarchyModel_h

// SlicerRt includes
#include "qSlicerSubjectHierarchyModuleWidgetsExport.h"

// MRMLWidgets includes
#include "qMRMLSceneHierarchyModel.h"

class qMRMLSceneSubjectHierarchyModelPrivate;

/// \ingroup Slicer_QtModules_SubjectHierarchy
class Q_SLICER_MODULE_SUBJECTHIERARCHY_WIDGETS_EXPORT qMRMLSceneSubjectHierarchyModel : public qMRMLSceneHierarchyModel
{
  Q_OBJECT

  /// Control in which column vtkMRMLNode::NodeType are displayed (Qt::DecorationRole).
  /// A value of -1 hides it. Hidden by default (value of -1).
  Q_PROPERTY (int nodeTypeColumn READ nodeTypeColumn WRITE setNodeTypeColumn)

public:
  typedef qMRMLSceneHierarchyModel Superclass;
  qMRMLSceneSubjectHierarchyModel(QObject *parent=0);
  virtual ~qMRMLSceneSubjectHierarchyModel();

  /// Function returning the supported MIME types
  virtual QStringList mimeTypes()const;

  /// Function encoding the dragged item to MIME data
  virtual QMimeData* mimeData(const QModelIndexList &indexes)const;

  /// Handles dropping of items
  virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

  /// Fast function that only check the type of the node to know if it can be a child.
  virtual bool canBeAChild(vtkMRMLNode* node)const;

  /// Fast function that only check the type of the node to know if it can be a parent.
  virtual bool canBeAParent(vtkMRMLNode* node)const;

  /// Insert/move node in subject hierarchy under newParent
  virtual bool reparent(vtkMRMLNode* node, vtkMRMLNode* newParent);

  int nodeTypeColumn()const;
  void setNodeTypeColumn(int column);

public slots:
  /// Force update of the scene in the model
  void forceUpdateScene();

signals:
  void saveTreeExpandState();
  void loadTreeExpandState();
  void invalidateModels();

protected:
  /// Get the largest column ID
  virtual int maxColumnId()const;

  /// Overridden function to handle tree view item display from node data
  virtual void updateItemDataFromNode(QStandardItem* item, vtkMRMLNode* node, int column);

  /// Overridden function to handle node update from tree view item
  virtual void updateNodeFromItemData(vtkMRMLNode* node, QStandardItem* item);

private:
  Q_DECLARE_PRIVATE(qMRMLSceneSubjectHierarchyModel);
  Q_DISABLE_COPY(qMRMLSceneSubjectHierarchyModel);
};

#endif