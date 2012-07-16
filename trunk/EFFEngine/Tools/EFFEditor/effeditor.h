#ifndef EFFEDITOR_H
#define EFFEDITOR_H

#include <QtGui/QMainWindow>
#include "ui_effeditor.h"


class EFFEditorInspectorPanel;
class EFFEditorProjecPanel;
class EFFEditorHierarchyPanel;
class EFFEditorScenePanel;

class EFFEditorMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	EFFEditorMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~EFFEditorMainWindow();

public:
	bool LoadLayout(const QString & filePath);
	EFFEditorInspectorPanel * GetInspectorPanel() { return m_pInspectorPanel; }
	EFFEditorHierarchyPanel * GetHierarchyPanel() { return m_pHierarchyPanel; }
public:
	void closeEvent(QCloseEvent *event);
private:
	Ui::EFFEditorClass ui;

	EFFEditorInspectorPanel *					m_pInspectorPanel;
	EFFEditorProjecPanel *						m_pProjectPanel;
	EFFEditorHierarchyPanel *					m_pHierarchyPanel;
	EFFEditorScenePanel *						m_pScenePanel;
};

#endif // EFFEDITOR_H
