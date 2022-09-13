#include "colorfulmodel.h"

colorfulModel::colorfulModel(QObject *parent)
    : QStringListModel{parent}{}

colorfulModel::colorfulModel(const QStringList &strings, QObject *parent)
    : QStringListModel(strings){}

QVariant colorfulModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();
    if (role == Qt::TextAlignmentRole)
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    else if (role == Qt::DisplayRole)
        return QStringListModel::data(index, role);
    else if(role == Qt::BackgroundRole){
        if(this->data(index, Qt::DisplayRole).toString().contains("√"))
            return QColor(Qt::green);
        else
            return QColor(Qt::white);
    }
    return QVariant();
}

bool colorfulModel::setData(const QModelIndex &index, const QVariant &value, int role){
    return QStringListModel::setData(index, value, role);
}
