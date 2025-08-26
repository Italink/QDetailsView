#ifndef QPropertyHandle_h__
#define QPropertyHandle_h__

#include <QObject>
#include <QQuickItem>
#include "PropertyHandleImpl/QPropertyHandleImpl_Enum.h"
#include "PropertyHandleImpl/QPropertyHandleImpl_Object.h"
#include "PropertyHandleImpl/QPropertyHandleImpl_Associative.h"
#include "PropertyHandleImpl/QPropertyHandleImpl_Sequential.h"

class IPropertyHandleImpl;

class QDETAILS_VIEW_API QPropertyHandle : public QObject {
	Q_OBJECT
	Q_PROPERTY(QVariant Var READ getVar WRITE setVar NOTIFY asVarChanged)
public:
	using Getter = std::function<QVariant()>;
	using Setter = std::function<void(QVariant)>;

	enum PropertyType {
		Unknown,
		RawType,
		Enum,
		Sequential,
		Associative,
		Object
	};

	static QPropertyHandle* Find(const QObject* inParent, const QString& inPropertyPath);
	static QPropertyHandle* FindOrCreate(QObject* inParent, QMetaType inType, QString inPropertyPath, Getter inGetter, Setter inSetter);
	static QPropertyHandle* FindOrCreate(QObject* inObject);
	static QPropertyHandle* Create(QObject* inParent, QMetaType inType, QString inPropertyPath, Getter inGetter, Setter inSetter);
	static void Cleanup(QObject* inParent);

	QMetaType getType();
	PropertyType getPropertyType() const;
	QString getName();
	QString getPropertyPath();
	QString createSubPath(const QString& inSubName);

	void invalidateStructure();

	Q_INVOKABLE QVariant getVar();
	Q_INVOKABLE void setVar(QVariant var);

	bool hasMetaData(const QString& inName) const;
	QVariant getMetaData(const QString& inName) const;
	const QVariantHash& getMetaDataMap() const;

	QPropertyHandle* findChild(QString inPropertyName);
	QPropertyHandle* findOrCreateChild(QMetaType inType, QString inPropertyName, QPropertyHandle::Getter inGetter, QPropertyHandle::Setter inSetter);

	QQuickItem* setupNameEditor(QQuickItem* inParent);
	QQuickItem* steupValueEditor(QQuickItem* inParent);

	IPropertyHandleImpl::Type type();
	QPropertyHandleImpl_Enum* asEnum();
	QPropertyHandleImpl_Object* asObject();
	QPropertyHandleImpl_Associative* asAssociative();
	QPropertyHandleImpl_Sequential* asSequential();

	static PropertyType parserType(QMetaType inType);
	static QVariant createNewVariant(QMetaType inOutputType);
Q_SIGNALS:
	void asVarChanged(QVariant);
	void asStructureChanged();
	void asRequestRollback(QVariant);
protected:
	QPropertyHandle(QObject* inParent, QMetaType inType, QString inPropertyPath, Getter inGetter, Setter inSetter);
	void resloveMetaData();
private:
	QMetaType mType;
	PropertyType mPropertyType;
	QString mPropertyPath;
	Getter mGetter;
	Setter mSetter;
	QVariant mInitialValue;
	QVariantHash mMetaData;
	QSharedPointer<IPropertyHandleImpl> mImpl;
};

struct QDETAILS_VIEW_API ExternalRefCountWithMetaType : public QtSharedPointer::ExternalRefCountData {
	typedef ExternalRefCountData Parent;
	QMetaType mMetaType;
	void* mData;

	static void deleter(ExternalRefCountData* self) {
		ExternalRefCountWithMetaType* that =
			static_cast<ExternalRefCountWithMetaType*>(self);
		that->mMetaType.destroy(that->mData);
		Q_UNUSED(that); // MSVC warns if T has a trivial destructor
	}

	static inline ExternalRefCountData* create(QMetaType inMetaType, void* inPtr)
	{
		ExternalRefCountWithMetaType* d = static_cast<ExternalRefCountWithMetaType*>(::operator new(sizeof(ExternalRefCountWithMetaType)));

		// initialize the d-pointer sub-object
		// leave d->data uninitialized
		new (d) Parent(ExternalRefCountWithMetaType::deleter); // can't throw
		d->mData = inPtr;
		d->mMetaType = inMetaType;
		return d;
	}
};


#endif // QPropertyHandle_h__
