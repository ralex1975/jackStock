/****************************************************************************
** Meta object code from reading C++ file 'externDataTab.h'
**
** Created: Sun Feb 3 14:44:25 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "externDataTab.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'externDataTab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CExternDataTab[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      61,   49,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CExternDataTab[] = {
    "CExternDataTab\0\0on_getExternDataButton_clicked()\0"
    "text,number\0setTextAke(QString,int)\0"
};

void CExternDataTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CExternDataTab *_t = static_cast<CExternDataTab *>(_o);
        switch (_id) {
        case 0: _t->on_getExternDataButton_clicked(); break;
        case 1: _t->setTextAke((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CExternDataTab::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CExternDataTab::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CExternDataTab,
      qt_meta_data_CExternDataTab, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CExternDataTab::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CExternDataTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CExternDataTab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CExternDataTab))
        return static_cast<void*>(const_cast< CExternDataTab*>(this));
    return QDialog::qt_metacast(_clname);
}

int CExternDataTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
