#include "labelmodel.h"

#include <unordered_map>

#include "poireader.h"

LabelModel::LabelModel(std::vector<POI> &pois, QObject *parent)
{
    root = new LabelItem("$name");

    std::unordered_map<std::string, std::vector<POI*>> categories;
    for(auto& p : pois)
    {
        for(auto c : POI::CLASS_KEYS)
            if(p.getClass(c) != "")
                categories[c].push_back(&p);
    }

    for(auto& cc : categories)
    {
        auto categoryItem = new LabelItem(QString::fromStdString(cc.first), root);
        root->appendChild(categoryItem);
        for(auto p : cc.second)
        {
            auto poiItem = new LabelItem(p, categoryItem);
            categoryItem->appendChild(poiItem);
        }
    }
}

QModelIndex LabelModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    LabelItem *parentItem;

    if (!parent.isValid())
        parentItem = root;
    else
        parentItem = static_cast<LabelItem*>(parent.internalPointer());

    LabelItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex LabelModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    LabelItem *childItem = static_cast<LabelItem*>(index.internalPointer());
    LabelItem *parentItem = childItem->parentItem();

    if (parentItem == root)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int LabelModel::rowCount(const QModelIndex &parent) const
{
    LabelItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = root;
    else
        parentItem = static_cast<LabelItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int LabelModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<LabelItem*>(parent.internalPointer())->columnCount();
    else
        return root->columnCount();
}

QVariant LabelModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    LabelItem *item = static_cast<LabelItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags LabelModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant LabelModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return root->data(section);

    return QVariant();
}
