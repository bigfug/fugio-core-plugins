#include "togglenode.h"

#include <fugio/core/uuid.h>

#include <fugio/context_interface.h>
#include <fugio/core/variant_interface.h>

ToggleNode::ToggleNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_OUTPUT_BOOLEAN, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );

	mPinInput  = pinInput( "Trigger", PID_FUGIO_NODE_TRIGGER );

	mValOutput = pinOutput<fugio::VariantInterface *>( "Boolean", mPinOutput, PID_BOOL, PIN_OUTPUT_BOOLEAN );

	mValOutput->setVariant( false );
}

void ToggleNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	if( mPinInput->isUpdated( pTimeStamp ) )
	{
		if( mValOutput->variant().toBool() )
		{
			mValOutput->setVariant( false );
		}
		else
		{
			mValOutput->setVariant( true );
		}

		mNode->context()->pinUpdated( mPinOutput );
	}
}
