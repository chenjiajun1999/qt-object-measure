#pragma once

#include <QPointer>
#include <QQuickWindow>

#define OPENVC_DATASET_CATALOG_STRUCUTRE_NEGATIVE_SAMPLES "negative"
#define OPENVC_DATASET_CATALOG_STRUCUTRE_POSITIVE_SAMPLES "positive"

#define OPEN_CV_DATASETS_CARS "https://cogcomp.seas.upenn.edu/Data/Car/"

/*
 * Type of neural network frameworks
 */
typedef enum NEUR_NET_FRAMEWORK {
  OpenCV,
} NEUR_NET_FRAMEWORK;

/**
 *  Instead of remove_pointer
 */
template <typename T> struct remove_ptr { typedef T type; };
template <typename T> struct remove_ptr<T *> {
  typedef typename remove_ptr<T>::type type;
};

template <typename T,
          std::enable_if_t<!std::is_floating_point_v<T>> * = nullptr>
bool equal(const T &lhs, const T &rhs) {
  return lhs == rhs;
}

template <typename T, std::enable_if_t<std::is_floating_point_v<T>> * = nullptr>
bool equal(const T &lhs, const T &rhs) {
  return qFuzzyCompare(lhs + T(1), rhs + T(1));
}

template <typename T> bool updateIfChanged(T &lhs, const T &rhs) {
  if (!equal(lhs, rhs)) {
    lhs = rhs;
    return true;
  }

  return false;
}

template <typename T> bool updateIfChanged(QPointer<T> &lhs, T *rhs) {
  if (lhs != rhs) {
    lhs = rhs;
    return true;
  }

  return false;
}

class QMLObjectGetter {
  inline static QQuickWindow *rootWindow;

public:
  static const QMLObjectGetter &
  getInstance(QQuickWindow *aRootWindow = nullptr) {
    static QMLObjectGetter instance;

    if (rootWindow == nullptr)
      rootWindow = aRootWindow;

    return instance;
  }

  static QObject *getQmlObject(QString objectName) {
    QObject *object = rootWindow->findChild<QObject *>(objectName);
    if (!object) {
      qDebug()<<"Couldn't find "<<objectName<<" object";
      assert(0);
    }
    return object;
  }

  template <typename T>
  static void setPropertyOfQmlObject(QString objectName, const char *property,
                                     T value) {
    getQmlObject(objectName)->setProperty(property, value);
  }
};
