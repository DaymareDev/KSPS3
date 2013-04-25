#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "saveformater.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QTextBrowser>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_diagnosticsWindow = findChild<QTextBrowser*>("processTextBrowser");
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
    if(!selectedDir.exists())
    {
        return;
    }

    pathField->setText(path);
    QTextEdit* fileEditor = this->findChild<QTextEdit*>("saveFileTextEdit");
    if(path.endsWith(".sfs") && selectedDir.open(QIODevice::ReadOnly))
    {

        fileEditor->setText(QString(selectedDir.readAll()));
    }
    else
    {
        QMessageBox::warning(0, QString("File Error"), QString("Unable to open the file, .sfs format required!"));
        return;
    }

    KSPS3::SaveFormater formater(path);
    formater.CreateKSPS3File();
    while(formater.HasMessage())
    {
        m_diagnosticsWindow->append(formater.GetMessage());
    }

}
