/****************************************************************************
** Meta object code from reading C++ file 'EFFEditorScenePanel.h'
**
** Created: Wed Oct 24 23:38:43 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StdAfx.h"
#include "..\..\Include\EFFEditorScenePanel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EFFEditorScenePanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EFFEditorScenePanel[] = {

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
      21,   20,   20,   20, 0x0a,
      51,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EFFEditorScenePanel[] = {
    "EFFEditorScenePanel\0\0drawModeMenuPressed(QAction*)\0"
    "renderModeMenuPressed(QAction*)\0"
};

void EFFEditorScenePanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EFFEditorScenePanel *_t = static_cast<EFFEditorScenePanel *>(_o);
        switch (_id) {
        case 0: _t->drawModeMenuPressed((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: _t->renderModeMenuPressed((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EFFEditorScenePanel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EFFEditorScenePanel::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_EFFEditorScenePanel,
      qt_meta_data_EFFEditorScenePanel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EFFEditorScenePanel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EFFEditorScenePanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EFFEditorScenePanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EFFEditorScenePanel))
        return static_cast<void*>(const_cast< EFFEditorScenePanel*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int EFFEditorScenePanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
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
