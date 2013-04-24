#include "stdafx.h"
#include "effeditor.h"

#include "EFFEditorInspectorPanel.h"
#include "EFFEditorProjectPanel.h"
#include "EFFEditorHierarchyPanel.h"
#include "EFFEditorScenePanel.h"

extern HANDLE g_hRenderThread;
extern effBOOL g_bExitRenderThread;


EFFEditorMainWindow::EFFEditorMainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	//ui.setupUi(this);

	//setDocumentMode(true);


	//setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);
	setDockNestingEnabled(true);

	//m_pMainMenuBar = new QMenuBar(this);
	//SetMenu(m_pMainMenuBar);

	QMenu * fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(new QAction(tr("Save"), fileMenu));

	connect(fileMenu, SIGNAL(triggered(QAction *)), this, SLOT(fileMenuPressed(QAction *)));

	m_pScenePanel = new EFFEditorScenePanel(this);
	addDockWidget(Qt::LeftDockWidgetArea, m_pScenePanel);

	m_pProjectPanel = new EFFEditorProjecPanel(this);
	addDockWidget(Qt::BottomDockWidgetArea, m_pProjectPanel);

	m_pHierarchyPanel = new EFFEditorHierarchyPanel(this);
	addDockWidget(Qt::BottomDockWidgetArea, m_pHierarchyPanel);

	m_pInspectorPanel = new EFFEditorInspectorPanel(this);
	addDockWidget(Qt::RightDockWidgetArea, m_pInspectorPanel, Qt::Vertical);


	//tabifyDockWidget(m_pInspectorPanel, m_pProjectPanel);
	/*setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);
	setTabShape(QTabWidget::Triangular);*/


	if ( !LoadLayout(tr("./EditorRes/Layout/Saved.layout")) )
	{
		LoadLayout(tr("./EditorRes/Layout/Default.layout"));
	}


}

EFFEditorMainWindow::~EFFEditorMainWindow()
{

}

bool EFFEditorMainWindow::LoadLayout(const QString & filePath)
{
	QFile layoutFile(filePath);
    if ( !layoutFile.open(QIODevice::ReadOnly) )
    {
		layoutFile.close();
		return false;
	}

	QByteArray bytes = layoutFile.readAll();
	restoreState(bytes);
	return true;
}

void EFFEditorMainWindow::closeEvent(QCloseEvent *event)
{

    QByteArray state = saveState();

	QFile layoutFile(tr("./EditorRes/Layout/Saved.layout"));
    if( layoutFile.open(QIODevice::WriteOnly) )
    {
		layoutFile.write(state);
		layoutFile.close();
    }

	CRITICAL_SECTION criticalSection;
	InitializeCriticalSection(&criticalSection);
	EnterCriticalSection(&criticalSection);

	g_bExitRenderThread = effTRUE;

	LeaveCriticalSection(&criticalSection);
	DeleteCriticalSection(&criticalSection);

	WaitForSingleObject(g_hRenderThread, INFINITE);

    QMainWindow::closeEvent(event);
}


void EFFEditorMainWindow::fileMenuPressed(QAction * action)
{
	if ( action->text() == tr("Save") )
	{
		EFFObjectManager * objectManager = EFFGetObjectManager(EFF3DObject::GetThisClass());
		std::map<effUINT, EFFObject *> & objects = objectManager->GetObjects();
		std::map<effUINT, EFFObject *>::iterator it = objects.begin();
		for ( ; it != objects.end(); it++ )
		{
			it->second->SaveToFile(_effT("1.yaml"), effFALSE);
		}
	}
}

