#ifndef COLORFULMODEL_H
#define COLORFULMODEL_H

#include <QStandardItemModel>
#include <QStringListModel>

class colorfulModel : public QStringListModel
{
public:
    explicit colorfulModel(QObject *parent = nullptr);
    explicit colorfulModel(const QStringList &strings, QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
};

#endif // COLORFULMODEL_H
