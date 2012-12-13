/****************************************************************************
** Meta object code from reading C++ file 'EFFEditorComponentPanel.h'
**
** Created: Wed Oct 24 23:05:32 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StdAfx.h"
#include "..\..\Include\EFFEditorComponentPanel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EFFEditorComponentPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EFFEditorComponentPanel[] = {

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
      31,   25,   24,   24, 0x0a,
      64,   25,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EFFEditorComponentPanel[] = {
    "EFFEditorComponentPanel\0\0state\0"
    "PropertyBranchWidgetClicked(int)\0"
    "ComponentBranchWidgetClicked(int)\0"
};

void EFFEditorComponentPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EFFEditorComponentPanel *_t = static_cast<EFFEditorComponentPanel *>(_o);
        switch (_id) {
        case 0: _t->PropertyBranchWidgetClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->ComponentBranchWidgetClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EFFEditorComponentPanel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EFFEditorComponentPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_EFFEditorComponentPanel,
      qt_meta_data_EFFEditorComponentPanel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EFFEditorComponentPanel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EFFEditorComponentPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EFFEditorComponentPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EFFEditorComponentPanel))
        return static_cast<void*>(const_cast< EFFEditorComponentPanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int EFFEditorComponentPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
