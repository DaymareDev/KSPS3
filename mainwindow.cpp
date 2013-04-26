#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vesseldata.h"
#include "saveformater.h"

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

bool MainWindow::isSFSFile(const QString& pathToFile)
{
    if(!pathToFile.endsWith(".sfs"))
    {
        QMessageBox::warning(0, QString("File Error"), QString("Unable to open the file, .sfs format required!"));
        return false;
    }
    QFile file(pathToFile);
    if(!file.exists())
    {
        QMessageBox::warning(0, QString("File not found!"), QString("Unable to open the file: ") + pathToFile);
        return false;
    }
    return true;
}


void MainWindow::on_BrowseSaveButton_clicked()
{
    verifyPathField(QString("SavePathTextEdit"));
}

void MainWindow::verifyPathField(QString& pathfieldName)
{
    QTextEdit* pathField = this->findChild<QTextEdit*>(pathfieldName);
    QString path = pathField->toPlainText();
    QFile selectedDir(path);
    path = QFileDialog::getOpenFileName(0, QString("Select the save file to use"), path);
    selectedDir.setFileName(path);
    pathField->setText(path);
    if(!isSFSFile(path))
    {
        QMessageBox::warning(0, QString("File Error"), QString("Unable to open the file, .sfs format required!"));
        return;
    }
    tryEnableInjectionButton();
}

void MainWindow::on_BrowseInjectFromButton_clicked()
{
    verifyPathField(QString("fileToInjectTextEdit"));
}

bool MainWindow::tryEnableInjectionButton()
{
    QTextEdit* savePathTextField = this->findChild<QTextEdit*>("SavePathTextEdit");
    QString pathToSave = savePathTextField->toPlainText();
    QFile saveFile(pathToSave);
    QPushButton* injectButton = this->findChild<QPushButton*>("injectPushButton");
    if(!saveFile.exists() || !pathToSave.endsWith(".sfs"))
    {
        injectButton->setEnabled(false);
        return false;
    }

    QTextEdit* injectPathTextField = this->findChild<QTextEdit*>("fileToInjectTextEdit");
    QString pathToInjectFrom = injectPathTextField->toPlainText();
    QFile injectFile(pathToInjectFrom);

    if(!injectFile.exists() || !pathToInjectFrom.endsWith(".sfs"))
    {
        injectButton->setEnabled(false);
        return false;
    }
    injectButton->setEnabled(true);
    return true;
}


void MainWindow::on_injectPushButton_clicked()
{
    findChild<QTextEdit*>("SavePathTextEdit")->setEnabled(false);
    findChild<QTextEdit*>("fileToInjectTextEdit")->setEnabled(false);

    QString injectFromPath = findChild<QTextEdit*>("fileToInjectTextEdit")->toPlainText();
    QString coreSavePath = findChild<QTextEdit*>("SavePathTextEdit")->toPlainText();
    if(!isSFSFile(coreSavePath) || !isSFSFile(injectFromPath))
        QMessageBox::warning(0, QString("File error"), QString("One or more of your paths have become invalid, please re-check them!"));

    KSPS3::SaveFormater coreSave(coreSavePath);
    importSave(&coreSave);

    KSPS3::SaveFormater injectionFile(injectFromPath);
    std::vector<KSPS3::VesselData*> vesselsToInject;
    importSave(&injectionFile, &vesselsToInject);

    std::stringstream stringBuilder;
    for(std::size_t i = 0; i < vesselsToInject.size(); i++)
    {
        stringBuilder << "vessel " << i << std::endl << vesselsToInject[i]->GetName().toStdString()
                      << std::endl << "PID: " << vesselsToInject[i]->GetPID().toStdString();
    }

    findChild<QTextEdit*>("saveFileTextEdit")->setText(QString(stringBuilder.str().c_str()));
}

void MainWindow::importSave(KSPS3::SaveFormater *toUse, std::vector<KSPS3::VesselData*> *vesselsOut)
{
    toUse->CreateKSPS3File();
    while(toUse->HasMessage())
    {
        m_diagnosticsWindow->append(toUse->GetMessage());
    }
    if(vesselsOut)
        toUse->GetVesselManifests(*vesselsOut);
    m_diagnosticsWindow->append(QString("Done importing vessels from") + toUse->GetPath() + QString("\n"));
}
