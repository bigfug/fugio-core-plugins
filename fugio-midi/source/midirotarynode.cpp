#include "midirotarynode.h"

#include <fugio/core/uuid.h>

#include <fugio/context_interface.h>

MidiRotaryNode::MidiRotaryNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_MIDI_CONTROL, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_FORCE, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_OUTPUT_FORCE, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );

	mPinInput = pinInput( "MIDI Control", PIN_INPUT_MIDI_CONTROL );
	mPinForce = pinInput( "Force", PIN_INPUT_FORCE );

	mValOutput = pinOutput<fugio::VariantInterface *>( "Force", mPinOutput, PID_FLOAT, PIN_OUTPUT_FORCE );

	mPinForce->setValue( 0.1 );
}

void MidiRotaryNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	double		Force = mPinForce->value().toDouble();

	fugio::VariantInterface	*V;

	if( ( V = input<fugio::VariantInterface *>( mPinForce ) ) )
	{
		Force = V->variant().toDouble();
	}

	if( ( V = input<fugio::VariantInterface *>( mPinInput ) ) )
	{
		if( V->variant().toInt() >= 63 )
		{
			Force *= -1.0;
		}
		else
		{
			Force *=  1.0;
		}
	}

	mValOutput->setVariant( Force );

	mNode->context()->pinUpdated( mPinOutput );
}
