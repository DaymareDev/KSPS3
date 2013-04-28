#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QSharedDataPointer>

class QTextBrowser;

namespace KSPS3
{
    class VesselData;
    class SaveFormater;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:


    void on_BrowseSaveButton_clicked();

    void on_BrowseInjectFromButton_clicked();

    void on_injectPushButton_clicked();

    void on_injectVesselPushButton_clicked();

    void on_exportSavePushButton_clicked();

    void on_removeVesselPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTextBrowser* m_diagnosticsWindow;
    bool m_injectionEnabled;

    void importSave(KSPS3::SaveFormater *toUse, std::vector<QSharedPointer<KSPS3::VesselData>> *vesselsOut = 0);
    bool isSFSFile(const QString& pathToFile);
    void verifyPathField(QString& pathfieldName);
    void readVesselsToViews();
    void setInjectionEnabled(bool state);

    bool tryEnableInjectionButtons();
};

#endif // MAINWINDOW_H
