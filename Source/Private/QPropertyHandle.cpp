#include "QPropertyHandle.h"
#include <QRegularExpression>
#include "PropertyHandleImpl/QPropertyHandleImpl_Sequential.h"
#include "PropertyHandleImpl/QPropertyHandleImpl_Associative.h"
#include "PropertyHandleImpl/QPropertyHandleImpl_Enum.h"
#include "PropertyHandleImpl/QPropertyHandleImpl_Object.h"
#include "PropertyHandleImpl/QPropertyHandleImpl_RawType.h"

QPropertyHandle::QPropertyHandle(QObject* inParent, QMetaType inType, QString inPropertyPath, Getter inGetter, Setter inSetter)
	: mType(inType)
	, mPropertyPath(inPropertyPath)
	, mGetter(inGetter)
	, mSetter(inSetter)
{
	setParent(inParent);
	setObjectName(inPropertyPath.split(".").back());
	resloveMetaData();
	mInitialValue = inGetter();
	mPropertyType = parserType(inType);
	switch (mPropertyType)
	{
	default:
		break;
	case PropertyType::RawType:
		mImpl.reset(new QPropertyHandleImpl_RawType(this));
		break;
	case PropertyType::Enum:
		mImpl.reset(new QPropertyHandleImpl_Enum(this));
		break;
	case PropertyType::Sequential:
		mImpl.reset(new QPropertyHandleImpl_Sequential(this));
		break;
	case PropertyType::Associative:
		mImpl.reset(new QPropertyHandleImpl_Associative(this));
		break;
	case PropertyType::Object:
		mImpl.reset(new QPropertyHandleImpl_Object(this));
		break;
	}
}

void QPropertyHandle::resloveMetaData()
{
	auto metaObj = parent()->metaObject();
	auto firstField = getPropertyPath().split(".").first();
	for (int i = 0; i < metaObj->classInfoCount(); i++) {
		auto metaClassInfo = metaObj->classInfo(i);
		if (metaClassInfo.name() == firstField) {
			QStringList fields = QString(metaClassInfo.value()).split(",", Qt::SplitBehaviorFlags::SkipEmptyParts);
			for (auto field : fields) {
				QStringList pair = field.split("=");
				QString key, value;
				if (pair.size() > 0) {
					key = pair.first().trimmed();
				}
				if (pair.size() > 1) {
					value = pair[1].trimmed();
				}
				mMetaData[key] = value;
			}
			return;
		}
	}
}

QPropertyHandle* QPropertyHandle::Find(const QObject* inParent, const QString& inPropertyPath)
{
	for (QObject* child : inParent->children()) {
		QPropertyHandle* handler = qobject_cast<QPropertyHandle*>(child);
		if (handler && handler->getPropertyPath() == inPropertyPath) {
			return handler;
		}
	}
	return nullptr;
}

QPropertyHandle* QPropertyHandle::FindOrCreate(QObject* inParent, QMetaType inType, QString inPropertyPath, Getter inGetter, Setter inSetter)
{
	QPropertyHandle* handle = Find(inParent, inPropertyPath);
	if (handle)
		return handle;
	return new QPropertyHandle(
		inParent,
		inType,
		inPropertyPath,
		inGetter,
		inSetter
	);
}

QPropertyHandle* QPropertyHandle::FindOrCreate(QObject* inObject)
{
	QPropertyHandle* handle = Find(inObject, "");
	if (handle)
		return handle;
	return new QPropertyHandle(
		inObject,
		inObject->metaObject()->metaType(),
		"",
		[inObject]() {return QVariant::fromValue(inObject); },
		[inObject](QVariant var) {}
	);
}

QPropertyHandle* QPropertyHandle::Create(QObject* inParent, QMetaType inType, QString inPropertyPath, Getter inGetter, Setter inSetter)
{
	return new QPropertyHandle(inParent, inType, inPropertyPath, inGetter, inSetter);
}

QMetaType QPropertyHandle::getType()
{
	return mType;
}

QPropertyHandle::PropertyType QPropertyHandle::getPropertyType() const
{
	return mPropertyType;
}

QString QPropertyHandle::getName()
{
	return objectName();
}

QString QPropertyHandle::getPropertyPath()
{
	return mPropertyPath;
}

QString QPropertyHandle::createSubPath(const QString& inSubName)
{
	return getPropertyPath() + "." + inSubName;
}

void QPropertyHandle::invalidateStructure()
{
	Q_EMIT asStructureChanged();
}

QVariant QPropertyHandle::getVar()
{
	return mGetter();
}

void QPropertyHandle::setVar(QVariant var)
{
	QVariant lastVar = mGetter();
	if (lastVar != var) {
		mSetter(var);
		Q_EMIT asVarChanged(var);
		QVariant currVar = mGetter();
		if (currVar != var) {
			Q_EMIT asRequestRollback(currVar);
		}
	}
}

bool QPropertyHandle::hasMetaData(const QString& inName) const
{
	return mMetaData.contains(inName);
}

QVariant QPropertyHandle::getMetaData(const QString& inName) const
{
	return mMetaData.value(inName);
}

const QVariantHash& QPropertyHandle::getMetaDataMap() const
{
	return mMetaData;
}

QPropertyHandle* QPropertyHandle::findChild(QString inPropertyName)
{
	return Find(this->parent(), this->createSubPath(inPropertyName));
}

QPropertyHandle* QPropertyHandle::findOrCreateChild(QMetaType inType, QString inPropertyName, Getter inGetter, Setter inSetter)
{
	QPropertyHandle* childHandle = QPropertyHandle::FindOrCreate(
		this->parent(),
		inType,
		this->createSubPath(inPropertyName),
		inGetter,
		inSetter
	);
	childHandle->disconnect(this);
	connect(this, &QPropertyHandle::asVarChanged, childHandle, [childHandle](QVariant) {
		QVariant var = childHandle->getVar();
		Q_EMIT childHandle->asRequestRollback(var);
	});
	connect(this, &QPropertyHandle::asRequestRollback, childHandle, [childHandle](QVariant) {
		QVariant var = childHandle->getVar();
		Q_EMIT childHandle->asRequestRollback(var);
		});
	return childHandle;
}

QQuickItem* QPropertyHandle::setupNameEditor(QQuickItem* inParent)
{
	return mImpl->createNameEditor(inParent);
}

QQuickItem* QPropertyHandle::steupValueEditor(QQuickItem* inParent)
{
	return mImpl->createValueEditor(inParent);
}

IPropertyHandleImpl::Type QPropertyHandle::type()
{
	return mImpl->type();
}

QPropertyHandleImpl_Enum* QPropertyHandle::asEnum()
{
	return static_cast<QPropertyHandleImpl_Enum*>(mImpl.get());
}

QPropertyHandleImpl_Object* QPropertyHandle::asObject()
{
	return static_cast<QPropertyHandleImpl_Object*>(mImpl.get());
}

QPropertyHandleImpl_Associative* QPropertyHandle::asAssociative()
{
	return static_cast<QPropertyHandleImpl_Associative*>(mImpl.get());
}

QPropertyHandleImpl_Sequential* QPropertyHandle::asSequential()
{
	return static_cast<QPropertyHandleImpl_Sequential*>(mImpl.get());
}

QPropertyHandle::PropertyType QPropertyHandle::parserType(QMetaType inType)
{
	if (QMetaType::canConvert(inType, QMetaType::fromType<QVariantList>())
		&& !QMetaType::canConvert(inType, QMetaType::fromType<QString>())
		) {
		return Sequential;
	}
	else if (QMetaType::canConvert(inType, QMetaType::fromType<QVariantMap>())) {
		return Associative;
	}
	else if (inType.flags() & QMetaType::IsEnumeration) {
		return Enum;
	}
	else {
		QRegularExpression reg("QSharedPointer\\<(.+)\\>");
		QRegularExpressionMatch match = reg.match(inType.name(), 0, QRegularExpression::MatchType::PartialPreferCompleteMatch, QRegularExpression::AnchorAtOffsetMatchOption);
		QStringList matchTexts = match.capturedTexts();
		QMetaType innerMetaType;
		if (!matchTexts.isEmpty()) {
			QString metaTypeName = matchTexts.back();
			innerMetaType = QMetaType::fromName(metaTypeName.toLocal8Bit());
		}
		if (innerMetaType.metaObject() || inType.metaObject()) {
			return Object;
		}
		else {
			return RawType;
		}
	}
}

QVariant QPropertyHandle::createNewVariant(QMetaType inOutputType)
{
	QRegularExpression reg("QSharedPointer\\<(.+)\\>");
	QRegularExpressionMatch match = reg.match(inOutputType.name());
	QStringList matchTexts = match.capturedTexts();
	if (!matchTexts.isEmpty()) {
		QMetaType innerMetaType = QMetaType::fromName((matchTexts.back()).toLocal8Bit());
		if (innerMetaType.isValid()) {
			void* ptr = innerMetaType.create();
			QVariant sharedPtr(inOutputType);
			memcpy(sharedPtr.data(), &ptr, sizeof(ptr));
			QtSharedPointer::ExternalRefCountData* data = ExternalRefCountWithMetaType::create(innerMetaType, ptr);
			memcpy((char*)sharedPtr.data() + sizeof(ptr), &data, sizeof(data));
			return sharedPtr;
		}
	}
	else if (inOutputType.flags().testFlag(QMetaType::IsPointer)) {
		const QMetaObject* metaObject = inOutputType.metaObject();
		if (metaObject && metaObject->inherits(&QObject::staticMetaObject)) {
			QObject* obj = metaObject->newInstance();
			if (obj)
				return QVariant::fromValue(obj);
		}
		QMetaType innerMetaType = QMetaType::fromName(QString(inOutputType.name()).remove("*").toLocal8Bit());
		if (innerMetaType.isValid()) {
			void* ptr = innerMetaType.create();
			QVariant var(inOutputType, ptr);
			memcpy(var.data(), &ptr, sizeof(ptr));
			return var;
		}
		else {
			return QVariant();
		}
	}
	return QVariant(inOutputType);
}
