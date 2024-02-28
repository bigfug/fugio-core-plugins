#include "utctimenode.h"


#include <fugio/core/uuid.h>
#include <fugio/time/uuid.h>
#include <fugio/context_signals.h>

#include <QDateTime>

UTCTimeNode::UTCTimeNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_OUTPUT_TIME, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );

	mPinInputTrigger = pinInput( tr( "Trigger" ), PID_FUGIO_NODE_TRIGGER );

	mValOutputTime = pinOutput<fugio::VariantInterface*>( tr( "DateTime" ), mPinOutputTime, PID_DATETIME, PIN_OUTPUT_TIME );
}

bool UTCTimeNode::initialise()
{
	if( !NodeControlBase::initialise() )
	{
		return( false );
	}

	connect( mNode->context()->qobject(), SIGNAL(frameStart(qint64)), this, SLOT(contextFrameStart(qint64)) );

	return( true );
}

bool UTCTimeNode::deinitialise()
{
	disconnect( mNode->context()->qobject(), SIGNAL(frameStart(qint64)), this, SLOT(contextFrameStart(qint64)) );

	return( NodeControlBase::deinitialise() );
}

void UTCTimeNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	updateTime();
}

void UTCTimeNode::contextFrameStart( qint64 pTimeStamp )
{
	if( !mPinInputTrigger->isConnected() )
	{
		updateTime();
	}
}

void UTCTimeNode::updateTime()
{
	QDateTime		LocalTime = QDateTime::currentDateTimeUtc();

	if( mValOutputTime->variant().toDateTime() != LocalTime )
	{
		mValOutputTime->setVariant( LocalTime );

		pinUpdated( mPinOutputTime );
	}
}
