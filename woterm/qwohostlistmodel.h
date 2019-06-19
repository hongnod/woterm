#pragma once

#include "qwoglobal.h"

#include <QAbstractListModel>

#define ROLE_INDEX   (Qt::UserRole+1)
#define ROLE_HOSTINFO (Qt::UserRole+2)

class QWoHostListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QWoHostListModel(QObject *parent = nullptr);
    virtual ~QWoHostListModel() override;

    void refreshList();
    bool find(const QUuid& uid, HostInfo *phi) const;

private:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    Qt::DropActions supportedDropActions() const override;

private:
    Q_DISABLE_COPY(QWoHostListModel)
    QList<HostInfo> m_hosts;
};