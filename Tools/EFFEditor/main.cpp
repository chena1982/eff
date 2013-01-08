#include "stdafx.h"
#include "effeditor.h"
#include "EFFEditorHierarchyPanel.h"
#include <QtGui/QApplication>

EFFEditorMainWindow * g_pMainWindow = NULL;

//-stylesheet ../EditorRes/Qss/1.qss

void setStyleSheet(QApplication & a, const QString &sheetName)
{
	QFile file("./EditorRes/Skin/Dark/" + sheetName.toLower() + ".qss");

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString styleSheet = QLatin1String(file.readAll());


	a.setStyleSheet(styleSheet);

}

class A;

class B
{
	RTTI_DECLARE_BASE(B)
	int t;

	void * test(int * z, A & d, A * s)
	{
		return NULL;
	}

	void test1()
	{
	}
};

class A : public EFFComponent
{
	RTTI_DECLARE(A, EFFComponent)
public:
	effINT m_z;
	effString m_s;
	B m_b;
	std::vector<effString> m_v;
	std::vector<B *> m_t;
	std::vector<B>	m_r;
	std::vector<effINT>	m_u;

	effBOOL m_bool;

	std::vector<effString *> m_n;
};

RTTI_IMPLEMENT(A, 0)
RTTI_IMPLEMENT_BASE(B, 0)

void test()
{

	//EFFObjectManager * objectManager = new EFFObjectManager();
	//EFF3DObject * object = (EFF3DObject *)objectManager->CreateObject(EFF3DObject::GetThisClass());

	//EFFRegisterObjectManager(object->GetRuntimeClass(), objectManager);

	//A * a = (A *)EFFCreateObject(_effT("A"));
	/*A * a = (A *)object->AddComponent(A::GetThisClass()->GetID());
	a->m_z = 10;
	a->m_s = _effT("sssssssss");
	a->m_v.push_back(_effT("ºÍÐ³"));
	EFFClass * AClass = a->GetThisClass();
	//AClass->AddProperty(&A::m_z, _effT("height"));

	//AClass->AddProperty(&A::m_u, _effT("sss"));
	a->m_u.push_back(1);
	AClass->AddProperty(&A::m_s, _effT("Name"));
	//AClass->AddProperty(&A::m_b, _effT("Bzz"));
	AClass->AddProperty(&A::m_v, _effT("Vector"));
	//AClass->AddProperty(&A::m_n, _effT("string pointer Vector"));

	//AClass->AddProperty(&A::m_t, _effT("BVector"));

	AClass->AddProperty(&A::m_bool, _effT("Bool"));

	object->SetName(_effT("test"));
	g_pMainWindow->GetHierarchyPanel()->AddObject(object);*/
}


int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	setStyleSheet(a, "Dark");


	EFFEditorMainWindow w;
	g_pMainWindow = &w;

	test();

	w.showMaximized();
	return a.exec();
}
