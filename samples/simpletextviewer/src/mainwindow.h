#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "textedit.h"

class MainWindow : public QMainWindow
{
	CS_OBJECT(MainWindow)
public:
	MainWindow();

private:
	CS_SLOT_1(Private, void about())
	CS_SLOT_2(about)
	CS_SLOT_1(Private, void open())
	CS_SLOT_2(open)

protected:
	void closeEvent(QCloseEvent *event);

private:
	void createActions();
	void createMenus();

	TextEdit *textViewer;

	QMenu *fileMenu;
	QMenu *helpMenu;

	QAction *clearAct;
	QAction *openAct;
	QAction *exitAct;
	QAction *aboutAct;
	QAction *aboutCsAct;
};

#endif
