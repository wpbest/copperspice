#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QtCore/QUrl>
#include <QtGui/QTextEdit>

class TextEdit : public QTextEdit
{
	CS_OBJECT(TextEdit)
public:
	TextEdit(QWidget *parent = 0);
	void setContents(const QString &fileName);

private:
	QVariant loadResource(int type, const QUrl &name);
	QUrl srcUrl;
};

#endif
