#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "poireader.h"

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <iostream>
#include <unordered_map>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_OSM_file_triggered()
{
    auto osmFile = QFileDialog::getOpenFileName(this, "Choose an OSM File", "~", "*.osm");
    if(osmFile != "")
    {
        POIReader reader(osmFile.toStdString());
        reader.run();
        auto& pois = reader.getPois();

        int columnCount = 0;
        for(auto& p : pois)
            if(columnCount < p.getClasses().size())
                columnCount = p.getClasses().size();

        if(model != nullptr)
            delete model;

        model = new QStandardItemModel(this);
        QStandardItem* root = model->invisibleRootItem();
        root->setColumnCount(columnCount);

        std::unordered_map<std::string, std::unordered_map<std::string, std::vector<POI*>>> categories;
        for(auto& p : pois)
        {
            for(auto c : POI::CLASS_KEYS)
            {
                auto sCategory = p.getClass(c);
                if(sCategory != "")
                    categories[c][sCategory].push_back(&p);
            }
        }

        for(auto& c : categories)
        {
            auto categoryItem = new QStandardItem(QString::fromStdString(c.first));
            root->appendRow(QList<QStandardItem*>() << categoryItem);

            for(auto sc : c.second)
            {
                auto scategoryItem = new QStandardItem(QString::fromStdString(sc.first));
                auto nsCategoryItem = new QStandardItem(QString("%1").arg(sc.second.size()));
                categoryItem->appendRow(QList<QStandardItem*>() << scategoryItem << nsCategoryItem);

                for(auto p : sc.second)
                {
                    auto label = QString::fromStdString(p->getLabel());
                    auto mainCategory = QString::fromStdString(c.first);
                    auto secondaryCategory = QString::fromStdString(sc.first);

                    QJsonObject poiJson;
                    QJsonValue jsonLabel = QJsonValue(label);
                    QJsonValue jsonPosX = QJsonValue(p->getPos().first);
                    QJsonValue jsonPosY = QJsonValue(p->getPos().second);
                    QJsonValue jsonMC = QJsonValue(mainCategory);
                    QJsonValue jsonSC = QJsonValue(secondaryCategory);

                    poiJson.insert("label", jsonLabel);
                    poiJson.insert("x", jsonPosX);
                    poiJson.insert("y", jsonPosY);
                    poiJson.insert("mc", jsonMC);
                    poiJson.insert("sc", jsonSC);

                    auto poiItem = new QStandardItem(label);
                    poiItem->setData(poiJson, Qt::UserRole);

                    QList<QStandardItem*> columns;
                    columns << poiItem;
                    QString str = QString("%1:%2").arg(mainCategory).arg(secondaryCategory);
                    auto columnItem = new QStandardItem(str);
                    columns << columnItem;

                    while(columns.size() < columnCount)
                        columns << new QStandardItem("");

                    scategoryItem->appendRow(columns);
                }
            }
        }

        ui->treeView->setModel(model);
        ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
}

void MainWindow::on_pushButton_clicked()
{
    auto saveTo = QFileDialog::getSaveFileName(this, "Save to...", "");
    if(saveTo == "")
        return;

    QFile file(saveTo);
    if(!file.open(QIODevice::WriteOnly))
    {
        std::cout << "Error" << std::endl;
        return;
    }

    std::function<void (QStandardItem*, QJsonArray&)> iterateChildren = [&](QStandardItem* item, QJsonArray& jsonArray) {
        if(!item->hasChildren())
            readJson(item, jsonArray);

        for(int i = 0; i < item->rowCount(); ++i)
        {
            auto child = item->child(i);
            if(child->hasChildren())
                iterateChildren(child, jsonArray);
            else
                readJson(child, jsonArray);
        }
    };

    QJsonArray array;
    auto selected = ui->treeView->selectionModel()->selectedIndexes();
    for(auto& index : selected)
    {
        if(index.column() == 0)
        {
            auto item = model->itemFromIndex(index);
            iterateChildren(item, array);
        }
    }

    QJsonDocument doc = QJsonDocument(array);
    file.write(doc.toJson(QJsonDocument::Compact));
    file.close();
}

void MainWindow::readJson(QStandardItem* item, QJsonArray& jsonArray)
{
    auto json = item->data(Qt::UserRole).toJsonObject();
    jsonArray.append(json);
}
