/****************************************************************************
** Meta object code from reading C++ file 'graphTableTab.h'
**
** Created: Fri Feb 28 23:39:51 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "graphTableTab.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphTableTab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CGraphTableTab[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      50,   43,   15,   15, 0x0a,
      93,   80,   15,   15, 0x08,
     119,  113,   15,   15, 0x08,
     153,   15,   15,   15, 0x08,
     183,   15,   15,   15, 0x08,
     209,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CGraphTableTab[] = {
    "CGraphTableTab\0\0recvParserThreadIsFinish()\0"
    "number\0recvHtmlPageIsDownloaded(int)\0"
    "logicalIndex\0sectionClicked(int)\0index\0"
    "on_tableView_clicked(QModelIndex)\0"
    "on_getWebDataButton_clicked()\0"
    "on_FilterButton_clicked()\0"
    "on_ShowLatestButton_clicked()\0"
};

void CGraphTableTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CGraphTableTab *_t = static_cast<CGraphTableTab *>(_o);
        switch (_id) {
        case 0: _t->recvParserThreadIsFinish(); break;
        case 1: _t->recvHtmlPageIsDownloaded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->sectionClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_tableView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: _t->on_getWebDataButton_clicked(); break;
        case 5: _t->on_FilterButton_clicked(); break;
        case 6: _t->on_ShowLatestButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CGraphTableTab::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CGraphTableTab::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CGraphTableTab,
      qt_meta_data_CGraphTableTab, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CGraphTableTab::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CGraphTableTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CGraphTableTab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CGraphTableTab))
        return static_cast<void*>(const_cast< CGraphTableTab*>(this));
    return QDialog::qt_metacast(_clname);
}

int CGraphTableTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
