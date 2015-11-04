#include <QtCore/QDir>
#include <QtGui/QLayout>
#include <QtGui/QComboBox>
#include <QtGui/QTreeWidget>
#include <QtGui/QLayout>
#include <QtGui/QFileDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QToolButton>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>

#include "findfiledialog.h"
#include "assistant.h"
#include "textedit.h"

FindFileDialog::FindFileDialog(TextEdit *editor)
	: QDialog(editor)
{
	currentEditor = editor;

	createButtons();
	createComboBoxes();
	createFilesTree();
	createLabels();
	createLayout();

	directoryComboBox->addItem(QDir::toNativeSeparators(QDir::currentPath()));
	fileNameComboBox->addItem("*");
	findFiles();

	setWindowTitle(tr("Find File"));
}

void FindFileDialog::browse()
{
	QString currentDirectory = directoryComboBox->currentText();
	QString newDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), currentDirectory);
	if (!newDirectory.isEmpty()) {
		directoryComboBox->addItem(QDir::toNativeSeparators(newDirectory));
		directoryComboBox->setCurrentIndex(directoryComboBox->count() - 1);
		update();
	}
}

void FindFileDialog::openFile()
{
	qDebug("openFile");
	QTreeWidgetItem* item = foundFilesTree->currentItem();
	if (!item)
		return;

	QString fileName = item->text(0);
	QString path = directoryComboBox->currentText() + QDir::separator();

	currentEditor->setContents(path + fileName);
	close();
}

void FindFileDialog::update(const QString&)
{
	findFiles();
	buttonBox->button(QDialogButtonBox::Open)->setEnabled(foundFilesTree->topLevelItemCount() > 0);
}

void FindFileDialog::findFiles()
{
	QRegExp filePattern(fileNameComboBox->currentText() + "*");
	filePattern.setPatternSyntax(QRegExp::Wildcard);

	QDir directory(directoryComboBox->currentText());

	QStringList allFiles = directory.entryList(QDir::Files | QDir::NoSymLinks);
	QStringList matchingFiles;

	foreach (QString file, allFiles) {
		if (filePattern.exactMatch(file))
			matchingFiles << file;
	}
	showFiles(matchingFiles);
}

void FindFileDialog::showFiles(const QStringList &files)
{
	foundFilesTree->clear();

	for (int i = 0; i < files.count(); ++i) {
		QTreeWidgetItem *item = new QTreeWidgetItem(foundFilesTree);
		item->setText(0, files[i]);
	}

	if (files.count() > 0)
		foundFilesTree->setCurrentItem(foundFilesTree->topLevelItem(0));
}

void FindFileDialog::createButtons()
{
	browseButton = new QToolButton;
	browseButton->setText(tr("..."));
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Open
								| QDialogButtonBox::Cancel);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(openFile()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void FindFileDialog::createComboBoxes()
{
	directoryComboBox = new QComboBox;
	fileNameComboBox = new QComboBox;

	fileNameComboBox->setEditable(true);
	fileNameComboBox->setSizePolicy(QSizePolicy::Expanding,
								QSizePolicy::Preferred);

	directoryComboBox->setMinimumContentsLength(30);
	directoryComboBox->setSizeAdjustPolicy(
			QComboBox::AdjustToMinimumContentsLength);
	directoryComboBox->setSizePolicy(QSizePolicy::Expanding,
								QSizePolicy::Preferred);

	connect(fileNameComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(update(const QString&)));
	connect(directoryComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(update(const QString&)));
}

void FindFileDialog::createFilesTree()
{
	foundFilesTree = new QTreeWidget;
	foundFilesTree->setColumnCount(1);
	foundFilesTree->setHeaderLabels(QStringList(tr("Matching Files")));
	foundFilesTree->setRootIsDecorated(false);
	foundFilesTree->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(foundFilesTree, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(openFile()));
}

void FindFileDialog::createLabels()
{
	directoryLabel = new QLabel(tr("Search in:"));
	fileNameLabel = new QLabel(tr("File name (including wildcards):"));
}

void FindFileDialog::createLayout()
{
	QHBoxLayout *fileLayout = new QHBoxLayout;
	fileLayout->addWidget(fileNameLabel);
	fileLayout->addWidget(fileNameComboBox);

	QHBoxLayout *directoryLayout = new QHBoxLayout;
	directoryLayout->addWidget(directoryLabel);
	directoryLayout->addWidget(directoryComboBox);
	directoryLayout->addWidget(browseButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(fileLayout);
	mainLayout->addLayout(directoryLayout);
	mainLayout->addWidget(foundFilesTree);
	mainLayout->addStretch();
	mainLayout->addWidget(buttonBox);
	setLayout(mainLayout);
}
