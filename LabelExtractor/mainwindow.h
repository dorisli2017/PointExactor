#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStandardItemModel>
#include <QMainWindow>

#include "labelmodel.h"

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
    void on_actionLoad_OSM_file_triggered();
    void on_pushButton_clicked();

private:
    void readJson(QStandardItem* item, QJsonArray& jsonArray);

    Ui::MainWindow *ui;

    QStandardItemModel* model;
};

#endif // MAINWINDOW_H
