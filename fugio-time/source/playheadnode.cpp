#include "playheadnode.h"

#include <QSettings>
#include <QPushButton>
#include <QDateTime>

#include "fugio/global_interface.h"
#include "fugio/context_interface.h"

PlayheadNode::PlayheadNode( QSharedPointer<fugio::NodeInterface> pNode ) :
	NodeControlBase( pNode )
{
	FUGID( PIN_OUTPUT_TIME, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );

	mValTime = pinOutput<fugio::VariantInterface *>( "Time", mPinTime, PID_FLOAT, PIN_OUTPUT_TIME );
}

bool PlayheadNode::initialise()
{
	mNode->context()->registerPlayhead( this );

	return( true );
}

bool PlayheadNode::deinitialise()
{
	mNode->context()->unregisterPlayhead( this );

	return( true );
}

void PlayheadNode::playheadMove( qreal pTimeStamp )
{
	mValTime->setVariant( pTimeStamp );

	mNode->context()->pinUpdated( mPinTime );
}

bool PlayheadNode::playheadPlay( qreal pTimePrev, qreal pTimeCurr )
{
	Q_UNUSED( pTimePrev )

	mValTime->setVariant( pTimeCurr );

	mNode->context()->pinUpdated( mPinTime );

	return( true );
}
