#include "qwohostlistmodel.h"
#include "qwosshconf.h"

#include<QPair>
#include<QVector>

QWoHostListModel::QWoHostListModel(QObject *parent)
    : QAbstractListModel (parent)
    , m_maxColumn(1)
{
    refreshList();
}

QWoHostListModel::~QWoHostListModel()
{

}

void QWoHostListModel::setMaxColumnCount(int cnt)
{
    m_maxColumn = cnt;
}

void QWoHostListModel::refreshList()
{
    if(QWoSshConf::instance()->refresh()){
        emit beginResetModel();
        m_hosts = QWoSshConf::instance()->hostList();
        emit endResetModel();
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

QVariant QWoHostListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Host");
        case 2:
            return tr("Memo");
        }
    }
    return QAbstractListModel::headerData(section, orientation, role);
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

    if(role == ROLE_INDEX) {
        return index.row();
    }

    if(role == ROLE_HOSTINFO) {
        QVariant v;
        v.setValue(hi);
        return v;
    }
    if(role == ROLE_REFILTER) {
        QVariant v;
        v.setValue(QString("%1-%2:%3-%4").arg(hi.name).arg(hi.host).arg(hi.port).arg(hi.memo));
        return v;
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        int col = index.column();
        switch (col) {
        case 0:
            return QVariant(hi.name);
        case 1:
            return QVariant(QString("%1:%2").arg(hi.host).arg(hi.port));
        case 2:
            return QVariant(hi.memo);
        }
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

int QWoHostListModel::columnCount(const QModelIndex &parent) const
{
    return m_maxColumn;
}

Qt::DropActions QWoHostListModel::supportedDropActions() const
{
    return QAbstractListModel::supportedDropActions();
}
