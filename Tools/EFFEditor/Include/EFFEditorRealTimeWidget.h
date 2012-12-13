/********************************************************************
	created:	2012-10-24   22:47
	file path:	d:\EFF\EFFEngine\Tools\EFFEditor\Include\EFFEditorRealTimeWidget.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFEditorRealTimeWidget_2012_10_24__
#define __EFFEditorRealTimeWidget_2012_10_24__



class EFFEditorRealTimeWidget : public QDockWidget
{
	Q_OBJECT
public:
	EFFEditorRealTimeWidget(QWidget * pParent);
	virtual ~EFFEditorRealTimeWidget();
public:
	void Init();
	QPaintEngine * paintEngine() const;
protected:

	void paintEvent(QPaintEvent * paintEvent);

protected:
	MMOApplication app;
};

#endif