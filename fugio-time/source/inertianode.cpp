#include "inertianode.h"

#include <cmath>

#include <fugio/core/uuid.h>

#include <fugio/context_interface.h>
#include <fugio/node_interface.h>
#include <fugio/context_interface.h>
#include <fugio/core/variant_interface.h>
#include <fugio/context_signals.h>

InertiaNode::InertiaNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode ), mTimeLast( -1 ), mValue( 0 ), mForce( 0 )
{
	FUGID( PIN_INPUT_INPUT, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_POSITION, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );

	mPinInput = pinInput( "Input", PIN_INPUT_INPUT );

	mValOutput = pinOutput<fugio::VariantInterface *>( "Position", mPinOutput, PID_FLOAT, PIN_OUTPUT_POSITION );
}

void InertiaNode::inputsUpdated( qint64 pTimeStamp )
{
	if( mPinInput->isConnectedToActiveNode() && mPinInput->isUpdated( pTimeStamp ) )
	{
		fugio::VariantInterface		*V = input<fugio::VariantInterface *>( mPinInput );

		if( V )
		{
			double	v = V->variant().toDouble();

			if( v > 0.0 )
			{
				mForce += fabs( v );
			}
			else if( v < 0.0 )
			{
				mForce -= fabs( v );
			}
		}
	}

	mNode->context()->pinUpdated( mPinOutput );
}

bool InertiaNode::initialise()
{
	if( !fugio::NodeControlBase::initialise() )
	{
		return( false );
	}

	connect( mNode->context()->qobject(), SIGNAL(frameProcess(qint64)), this, SLOT(onContextFrame(qint64)) );

	return( true );
}

void InertiaNode::onContextFrame( qint64 pTimeStamp )
{
	if( mTimeLast < 0 || mTimeLast > pTimeStamp )
	{
		mTimeLast = pTimeStamp;
	}

	double		TimeDiff = double( pTimeStamp - mTimeLast ) / 1000.0;

	mValue += mForce * TimeDiff;

	emit valueUpdated( fmod( mValue, 1.0 ) * 10000.0 );

	mForce -= mForce * TimeDiff;

	mValue = qBound( 0.0, mValue, mNode->context()->duration() );

	mValOutput->setVariant( mValue / mNode->context()->duration() );

	mNode->context()->pinUpdated( mPinOutput );

	mTimeLast = pTimeStamp;
}

QWidget *InertiaNode::gui()
{
	QDial					*GUI;

	if( ( GUI = new QDial() ) )
	{
		GUI->setMaximum( 10000 );

		GUI->setWrapping( true );
		GUI->setTracking( false );

		GUI->setValue( fmod( mValue, 1.0 ) * 10000.0 );

		connect( this, SIGNAL(valueUpdated(int)), GUI, SLOT(setValue(int)) );
	}

	return( GUI );
}
