#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vesseldata.h"
#include "saveformater.h"
#include "VesselListWidgetItem.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QTextBrowser>
#include <vector>
//#include <QListWidgetItem>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) : m_injectionEnabled(false),
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
    tryEnableInjectionButtons();
}

void MainWindow::readVesselsToViews()
{
    QString injectFromPath = findChild<QTextEdit*>("fileToInjectTextEdit")->toPlainText();
    QString coreSavePath = findChild<QTextEdit*>("SavePathTextEdit")->toPlainText();
    if(!isSFSFile(coreSavePath) || !isSFSFile(injectFromPath))
        QMessageBox::warning(0, QString("File error"), QString("One or more of your paths have become invalid, please re-check them!"));

    KSPS3::SaveFormater coreSave(coreSavePath);
    std::vector<QSharedPointer<KSPS3::VesselData>> savedVessels;
    importSave(&coreSave, &savedVessels);

    QListWidget* savesList = findChild<QListWidget*>("vesselsInSaveListView");
    for(std::size_t i = 0; i < savedVessels.size(); i++)
    {
        VesselListWidgetItem* element = new VesselListWidgetItem(savedVessels[i], savesList);
        element->setText(savedVessels[i]->GetName());
        m_diagnosticsWindow->append(*element->GetVesselData()->AccessFullText());
        savesList->addItem(element);
    }

    KSPS3::SaveFormater injectionFile(injectFromPath);
    std::vector<QSharedPointer<KSPS3::VesselData>> vesselsToInject;
    importSave(&injectionFile, &vesselsToInject);

    QListWidget* injectionList = findChild<QListWidget*>("InjectableVesselsListView");
    for(std::size_t i = 0; i < vesselsToInject.size(); i++)
    {
        VesselListWidgetItem* element = new VesselListWidgetItem(vesselsToInject[i], injectionList);
        element->setText(vesselsToInject[i]->GetName());
        m_diagnosticsWindow->append(*element->GetVesselData()->AccessFullText());
        savesList->addItem(element);
    }

    std::stringstream stringBuilder;
    for(std::size_t i = 0; i < vesselsToInject.size(); i++)
    {
        stringBuilder << "vessel " << i << std::endl << vesselsToInject[i]->GetName().toStdString()
                      << std::endl << "PID: " << vesselsToInject[i]->GetPID().toStdString();
    }

    findChild<QTextEdit*>("saveFileTextEdit")->setText(QString(stringBuilder.str().c_str()));
}

void MainWindow::setInjectionEnabled(bool state)
{
    if(m_injectionEnabled == state)
        return;
    QPushButton* injectAllButton = this->findChild<QPushButton*>("injectAllPushButton");
    QPushButton* injectOneButton = this->findChild<QPushButton*>("injectVesselPushButton");
    QPushButton* removeOneButton = this->findChild<QPushButton*>("removeVesselPushButton");

    injectAllButton->setEnabled(state);
    injectOneButton->setEnabled(state);
    removeOneButton->setEnabled(state);

    findChild<QTextEdit*>("SavePathTextEdit")->setEnabled(!state);
    findChild<QTextEdit*>("fileToInjectTextEdit")->setEnabled(!state);
    m_injectionEnabled = true;

    if(!state)
    {
        return;
    }
    readVesselsToViews();

}

void MainWindow::on_BrowseInjectFromButton_clicked()
{
    verifyPathField(QString("fileToInjectTextEdit"));
}

bool MainWindow::tryEnableInjectionButtons()
{
    QTextEdit* savePathTextField = this->findChild<QTextEdit*>("SavePathTextEdit");
    QString pathToSave = savePathTextField->toPlainText();
    QFile saveFile(pathToSave);

    if(!saveFile.exists() || !pathToSave.endsWith(".sfs"))
    {
        setInjectionEnabled(false);
        return false;
    }

    QTextEdit* injectPathTextField = this->findChild<QTextEdit*>("fileToInjectTextEdit");
    QString pathToInjectFrom = injectPathTextField->toPlainText();
    QFile injectFile(pathToInjectFrom);

    if(!injectFile.exists() || !pathToInjectFrom.endsWith(".sfs"))
    {
        setInjectionEnabled(false);
        return false;
    }

    setInjectionEnabled(true);
    return true;
}


void MainWindow::on_injectPushButton_clicked()
{

}

void MainWindow::importSave(KSPS3::SaveFormater *toUse,std::vector<QSharedPointer<KSPS3::VesselData>> *vesselsOut)
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

void MainWindow::on_injectVesselPushButton_clicked()
{
    QListWidget* injectionList = findChild<QListWidget*>("InjectableVesselsListView");
    QListWidget* savesList = findChild<QListWidget*>("vesselsInSaveListView");

    QList<QListWidgetItem*> selected = injectionList->selectedItems();

    for(QList<QListWidgetItem*>::Iterator it = selected.begin(); it != selected.end(); it++)
    {
        QListWidgetItem* item = (*it);
        injectionList->takeItem(injectionList->row(item));
        savesList->addItem(item);
    }
}

void MainWindow::on_removeVesselPushButton_clicked()
{
    QListWidget* injectionList = findChild<QListWidget*>("InjectableVesselsListView");
    QListWidget* savesList = findChild<QListWidget*>("vesselsInSaveListView");

    QList<QListWidgetItem*> selected = savesList->selectedItems();

    for(QList<QListWidgetItem*>::Iterator it = selected.begin(); it != selected.end(); it++)
    {
        QListWidgetItem* item = (*it);
        savesList->takeItem(savesList->row(item));
        injectionList->addItem(item);
    }
}

void MainWindow::on_exportSavePushButton_clicked()
{
    QListWidget* savesList = findChild<QListWidget*>("vesselsInSaveListView");
    QTextEdit* savePathTextField = this->findChild<QTextEdit*>("SavePathTextEdit");
    QString pathToSave = savePathTextField->toPlainText();
    QFile saveFile(pathToSave);

    if(!saveFile.exists() || !pathToSave.endsWith(".sfs") || !saveFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(0, "Unable to open save file for writing", "When attempting to export new save, file was not opened.");
    }

    QString saveText(saveFile.readAll());
    std::size_t headerEnd = saveText.indexOf("VESSEL");
    QString header = saveText.left(headerEnd);
    saveFile.close();
    saveFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    m_diagnosticsWindow->append(QString("Isolated header:\n") + header + QString("\n--Writing vessels to save--\n"));

    std::stringstream stringBuilder;

    for(int i = 0; i < savesList->count(); i++)
    {
        VesselListWidgetItem* item = (VesselListWidgetItem*)savesList->item(i);
        stringBuilder << item->GetVesselData()->AccessFullText()->toStdString();
    }

    stringBuilder << "}\n}";
    saveFile.write(header.toLocal8Bit());
    saveFile.write(stringBuilder.str().c_str());
    saveFile.close();

}


