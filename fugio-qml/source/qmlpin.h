#ifndef QMLPIN_H
#define QMLPIN_H

#include <QObject>
#include <QQmlListProperty>
#include <QQmlEngine>

#include <fugio/pin_interface.h>
#include <fugio/pin_control_interface.h>

class QMLPin : public QObject
{
	Q_OBJECT

	Q_PROPERTY( QObject *control READ control CONSTANT )
	Q_PROPERTY( QObject *connectedPin READ connectedPin )
	Q_PROPERTY( QVariant value READ value WRITE setValue NOTIFY valueChanged )
	Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged )
	Q_PROPERTY( int count READ count WRITE setCount )
	Q_PROPERTY( QString type READ type WRITE setType CONSTANT )
	Q_PROPERTY( QString uuid READ uuid WRITE setUuid CONSTANT )
	Q_PROPERTY( QObject *node READ node WRITE setNode )

	QML_NAMED_ELEMENT( "Pin" )

public:
	explicit QMLPin( QObject *parent = nullptr );

	explicit QMLPin( QSharedPointer<fugio::PinInterface> pPin );

	virtual ~QMLPin( void ) {}

	// QObjects returned here must deletable or marked as Cpp owned

	Q_INVOKABLE QObject *findInterface( const QString &pUUID );

	// Not Q_INVOLKABLE to retain ownership of returned pointer

	Q_INVOKABLE QObject *control( void ) const;

	QObject *connectedPin( void );

	QString name() const;

	Q_INVOKABLE QVariant value() const;

	Q_INVOKABLE bool isUpdated( qint64 pTimeStamp );

	Q_INVOKABLE QVariant variant( int pIndex = 0 );

	Q_INVOKABLE int variantCount( void );

	Q_INVOKABLE void setVariant( QVariant pValue );

	Q_INVOKABLE void setVariant( int pIndex, QVariant pValue );

	Q_INVOKABLE void trigger( void );

	Q_INVOKABLE void initialise( void );

	int count( void );

	void setCount( int pCount );

	QString type( void );

	QString uuid( void );

	QObject *node( void ) const
	{
		return( mNode );
	}

signals:
	void nameChanged( QString name );

	void valueChanged( QVariant value );

	void updated( qint64 timestamp );

public slots:
	void setName( QString name );

	void setValue( QVariant value );

	void setType( QString type );

	void setUuid( QString uuid );

	void setNode( QObject *node );

private slots:
	void signalNameChanged( QString name );

	void signalValueChanged( QVariant value );

private:
	QSharedPointer<fugio::PinInterface>		 mPin;
	QString									 mName;
	QString									 mType;
	QString									 mUuid;
	QObject									*mNode;
};

#endif // QMLPIN_H
