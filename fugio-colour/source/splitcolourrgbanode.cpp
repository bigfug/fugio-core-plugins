#include "splitcolourrgbanode.h"

#include <fugio/context_interface.h>

SplitColourRGBANode::SplitColourRGBANode( QSharedPointer<fugio::NodeInterface> pNode ) :
	NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_COLOUR, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUT_R, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_OUT_G, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_OUT_B, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_OUT_A, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );

	mPinInput = pinInput( "Colour", PIN_INPUT_COLOUR );

	mPinInput->registerPinInputType( PID_COLOUR );

	mOutR = pinOutput<fugio::VariantInterface *>( "Red", mPinOutR, PID_FLOAT, PIN_OUT_R );

	mOutG = pinOutput<fugio::VariantInterface *>( "Green", mPinOutG, PID_FLOAT, PIN_OUT_G );

	mOutB = pinOutput<fugio::VariantInterface *>( "Blue", mPinOutB, PID_FLOAT, PIN_OUT_B );

	mOutA = pinOutput<fugio::VariantInterface *>( "Alpha", mPinOutA, PID_FLOAT, PIN_OUT_A );
}

void SplitColourRGBANode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	QColor							 C;

	fugio::ColourInterface			*ColInt = input<fugio::ColourInterface *>( mPinInput );

	if( ColInt )
	{
		C = ColInt->colour();
	}
	else
	{
		C = variant( mPinInput ).value<QColor>();
	}

	if( mOutR->variant().toFloat() != C.redF() )
	{
		mOutR->setVariant( C.redF() );

		mNode->context()->pinUpdated( mPinOutR );
	}

	if( mOutG->variant().toFloat() != C.greenF() )
	{
		mOutG->setVariant( C.greenF() );

		mNode->context()->pinUpdated( mPinOutG );
	}

	if( mOutB->variant().toFloat() != C.blueF() )
	{
		mOutB->setVariant( C.blueF() );

		mNode->context()->pinUpdated( mPinOutB );
	}

	if( mOutA->variant().toFloat() != C.alphaF() )
	{
		mOutA->setVariant( C.alphaF() );

		mNode->context()->pinUpdated( mPinOutA );
	}
}
