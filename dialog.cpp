#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

	int nWidth = 720;
	int nHeight = 1280;

	if (parent != NULL)
		setGeometry(parent->x() + parent->width()/2 - nWidth/2,
			parent->y() + parent->height()/2 - nHeight/2,
			nWidth, nHeight);
	else
		resize(nWidth, nHeight);

	QString sPath = "/mnt/sdcard";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirmodel->setRootPath(sPath);
    ui->treeView->setModel(dirmodel);
	//ui->treeView->setRootIndex(dirmodel->setRootPath("/"));
	ui->treeView->hideColumn(1);
	ui->treeView->hideColumn(2);
	ui->treeView->hideColumn(3);
    filemodel = new QFileSystemModel(this);


}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_treeView_clicked(QModelIndex index)
{
	filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
	//filemodel->setRootPath(sPath);
	ui->listView->setModel(filemodel);
    QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(filemodel->setRootPath(sPath));
}

void Dialog::on_listView_doubleClicked(const QModelIndex &index)
{
	 path = filemodel->fileInfo(index).absoluteFilePath();
	 qDebug() << path;
	// exec();
	 accept();
}
