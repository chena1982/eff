/********************************************************************
	created:	6:9:2010   0:15
	file path:	d:\EFF\EFFEngine\Tools\EFFEditorUI\Include\EFFEditorUITransformPanel.h
	author:		ChenA
	
	purpose:	
*********************************************************************/
#ifndef __EFFEditorUITransformPanel_2010_9_6__
#define __EFFEditorUITransformPanel_2010_9_6__

class EFFEditorUITransformPanel : public QWidget
{
public:
	EFFEditorUITransformPanel(QWidget * pParent);
	~EFFEditorUITransformPanel();

public:
	//QSize sizeHint() const { return minimumSizeHint(); }
	//QSize minimumSizeHint()  const { return QSize(260, -1); }

protected:
	void create();
};

#endif