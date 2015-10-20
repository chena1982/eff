#pragma once


class MainPanelControl;

class EFFEditor : public base::BaseManager,
				  public MyGUI::Singleton<EFFEditor>
{  
public:  
    EFFEditor();
	virtual ~EFFEditor();
    virtual void createScene();
	virtual void destroyScene();

	MainPanelControl * GetMainPanelControl() { return mMainPanelControl; }

	virtual void windowResized();
	virtual void windowMove();

	void ReceiveMsg(effFLOAT elapsedTime);
private:
	virtual void setupResources();

private:
	MainPanelControl * mMainPanelControl;
};