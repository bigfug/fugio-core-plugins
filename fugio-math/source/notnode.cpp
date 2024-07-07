#include "notnode.h"

#include <fugio/core/uuid.h>

#include <fugio/context_interface.h>

#include <fugio/pin_variant_iterator.h>

NotNode::NotNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_BOOLEAN, "c13a41c6-544b-46bb-a9f2-19dd156d236c" );
	FUGID( PIN_OUTPUT_BOOLEAN, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );

	mPinInput = pinInput( "Boolean", PIN_INPUT_BOOLEAN );

	mValOutput = pinOutput<fugio::VariantInterface *>( "Boolean", mPinOutput, PID_BOOL, PIN_OUTPUT_BOOLEAN );
}

void NotNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	bool		Update = mPinOutput->alwaysUpdate();

	fugio::VariantInterface		*SrcVar = input<fugio::VariantInterface *>( mPinInput );

	if( !SrcVar )
	{
		return;
	}

	variantSetCount( mValOutput, SrcVar->variantCount(), Update );

	for( int i = 0 ; i < SrcVar->variantCount() ; i++ )
	{
		variantSetValue( mValOutput, i, !SrcVar->variant( i ).toBool(), Update );
	}

	pinUpdated( mPinOutput, Update );
}
