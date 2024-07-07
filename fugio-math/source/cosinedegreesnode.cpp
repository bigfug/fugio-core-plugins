#include "cosinedegreesnode.h"

#include <fugio/core/uuid.h>

#include <fugio/context_interface.h>

#include <qmath.h>

#include <fugio/pin_variant_iterator.h>

CosineDegreeNode::CosineDegreeNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_DEGREES, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_COS, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );

	mPinInput = pinInput( "Degrees", PIN_INPUT_DEGREES );

	mValOutput = pinOutput<fugio::VariantInterface *>( "Cos", mPinOutput, PID_FLOAT, PIN_OUTPUT_COS );
}

void CosineDegreeNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	fugio::PinVariantIterator	Input( mPinInput );

	bool	OutputUpdated = mPinOutput->alwaysUpdate();

	variantSetCount( mValOutput, Input.count(), OutputUpdated );

	for( int i = 0 ; i < Input.count() ; i++ )
	{
		variantSetValue( mValOutput, i, std::cos( ( Input.index( i ).toDouble() / 180.0 ) * M_PI ), OutputUpdated );
	}

	if( OutputUpdated )
	{
		pinUpdated( mPinOutput );
	}
}
