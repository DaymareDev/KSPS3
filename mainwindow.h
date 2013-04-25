#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTextBrowser;

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

private:
    Ui::MainWindow *ui;
    QTextBrowser* m_diagnosticsWindow;
};

#endif // MAINWINDOW_H
