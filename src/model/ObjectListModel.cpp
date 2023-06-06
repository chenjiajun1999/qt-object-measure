#include "ObjectListModel.h"

#include <QMetaProperty>
#include <QDebug>
#include <QQmlEngine>
#include <QEvent>

ObjectListModel::ObjectListModel(bool owning, QObject* parent, QByteArray alias_role_name)
    : QAbstractListModel(parent)
    , mOwning(owning)
    , mRoleNames{([&]()
    {
        QHash<int, QByteArray> roles = {{OBJECT_ROLE, "object"}};
        if (!alias_role_name.isEmpty())
        {
            roles.insert(ALIAS_ROLE, alias_role_name);
            qDebug()<<"ROLA NAME : "+alias_role_name;
        }
        return roles;
    })()}
{
    connect(this, &QAbstractItemModel::rowsInserted, this, &ObjectListModel::notifyCount);
    connect(this, &QAbstractItemModel::rowsRemoved, this, &ObjectListModel::notifyCount);
    connect(this, &QAbstractItemModel::modelReset, this, &ObjectListModel::notifyCount);
}

ObjectListModel::ObjectListModel(QObjectList objects, bool owning, QObject* parent, QByteArray alias_role_name)
    : ObjectListModel(owning, parent, std::move(alias_role_name))
{
    mObjects = std::move(objects);
    attachToAll();
}

QObjectList &ObjectListModel::getRawData()
{
    return mObjects;
}

bool ObjectListModel::isOwning() const
{
    return mOwning;
}

int ObjectListModel::rowCount(const QModelIndex&) const
{
    return size();
}

QVariant ObjectListModel::data(const QModelIndex& index, int role) const
{
    // accepting DisplayRole, so it can work through proxies using display role as default
    if (!index.isValid() || index.row() >= mObjects.size()
        || (role != OBJECT_ROLE && role != ALIAS_ROLE && role != Qt::DisplayRole))
    {
        return {};
    }

    return QVariant::fromValue(mObjects[index.row()]);
}

QHash<int, QByteArray> ObjectListModel::roleNames() const
{
    return mRoleNames;
}

QObject* ObjectListModel::operator[](int index) const
{
    return mObjects[index];
}

QObject* ObjectListModel::at(int index) const
{
    return mObjects.at(index);
}

QObject* ObjectListModel::first() const
{
    return at(0);
}

QObject* ObjectListModel::last() const
{
    return at(size() - 1);
}

int ObjectListModel::size() const
{
    return mObjects.size();
}

int ObjectListModel::indexOf(QObject* object) const
{
    return mObjects.indexOf(object);
}

void ObjectListModel::append(QObject* object)
{
    insert(size(), object);
}

void ObjectListModel::insert(int index, QObject* object)
{
    if (mObjects.contains(object))
    {
        qWarning() << "Model already contains object" << object;

        if (mOwning)
        {
            // Deleting would fail..
            return;
        }
    }

    attachTo(object);

    beginInsertRows({}, index, index);
    mObjects.insert(index, object);
    endInsertRows();

    emit childrenChanged();
}

void ObjectListModel::reset(QObjectList objects)
{
    beginResetModel();

    for (auto object: mObjects)
    {
        detachFrom(object);
    }

    mObjects.swap(objects);
    attachToAll();

    endResetModel();

    emit childrenChanged();

    if (mOwning) ///< Delete objects after endResetModel(), so that any dependencies are reset first.
    {
        qDeleteAll(objects);
    }
}

QObject* ObjectListModel::takeAt(int index)
{
    if (index < 0 || index >= size())
    {
        return nullptr;
    }

    beginRemoveRows({}, index, index);

    auto object = mObjects.takeAt(index);
    detachFrom(object);

    endRemoveRows();

    emit childrenChanged();

    return object;
}

QObject* ObjectListModel::take(QObject* object)
{
    return takeAt(indexOf(object));
}

void ObjectListModel::removeAt(int index)
{
    if (auto taken_object = takeAt(index); mOwning)
    {
        delete taken_object;
    }
}

void ObjectListModel::remove(QObject* object)
{
    if (mObjects.contains(object))
    {
        removeAt(indexOf(object));
    }
    else
    {
        qWarning() << "Trying to remove non-contained object (" << object << ")";
    }
}

void ObjectListModel::clearRange(int start_index, int count)
{
    if (start_index < 0 || count < 1 || start_index >= size())
    {
        return;
    }

    count = std::min(count, size() - start_index);

    for (int index = start_index + count - 1; index >= start_index; --index)
    {
        removeAt(index);
    }
}

void ObjectListModel::notifyCount() const
{
    emit countChanged(rowCount());
}

void ObjectListModel::propertyChanged()
{
    notifyPropertyChanged(sender());
}

void ObjectListModel::notifyPropertyChanged(QObject* object)
{
    if (int row = indexOf(object); row >= 0)
    {
        emit dataChanged(index(row), index(row), {PROPERTY_MODIFIED_ROLE});
    }
    else
    {
        qWarning() << "Property change from unknown sender" << object;
    }
}

void ObjectListModel::attachToAll()
{
    for (auto object : mObjects)
    {
        attachTo(object);
    }
}

void ObjectListModel::attachTo(QObject* object)
{
    object->installEventFilter(this);
    auto meta = object->metaObject();
    auto slot_changed = metaObject()->method(metaObject()->indexOfSlot(QString("propertyChanged()").toUtf8().constData()));
    for (int i = 0; i < meta->propertyCount(); ++i)
    {
        if (auto property_changed = meta->property(i).notifySignal(); property_changed.isValid())
        {
            connect(object, property_changed, this, slot_changed, Qt::UniqueConnection);
        }
    }

    if (mOwning)
    {
        object->setParent(this);
    }
    QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
}

void ObjectListModel::detachFrom(QObject* object)
{
    object->removeEventFilter(this);
    object->disconnect(this);

    if (mOwning)
    {
        object->setParent(nullptr);
    }
}

bool ObjectListModel::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::DynamicPropertyChange)
    {
        notifyPropertyChanged(watched);
    }
    return false;
}

QQmlListProperty<QObject> ObjectListModel::children()
{
    return QQmlListProperty<QObject>(
        this, this,
        &ObjectListModel::qmlAppendChild,
        &ObjectListModel::qmlChildCount,
        &ObjectListModel::qmlChildAt,
        &ObjectListModel::qmlClearChildren
    );
}

void ObjectListModel::qmlAppendChild(QQmlListProperty<QObject>* list, QObject* child)
{
    reinterpret_cast<ObjectListModel*>(list->data)->append(child);
}

int ObjectListModel::qmlChildCount(QQmlListProperty<QObject>* list)
{
    return reinterpret_cast<ObjectListModel*>(list->data)->size();
}

QObject* ObjectListModel::qmlChildAt(QQmlListProperty<QObject>* list, int index)
{
    return reinterpret_cast<ObjectListModel*>(list->data)->at(index);
}

void ObjectListModel::qmlClearChildren(QQmlListProperty<QObject>* list)
{
    reinterpret_cast<ObjectListModel*>(list->data)->reset();
}
