#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include "textedit.h"

TextEdit::TextEdit(QWidget *parent)
	: QTextEdit(parent)
{
	setReadOnly(true);
}

void TextEdit::setContents(const QString &fileName)
{
	QFileInfo fi(fileName);
	srcUrl = QUrl::fromLocalFile(fi.absoluteFilePath());
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly)) {
		QString data(file.readAll());
		if (fileName.endsWith(".html"))
			setHtml(data);
		else
			setPlainText(data);
	}
}

QVariant TextEdit::loadResource(int type, const QUrl &name)
{
	if (type == QTextDocument::ImageResource) {
		QFile file(srcUrl.resolved(name).toLocalFile());
		if (file.open(QIODevice::ReadOnly))
			return file.readAll();
	}
	return QTextEdit::loadResource(type, name);
}
