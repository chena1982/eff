#ifndef EFFEDITOR_H
#define EFFEDITOR_H

#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include "ui_effeditor.h"

#define TOOLBAR_MIN_HEIGHT 16

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


public slots:
	void fileMenuPressed(QAction *);
public:
	void closeEvent(QCloseEvent *event);

private:
	Ui::EFFEditorClass ui;

	QMenuBar *						m_pMainMenuBar;
	EFFEditorInspectorPanel *		m_pInspectorPanel;
	EFFEditorProjecPanel *			m_pProjectPanel;
	EFFEditorHierarchyPanel *		m_pHierarchyPanel;
	EFFEditorScenePanel *			m_pScenePanel;
};

#endif // EFFEDITOR_H
