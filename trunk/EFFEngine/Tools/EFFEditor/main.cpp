#include "stdafx.h"
#include "effeditor.h"
#include <QtGui/QApplication>

QMainWindow * g_pMainWindow = NULL;

//-stylesheet ../EditorRes/Qss/1.qss

void setStyleSheet(QApplication & a, const QString &sheetName)
{
	QFile file("./EditorRes/Skin/Dark/" + sheetName.toLower() + ".qss");

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString styleSheet = QLatin1String(file.readAll());


	a.setStyleSheet(styleSheet);

}

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	setStyleSheet(a, "Dark");
	EFFEditorMainWindow w;
	g_pMainWindow = &w;
	w.showMaximized();
	return a.exec();
}
