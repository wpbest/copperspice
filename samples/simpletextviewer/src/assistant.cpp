#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QLibraryInfo>
#include <QtCore/QProcess>

#include <QtGui/QMessageBox>

#include "assistant.h"

Assistant::Assistant()
	: proc(0)
{
}

Assistant::~Assistant()
{
	if (proc && proc->state() == QProcess::Running) {
		proc->terminate();
		proc->waitForFinished(3000);
	}
	delete proc;
}

void Assistant::showDocumentation(const QString &page)
{
	if (!startAssistant())
		return;

	QByteArray ba("SetSource ");
	ba.append("qthelp://com.trolltech.examples.simpletextviewer/doc/");

	proc->write(ba + page.toLocal8Bit() + '\n');
}

bool Assistant::startAssistant()
{
	if (!proc)
		proc = new QProcess();
/*
	if (proc->state() != QProcess::Running) {
		QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QDir::separator();
#if !defined(Q_OS_MAC)
		app += QLatin1String("assistant");
#else
		app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");
#endif

		QStringList args;
		args << QLatin1String("-collectionFile")
			<< QLibraryInfo::location(QLibraryInfo::ExamplesPath)
			+ QLatin1String("/help/simpletextviewer/documentation/simpletextviewer.qhc")
			<< QLatin1String("-enableRemoteControl");

		proc->start(app, args);

		if (!proc->waitForStarted()) {
			QMessageBox::critical(0, QObject::tr("Simple Text Viewer"),
				QObject::tr("Unable to launch Qt Assistant (%1)").arg(app));
			return false;
		}
	}*/
	return true;
}
