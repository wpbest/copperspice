#ifndef FINDFILEDIALOG_H
#define FINDFILEDIALOG_H

#include <QtGui/QDialog>

class QComboBox;
class QDialogButtonBox;
class QLabel;
class QToolButton;
class QTreeWidget;
class QTreeWidgetItem;

//class Assistant;
class TextEdit;

class FindFileDialog : public QDialog
{
	CS_OBJECT(FindFileDialog)

public:
	FindFileDialog(TextEdit *editor);

private:
	CS_SLOT_1(Private, void browse())
	CS_SLOT_2(browse)
	CS_SLOT_1(Private, void openFile())
	CS_SLOT_2(openFile) 
	CS_SLOT_1(Private, void update(const QString& text = QString()))
	CS_SLOT_2(update)

private:
	void findFiles();
	void showFiles(const QStringList &files);

	void createButtons();
	void createComboBoxes();
	void createFilesTree();
	void createLabels();
	void createLayout();

	TextEdit *currentEditor;
	QTreeWidget *foundFilesTree;

	QComboBox *directoryComboBox;
	QComboBox *fileNameComboBox;

	QLabel *directoryLabel;
	QLabel *fileNameLabel;

	QDialogButtonBox *buttonBox;

	QToolButton *browseButton;
};

#endif
