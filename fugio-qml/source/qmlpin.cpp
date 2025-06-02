#include "qmlpin.h"

#include <QQmlEngine>

#include <fugio/pin_signals.h>
#include <fugio/pin_control_interface.h>
#include <fugio/node_interface.h>
#include <fugio/context_interface.h>

#include <fugio/core/variant_interface.h>

#include "qmlplugin.h"

#include "qmlnode.h"

QMLPin::QMLPin(QObject *parent)
	: QObject( parent ), mControl( nullptr ), mNode( nullptr )
{

}

QMLPin::QMLPin( QSharedPointer<fugio::PinInterface> pPin )
	: mPin( pPin ), mControl( nullptr ), mNode( nullptr )
{
	connect( mPin->qobject(), SIGNAL(nameChanged(QString)), this, SLOT(setName(QString)) );

	connect( mPin->qobject(), SIGNAL(valueChanged(QVariant)), this, SLOT(setValue(QVariant)) );
}

QObject *QMLPin::connectedPin()
{
	QSharedPointer<fugio::PinInterface>		 P = ( mPin ? mPin->connectedPin() : nullptr );

	QMLPin									*Q = P ? new QMLPin( P ) : nullptr;

	if( Q )
	{
		QQmlEngine::setObjectOwnership( Q, QQmlEngine::JavaScriptOwnership );
	}

	return( Q );
}

QObject *QMLPin::findInterface( const QString &pUUID )
{
	QUuid		 UUID( pUUID );
	QObject		*PinInt = 0;

	if( !UUID.isNull() )
	{
		PinInt = mPin->findInterface( UUID );
	}
	else
	{
		for( QObject *TmpInt : mPin->interfaces() )
		{
			QString		TmpCls = TmpInt->metaObject()->className();

			if( TmpCls == pUUID )
			{
				PinInt = TmpInt;

				break;
			}
		}
	}

	if( PinInt )
	{
		QQmlEngine::setObjectOwnership( PinInt, QQmlEngine::CppOwnership );
	}

	return( PinInt );
}

QObject *QMLPin::control() const
{
	return( mControl );
}

QString QMLPin::name() const
{
	return( mPin ? mPin->name() : mName );
}

QVariant QMLPin::value() const
{
	return( mPin ? mPin->value() : QVariant() );
}

bool QMLPin::isUpdated( qint64 pTimeStamp )
{
	return( mPin ? mPin->isUpdated( pTimeStamp ) : false );
}

void QMLPin::setName( QString name )
{
	mName = name;

	if( mPin )
	{
		mPin->setName( name );
	}
}

void QMLPin::setValue( QVariant value )
{
	if( mPin )
	{
		mPin->setValue( value );
	}
}

void QMLPin::signalNameChanged( QString name )
{
	emit nameChanged( name );
}

void QMLPin::signalValueChanged( QVariant value )
{
	emit valueChanged( value );
}

QVariant QMLPin::variant( int pIndex )
{
	if( mPin->direction() == PIN_OUTPUT )
	{
		fugio::VariantInterface	*V = qobject_cast<fugio::VariantInterface *>( mPin->control()->qobject() );

		if( V )
		{
			if( V->variantCount() <= pIndex )
			{
				return( QVariant() );
			}

			return( V->variant( pIndex ) );
		}

		return( QVariant() );
	}

	if( mPin->isConnected() && mPin->connectedPin()->hasControl() )
	{
		fugio::VariantInterface	*V = qobject_cast<fugio::VariantInterface *>( mPin->connectedPin()->control()->qobject() );

		if( V )
		{
			if( V->variantCount() <= pIndex )
			{
				return( QVariant() );
			}

			return( V->variant( pIndex ) );
		}
	}

	return( mPin->value() );
}

int QMLPin::variantCount( void )
{
	if( mPin->direction() != PIN_INPUT )
	{
		return( 0 );
	}

	if( mPin->isConnected() && mPin->connectedPin()->hasControl() )
	{
		fugio::VariantInterface	*V = qobject_cast<fugio::VariantInterface *>( mPin->connectedPin()->control()->qobject() );

		if( V )
		{
			return( V->variantCount() );
		}
	}

	return( 0 );
}

void QMLPin::setVariant( QVariant pValue )
{
	setVariant( 0, pValue );
}

void QMLPin::setVariant( int pIndex, QVariant pValue )
{
	if( mPin->direction() != PIN_OUTPUT || !mPin->hasControl() )
	{
		return;
	}

	fugio::VariantInterface	*V = qobject_cast<fugio::VariantInterface *>( mPin->control()->qobject() );

	if( V )
	{
		if( mPin->alwaysUpdate() || V->variant( pIndex ) != pValue )
		{
			V->setVariant( pIndex, 0, pValue );

			mPin->node()->context()->pinUpdated( mPin );
		}
	}
}

void QMLPin::trigger()
{
	mPin->node()->context()->pinUpdated( mPin );
}

void QMLPin::initialise()
{
	QMLNode *Node = qobject_cast<QMLNode *>( mNode );

	if( !Node )
	{
		return;
	}

	QUuid		PinLocalId = QUuid::fromString( mUuid );

	mPin = Node->sharedNode()->findPinByLocalId( PinLocalId );

	if( !mPin )
	{
		QUuid	PinType = QMLPlugin::app()->findPinByClass( mType );

		if( PinType.isNull() )
		{
			return;
		}

		mControl = Node->sharedNode()->createPin( mName, PIN_OUTPUT, QUuid::createUuid(), PinLocalId, mPin, PinType );

		if( !mPin )
		{
			return;
		}
	}

	connect( mPin->qobject(), SIGNAL(nameChanged(QString)), this, SLOT(setName(QString)) );

	connect( mPin->qobject(), SIGNAL(valueChanged(QVariant)), this, SLOT(setValue(QVariant)) );
}

int QMLPin::count()
{
	return( variantCount() );
}

void QMLPin::setCount( int pCount )
{
	if( mPin->direction() != PIN_OUTPUT || !mPin->hasControl() )
	{
		return;
	}

	fugio::VariantInterface	*V = qobject_cast<fugio::VariantInterface *>( mPin->control()->qobject() );

	if( V )
	{
		V->setVariantCount( pCount );
	}
}

QString QMLPin::type()
{
	return( mType );
}

QString QMLPin::uuid()
{
	return( mUuid );
}

void QMLPin::setType( QString type )
{
	QUuid	TypeClassUuid = QMLPlugin::app()->findPinByClass( type );

	qDebug() << TypeClassUuid;

	mType = type;
}

void QMLPin::setUuid(QString uuid)
{
	QUuid	PinUid = QUuid::fromString( uuid );

	qDebug() << PinUid;

	mUuid = uuid;
}

void QMLPin::setNode(QObject *node)
{
	mNode = node;
}

