#ifndef CommonInclude_h__
#define CommonInclude_h__

#include <QObject>
#include <QSharedPointer>
#include <QDebug>
#include <QMetaType>

#define Q_PROPERTY_VAR(Type,Name)\
    Q_PROPERTY(Type Name READ get##Name WRITE set##Name) \
    Type get##Name(){ return Name; } \
    void set##Name(Type var){ \
        Name = var;  \
		qDebug() <<"Set" <<this <<#Name <<": " <<var; \
    } \
    Type Name

#endif // CommonInclude_h__
