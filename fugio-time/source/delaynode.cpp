#include "delaynode.h"

#include <QDateTime>
#include <QDebug>

#include <fugio/node_interface.h>
#include <fugio/context_interface.h>
#include <fugio/context_signals.h>
#include <fugio/core/uuid.h>
#include <fugio/variant_interface.h>

DelayNode::DelayNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode ), mTriggerTime( -1 )
{
	FUGID( PIN_INPUT_NUMBER, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_TRIGGER, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );

	mPinNumber = pinInput( "Number", PIN_INPUT_NUMBER );

	pinOutput<fugio::PinControlInterface *>( "Trigger", mPinTrigger, PID_TRIGGER, PIN_OUTPUT_TRIGGER );
}

void DelayNode::inputsUpdated( qint64 pTimeStamp )
{
	mTriggerTime = pTimeStamp + variant( mPinNumber ).toInt();

	if( mTriggerTime > pTimeStamp )
	{
		connect( mNode->context()->qobject(), SIGNAL(frameStart(qint64)), this, SLOT(frameStart(qint64)) );
	}
}

void DelayNode::frameStart( qint64 pTimeStamp )
{
	if( mTriggerTime < 0 || pTimeStamp < mTriggerTime )
	{
		return;
	}

	mNode->context()->pinUpdated( mPinTrigger );

	disconnect( mNode->context()->qobject(), SIGNAL(frameStart(qint64)), this, SLOT(frameStart(qint64)) );
}
