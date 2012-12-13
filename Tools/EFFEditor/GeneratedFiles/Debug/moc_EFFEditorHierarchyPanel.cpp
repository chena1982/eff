/****************************************************************************
** Meta object code from reading C++ file 'EFFEditorHierarchyPanel.h'
**
** Created: Wed Oct 24 23:05:32 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StdAfx.h"
#include "..\..\Include\EFFEditorHierarchyPanel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EFFEditorHierarchyPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EFFEditorHierarchyPanel[] = {

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
      30,   25,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EFFEditorHierarchyPanel[] = {
    "EFFEditorHierarchyPanel\0\0bAdd\0"
    "titleBarAddOrDeleteTab(bool)\0"
};

void EFFEditorHierarchyPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EFFEditorHierarchyPanel *_t = static_cast<EFFEditorHierarchyPanel *>(_o);
        switch (_id) {
        case 0: _t->titleBarAddOrDeleteTab((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EFFEditorHierarchyPanel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EFFEditorHierarchyPanel::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_EFFEditorHierarchyPanel,
      qt_meta_data_EFFEditorHierarchyPanel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EFFEditorHierarchyPanel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EFFEditorHierarchyPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EFFEditorHierarchyPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EFFEditorHierarchyPanel))
        return static_cast<void*>(const_cast< EFFEditorHierarchyPanel*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int EFFEditorHierarchyPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
