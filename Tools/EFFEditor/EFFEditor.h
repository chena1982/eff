#pragma once

class EFFEditor
{
public:
	EFFEditor();
	virtual ~EFFEditor();
	virtual void CreateScene();
	virtual void DestroyScene();


	void ReceiveMsg();
private:
	virtual void SetupResources();

private:
};


EFFEDITOR_API VOID InitEditor();
EFFEDITOR_API VOID DestroyEidtor();