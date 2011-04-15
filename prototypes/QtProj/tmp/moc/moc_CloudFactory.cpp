/****************************************************************************
** Meta object code from reading C++ file 'CloudFactory.hpp'
**
** Created: Fri Apr 15 16:59:12 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/CloudFactory.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CloudFactory.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CloudFactory[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_CloudFactory[] = {
    "CloudFactory\0\0onEnterFrame()\0"
};

const QMetaObject CloudFactory::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_CloudFactory,
      qt_meta_data_CloudFactory, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CloudFactory::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CloudFactory::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CloudFactory::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CloudFactory))
        return static_cast<void*>(const_cast< CloudFactory*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int CloudFactory::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onEnterFrame(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
