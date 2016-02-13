/****************************************************************************
** Meta object code from reading C++ file 'findfiledialog.h'
**
** Created: Sat Jan 19 15:48:41 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "findfiledialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'findfiledialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FindFileDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      25,   15,   15,   15, 0x08,
      37,   32,   15,   15, 0x08,
      64,   15,   15,   15, 0x28,
      75,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FindFileDialog[] = {
    "FindFileDialog\0\0browse()\0help()\0item\0"
    "openFile(QTreeWidgetItem*)\0openFile()\0"
    "update()\0"
};

void FindFileDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FindFileDialog *_t = static_cast<FindFileDialog *>(_o);
        switch (_id) {
        case 0: _t->browse(); break;
        case 1: _t->help(); break;
        case 2: _t->openFile((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->openFile(); break;
        case 4: _t->update(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FindFileDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FindFileDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FindFileDialog,
      qt_meta_data_FindFileDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FindFileDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FindFileDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FindFileDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FindFileDialog))
        return static_cast<void*>(const_cast< FindFileDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int FindFileDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
