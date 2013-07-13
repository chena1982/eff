/********************************************************************
	created:	2012-10-24   22:50
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\Src\EFFEditorRealTimeWidget.cpp
	author:		ChenA
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "EFFEditorRealTimeWidget.h"



EFFEditorRealTimeWidget::EFFEditorRealTimeWidget(QWidget * pParent) : QDockWidget(pParent)
{
	setAttribute(Qt::WA_PaintOnScreen, true);
}


EFFEditorRealTimeWidget::~EFFEditorRealTimeWidget()
{
}

void EFFEditorRealTimeWidget::Init()
{
	HWND hWnd = (HWND)winId();
	effINT width = this->width();
	effINT height = this->height();
	app.Init(effTRUE, hWnd, width, height);
}

QPaintEngine * EFFEditorRealTimeWidget::paintEngine() const
{
	return NULL;
}

void EFFEditorRealTimeWidget::paintEvent(QPaintEvent * paintEvent)
{
	app.Render();
}