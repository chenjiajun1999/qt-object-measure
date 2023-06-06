#pragma once

#include <QAbstractListModel>
#include <QQmlListProperty>

#include <type_traits>
#include <functional>

class ObjectListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ size NOTIFY countChanged)
    Q_PROPERTY(QQmlListProperty<QObject> children READ children NOTIFY childrenChanged)
    Q_CLASSINFO("DefaultProperty", "children")

public:
    enum
    {
        OBJECT_ROLE = Qt::UserRole,
        ALIAS_ROLE = OBJECT_ROLE + 1,
        PROPERTY_MODIFIED_ROLE = ALIAS_ROLE + 1,
        LAST_ROLE = PROPERTY_MODIFIED_ROLE
    };

    explicit ObjectListModel(bool owning = true, QObject* parent = nullptr, QByteArray alias_role_name = {});
    explicit ObjectListModel(QObjectList objects, bool owning = true, QObject* parent = nullptr, QByteArray alias_role_name = {});

    bool isOwning() const;
    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QObject* operator[](int index) const;

public slots:
    QObject* at(int index) const;
    QObject* first() const;
    QObject* last() const;

    QObjectList& getRawData();

    int size() const;
    int indexOf(QObject* object) const;

    void append(QObject* object);
    void insert(int index, QObject* object);

    void reset(QObjectList objects = {});
    QObject* takeAt(int index);
    QObject* take(QObject* object);
    void removeAt(int index);
    void remove(QObject* object);
    void clearRange(int start_index, int count);

signals:
    void countChanged(int count) const;
    void childrenChanged();

private slots:
    void notifyCount() const;
    void propertyChanged();
    void notifyPropertyChanged(QObject* object);

protected:
    void attachToAll();
    virtual void attachTo(QObject* object);
    virtual void detachFrom(QObject* object);

    bool eventFilter(QObject* watched, QEvent* event) override;

    QObjectList	mObjects;
    bool		mOwning;
    const QHash<int, QByteArray> mRoleNames;

    QQmlListProperty<QObject> children();

    static void qmlAppendChild(QQmlListProperty<QObject>*, QObject*);
    static int qmlChildCount(QQmlListProperty<QObject>*);
    static QObject* qmlChildAt(QQmlListProperty<QObject>*, int);
    static void qmlClearChildren(QQmlListProperty<QObject>*);
};
