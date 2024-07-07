#include "splitcolourhslanode.h"

#include <fugio/context_interface.h>

SplitColourHSLANode::SplitColourHSLANode( QSharedPointer<fugio::NodeInterface> pNode ) :
	NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_COLOUR, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_HUE, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_OUTPUT_SATURATION, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_OUTPUT_LIGHTNESS, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_OUTPUT_ALPHA, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );

	mPinInput = pinInput( "Colour", PIN_INPUT_COLOUR );

	mPinInput->registerPinInputType( PID_COLOUR );

	mOutH = pinOutput<fugio::VariantInterface *>( "Hue", mPinOutH, PID_FLOAT, PIN_OUTPUT_HUE );

	mOutS = pinOutput<fugio::VariantInterface *>( "Saturation", mPinOutS, PID_FLOAT, PIN_OUTPUT_SATURATION );

	mOutL = pinOutput<fugio::VariantInterface *>( "Lightness", mPinOutL, PID_FLOAT, PIN_OUTPUT_LIGHTNESS );

	mOutA = pinOutput<fugio::VariantInterface *>( "Alpha", mPinOutA, PID_FLOAT, PIN_OUTPUT_ALPHA );
}

void SplitColourHSLANode::inputsUpdated( qint64 pTimeStamp )
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

	if( mOutH->variant().toFloat() != C.hslHueF() )
	{
		mOutH->setVariant( C.hslHueF() );

		mNode->context()->pinUpdated( mPinOutH );
	}

	if( mOutS->variant().toFloat() != C.hslSaturationF() )
	{
		mOutS->setVariant( C.hslSaturationF() );

		mNode->context()->pinUpdated( mPinOutS );
	}

	if( mOutL->variant().toFloat() != C.lightnessF() )
	{
		mOutL->setVariant( C.lightnessF() );

		mNode->context()->pinUpdated( mPinOutL );
	}

	if( mOutA->variant().toFloat() != C.alphaF() )
	{
		mOutA->setVariant( C.alphaF() );

		mNode->context()->pinUpdated( mPinOutA );
	}
}
