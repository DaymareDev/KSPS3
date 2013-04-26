#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "saveformater.h"
#include "vesseldata.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QTextBrowser>
#include <vector>
#include <sstream>

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
    if(!path.endsWith(".sfs")  ||  !selectedDir.open(QIODevice::ReadOnly))
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

    std::vector<KSPS3::VesselData*> vessels;
    formater.GetVesselManifests(vessels);

    std::stringstream stringBuilder;
    for(std::size_t i = 0; i < vessels.size(); i++)
    {
        stringBuilder << "vessel " << i << std::endl << vessels[i]->GetName().toStdString()
                      << std::endl << "PID: " << vessels[i]->GetPID().toStdString();
        stringBuilder << std::endl << std::endl << (*vessels[i]->AccessFullText()).toStdString();
    }

    fileEditor->setText(QString(stringBuilder.str().c_str()));

}
