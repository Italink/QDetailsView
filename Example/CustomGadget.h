#ifndef CustomGadget_h__
#define CustomGadget_h__

#include "CommonInclude.h"

class QCustomGadget {
	Q_GADGET
	Q_CLASSINFO("LimitedDouble", "Min=0,Max=10")
public:
	Q_PROPERTY_VAR(double, LimitedDouble) = 1;
	Q_PROPERTY_VAR(QString, Desc) = "This is inline Gadget";
};

static QDebug operator<<(QDebug debug, const QCustomGadget& gadget) {
	return debug << gadget.LimitedDouble << gadget.Desc;
}

Q_DECLARE_METATYPE(QSharedPointer<QCustomGadget>);

#endif // CustomGadget_h__