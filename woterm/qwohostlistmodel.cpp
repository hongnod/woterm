#include "qwohostlistmodel.h"
#include "qwosshconf.h"

#include<QPair>
#include<QVector>

QWoHostListModel::QWoHostListModel(QObject *parent)
    : QAbstractListModel (parent)
{
    refreshList();
}

QWoHostListModel::~QWoHostListModel()
{

}

void QWoHostListModel::refreshList()
{
    if(QWoSshConf::instance()->refresh()){
        m_hosts = QWoSshConf::instance()->hostList();
        resetInternalData();
    }
}

int QWoHostListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()){
        return 0;
    }    
    return m_hosts.count();
}

QModelIndex QWoHostListModel::sibling(int row, int column, const QModelIndex &idx) const
{
    if (!idx.isValid() || column != 0 || row >= m_hosts.count() || row < 0)
        return QModelIndex();

    return createIndex(row, 0);
}

QVariant QWoHostListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_hosts.size()) {
        return QVariant();
    }
    if (!index.isValid()){
        return QVariant();
    }
    const HostInfo& hi = m_hosts.at(index.row());

    if(role == Qt::ToolTipRole) {
        QString tip = hi.name;
        if(hi.name != hi.host && !hi.host.isEmpty()) {
            tip.append("-").append(hi.host);
        }
        if(!hi.memo.isEmpty()){
            tip.append("-").append(hi.memo);
        }
        return tip;
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return QVariant(hi.name);
    }
    if(role == ITEM_INDEX) {
        return index.row();
    }
    return QVariant();
}

bool QWoHostListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

Qt::ItemFlags QWoHostListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QAbstractListModel::flags(index) | Qt::ItemIsDropEnabled;
    }
    return QAbstractListModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

bool QWoHostListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count < 1 || row < 0 || row > rowCount(parent)){
        return false;
    }

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r){
        m_hosts.insert(row, HostInfo());
    }

    endInsertRows();

    return QAbstractListModel::insertRows(row, count, parent);
}

bool QWoHostListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent)){
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    const auto it = m_hosts.begin() + row;
    m_hosts.erase(it, it + count);

    endRemoveRows();

    return true;
}

static bool ascendingLessThan(const QPair<QString, int> &s1, const QPair<QString, int> &s2)
{
    return s1.first < s2.first;
}

static bool decendingLessThan(const QPair<QString, int> &s1, const QPair<QString, int> &s2)
{
    return s1.first > s2.first;
}

void QWoHostListModel::sort(int column, Qt::SortOrder order)
{
#if 1
    return QAbstractListModel::sort(column, order);
#else
    emit layoutAboutToBeChanged(QList<QPersistentModelIndex>(), VerticalSortHint);

     QVector<QPair<QString, HostInfo> > list;
     const int lstCount = m_hosts.count();
     list.reserve(lstCount);
     for (int i = 0; i < lstCount; ++i){
         list.append(QPair<QString, HostInfo>(m_hosts.at(i), i));
     }

     if (order == Qt::AscendingOrder)
         std::sort(list.begin(), list.end(), ascendingLessThan);
     else
         std::sort(list.begin(), list.end(), decendingLessThan);

     m_hosts.clear();
     QVector<int> forwarding(lstCount);
     for (int i = 0; i < lstCount; ++i) {
         m_hosts.append(list.at(i).first);
         forwarding[list.at(i).second] = i;
     }

     QModelIndexList oldList = persistentIndexList();
     QModelIndexList newList;
     const int numOldIndexes = oldList.count();
     newList.reserve(numOldIndexes);
     for (int i = 0; i < numOldIndexes; ++i)
         newList.append(index(forwarding.at(oldList.at(i).row()), 0));
     changePersistentIndexList(oldList, newList);

     emit layoutChanged(QList<QPersistentModelIndex>(), VerticalSortHint);
#endif
}

Qt::DropActions QWoHostListModel::supportedDropActions() const
{
    return QAbstractListModel::supportedDropActions();
}
