/****************************************************************************
** Meta object code from reading C++ file 'EFFEditorProjectPanel.h'
**
** Created: Thu Dec 13 22:24:23 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StdAfx.h"
#include "..\..\Include\EFFEditorProjectPanel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EFFEditorProjectPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EFFEditorProjecPanel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      27,   22,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EFFEditorProjecPanel[] = {
    "EFFEditorProjecPanel\0\0bAdd\0"
    "titleBarAddOrDeleteTab(bool)\0"
};

void EFFEditorProjecPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EFFEditorProjecPanel *_t = static_cast<EFFEditorProjecPanel *>(_o);
        switch (_id) {
        case 0: _t->titleBarAddOrDeleteTab((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EFFEditorProjecPanel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EFFEditorProjecPanel::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_EFFEditorProjecPanel,
      qt_meta_data_EFFEditorProjecPanel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EFFEditorProjecPanel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EFFEditorProjecPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EFFEditorProjecPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EFFEditorProjecPanel))
        return static_cast<void*>(const_cast< EFFEditorProjecPanel*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int EFFEditorProjecPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
