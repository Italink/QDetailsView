#ifndef CustomType_h__
#define CustomType_h__

#include <QVector>
#include <QMetaType>

struct QCustomType {
	unsigned int ArraySize = 0;
	QVector<int> Array;
};

static QDebug operator<<(QDebug debug, const QCustomType& it) {
	return debug << it.Array;
}

Q_DECLARE_METATYPE(QCustomType)

#endif // CustomType_h__
