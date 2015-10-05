/****************************************************************************
** Meta object code from reading C++ file 'tatest.h'
**
** Created: Fri Feb 28 16:29:01 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "tatest.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tatest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TaTest[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x0a,
      38,    7,    7,    7, 0x0a,
      73,    7,    7,    7, 0x0a,
     112,    7,    7,    7, 0x0a,
     153,    7,    7,    7, 0x0a,
     181,    7,    7,    7, 0x0a,
     216,    7,    7,    7, 0x08,
     230,    7,    7,    7, 0x08,
     256,    7,    7,    7, 0x08,
     286,    7,    7,    7, 0x08,
     314,    7,    7,    7, 0x08,
     349,  343,    7,    7, 0x08,
     384,    7,    7,    7, 0x08,
     408,    7,    7,    7, 0x08,
     434,    7,    7,    7, 0x08,
     466,    7,    7,    7, 0x08,
     495,    7,    7,    7, 0x08,
     526,    7,    7,    7, 0x08,
     564,  555,    7,    7, 0x08,
     601,    7,    7,    7, 0x08,
     631,    7,    7,    7, 0x08,
     662,    7,    7,    7, 0x08,
     699,    7,    7,    7, 0x08,
     745,    7,    7,    7, 0x08,
     777,    7,    7,    7, 0x08,
     802,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TaTest[] = {
    "TaTest\0\0recvHtmlPageIsDownloaded(int)\0"
    "slotImportYahooTaDataIsFinish(int)\0"
    "slotReceivedAssetTaDataFromServer(int)\0"
    "slotRecvSingleAssetTaDataFromServer(int)\0"
    "slotResendTimerHasExpired()\0"
    "slotThreadReqGetTaDataFromServer()\0"
    "updatescale()\0on_UpdateButton_clicked()\0"
    "on_InsertDataButton_clicked()\0"
    "on_OpenFileButton_clicked()\0"
    "on_AddToTreeButton_clicked()\0index\0"
    "on_treeWidget_clicked(QModelIndex)\0"
    "on_pushButton_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_toggleZoomButton_3_clicked()\0"
    "on_ZoomOutButton_3_clicked()\0"
    "on_PanningOnButton_3_clicked()\0"
    "on_panLeftButton_3_clicked()\0position\0"
    "on_horizontalSlider_sliderMoved(int)\0"
    "on_panRightButton_3_clicked()\0"
    "on_parseDataButton_3_clicked()\0"
    "on_pushButtonImportAllData_clicked()\0"
    "on_pushButtonTestSendRecvServerData_clicked()\0"
    "on_SelStockListButton_clicked()\0"
    "on_pushButtonS_clicked()\0"
    "on_pushButtoGetLatetestDate_clicked()\0"
};

void TaTest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TaTest *_t = static_cast<TaTest *>(_o);
        switch (_id) {
        case 0: _t->recvHtmlPageIsDownloaded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->slotImportYahooTaDataIsFinish((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotReceivedAssetTaDataFromServer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slotRecvSingleAssetTaDataFromServer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slotResendTimerHasExpired(); break;
        case 5: _t->slotThreadReqGetTaDataFromServer(); break;
        case 6: _t->updatescale(); break;
        case 7: _t->on_UpdateButton_clicked(); break;
        case 8: _t->on_InsertDataButton_clicked(); break;
        case 9: _t->on_OpenFileButton_clicked(); break;
        case 10: _t->on_AddToTreeButton_clicked(); break;
        case 11: _t->on_treeWidget_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 12: _t->on_pushButton_clicked(); break;
        case 13: _t->on_pushButton_2_clicked(); break;
        case 14: _t->on_toggleZoomButton_3_clicked(); break;
        case 15: _t->on_ZoomOutButton_3_clicked(); break;
        case 16: _t->on_PanningOnButton_3_clicked(); break;
        case 17: _t->on_panLeftButton_3_clicked(); break;
        case 18: _t->on_horizontalSlider_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->on_panRightButton_3_clicked(); break;
        case 20: _t->on_parseDataButton_3_clicked(); break;
        case 21: _t->on_pushButtonImportAllData_clicked(); break;
        case 22: _t->on_pushButtonTestSendRecvServerData_clicked(); break;
        case 23: _t->on_SelStockListButton_clicked(); break;
        case 24: _t->on_pushButtonS_clicked(); break;
        case 25: _t->on_pushButtoGetLatetestDate_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TaTest::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TaTest::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_TaTest,
      qt_meta_data_TaTest, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TaTest::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TaTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TaTest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TaTest))
        return static_cast<void*>(const_cast< TaTest*>(this));
    return QDialog::qt_metacast(_clname);
}

int TaTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
