#include <QtCore/QLibraryInfo>
#include <QtGui/QApplication>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include "mainwindow.h"
#include "findfiledialog.h"

MainWindow::MainWindow()
{
	textViewer = new TextEdit;
	setCentralWidget(textViewer);

	createActions();
	createMenus();

	setWindowTitle(tr("Simple Text Viewer"));
	resize(750, 400);

}

void MainWindow::closeEvent(QCloseEvent *)
{
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About Simple Text Viewer"), tr("This example demonstrates how to use\n" "simple text viewer for you\n" "own application."));
}

void MainWindow::open()
{
	FindFileDialog dialog(textViewer);
	dialog.exec();
}

void MainWindow::createActions()
{
	openAct = new QAction(tr("&Open..."), this);
	openAct->setShortcut(QKeySequence::Open);
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	clearAct = new QAction(tr("&Clear"), this);
	clearAct->setShortcut(tr("Ctrl+C"));
	connect(clearAct, SIGNAL(triggered()), textViewer, SLOT(clear()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	aboutAct = new QAction(tr("&About"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	aboutCsAct = new QAction(tr("About &CopperSpice"), this);
	connect(aboutCsAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addAction(clearAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutCsAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(helpMenu);
}
