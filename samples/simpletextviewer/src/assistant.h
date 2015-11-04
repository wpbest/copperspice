#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <QtCore/QString>
#include <QProcess>

class Assistant
{
public:
	Assistant();
	~Assistant();
	void showDocumentation(const QString &file);

private:
	bool startAssistant();
	QProcess *proc;
};

#endif
