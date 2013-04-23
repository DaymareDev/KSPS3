#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_BrowseSaveButton_clicked()
{
    QTextEdit* pathField = this->findChild<QTextEdit*>("SavePathTextEdit");
    QString path = pathField->toPlainText();
    QFile selectedDir(path);
    path = QFileDialog::getOpenFileName(0, QString("Select the save file to use"), path);
    selectedDir.setFileName(path);
    if(selectedDir.exists())
    {
        pathField->setText(path);
        if(path.endsWith(".sfs") && selectedDir.open(QIODevice::ReadOnly))
        {
            QTextEdit* fileEditor = this->findChild<QTextEdit*>("saveFileTextEdit");
            fileEditor->setText(QString(selectedDir.readAll()));

        }
    }
}
