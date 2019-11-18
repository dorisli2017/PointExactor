#ifndef LABELMODEL_H
#define LABELMODEL_H

#include <QAbstractItemModel>
#include <iostream>
#include <osmium/osm/node.hpp>
#include <osmium/osm/types.hpp>

#include "poireader.h"

class LabelItem
{
public:
    explicit LabelItem(POI* poi, LabelItem *parent = nullptr)
    {
        m_parentItem = parent;
        m_itemData.append(QString::fromStdString(poi->getLabel()));
        for(auto& c : poi->getClasses())
        {
            QString str = QString("%1:%2").arg(QString::fromStdString(c.first)).arg(QString::fromStdString(c.second));
            std::cout << str.toStdString() << std::endl;
            m_itemData.append(str);
        }
    }

    explicit LabelItem(const QString& str, LabelItem *parent = nullptr)
    {
        m_parentItem = parent;
        m_itemData.append(str);
    }


    ~LabelItem() {qDeleteAll(m_childItems);}


    void appendChild(LabelItem *child) {m_childItems.append(child);}

    LabelItem *child(int row) {return m_childItems.value(row);}
    int childCount() const {return m_childItems.count();}
    int columnCount() const {return m_itemData.count();}
    QVariant data(int column) const {return m_itemData.value(column);}
    int row() const
    {
        if (m_parentItem)
            return m_parentItem->m_childItems.indexOf(const_cast<LabelItem*>(this));

        return 0;
    }
    LabelItem *parentItem() {return m_parentItem;}

private:
    QList<LabelItem*> m_childItems;
    QList<QVariant> m_itemData;
    LabelItem *m_parentItem;
};

class LabelModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    LabelModel(std::vector<POI>& pois, QObject *parent = nullptr);
    ~LabelModel() {delete root;}

    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    LabelItem* root;
};

#endif // LABELMODEL_H
