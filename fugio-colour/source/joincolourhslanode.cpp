#include "joincolourhslanode.h"

#include <QSettings>

#include "fugio/global_interface.h"
#include "fugio/context_interface.h"

JoinColourHSLANode::JoinColourHSLANode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_HUE, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_SATURATION, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_LIGHTNESS, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_INPUT_ALPHA, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_OUTPUT_COLOUR, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );

	mPinInputHue        = pinInput( "Hue", PIN_INPUT_HUE );
	mPinInputSaturation = pinInput( "Saturation", PIN_INPUT_SATURATION );
	mPinInputLightness  = pinInput( "Lightness", PIN_INPUT_LIGHTNESS );
	mPinInputAlpha      = pinInput( "Alpha", PIN_INPUT_ALPHA );

	mValOutputColour = pinOutput<fugio::ColourInterface *>( "Colour", mPinOutputColour, PID_COLOUR, PIN_OUTPUT_COLOUR );

	mPinInputHue->setDescription( tr( "The hue of the colour (0.0-1.0)" ) );

	mPinInputSaturation->setDescription( tr( "The saturation (gray to colour) of the colour (0.0-1.0)" ) );

	mPinInputLightness->setDescription( tr( "The lightness (black to white) of the colour (0.0-1.0)" ) );

	mPinInputAlpha->setDescription( tr( "The alpha (transparancy) of the colour (0.0-1.0)" ) );

	mPinOutputColour->setDescription( tr( "The calculated colour" ) );

	mPinInputHue->setValue( 0.0f );
	mPinInputSaturation->setValue( 0.5f );
	mPinInputLightness->setValue( 1.0f );
	mPinInputAlpha->setValue( 1.0f );
}

void JoinColourHSLANode::loadSettings( QSettings &pSettings )
{
	mColour = pSettings.value( "colour", mColour ).value<QColor>();
}

void JoinColourHSLANode::saveSettings( QSettings &pSettings ) const
{
	QVariant		V = mColour;

	pSettings.setValue( "colour", V );
}

qreal JoinColourHSLANode::value( QSharedPointer<fugio::PinInterface> pPin, qreal pVal )
{
	bool				 ValOk;
	qreal				 NewVal;

	if( !pPin->isConnected() )
	{
		NewVal = pPin->value().toReal( &ValOk );
	}
	else
	{
		fugio::VariantInterface	*IntVar = qobject_cast<fugio::VariantInterface *>( pPin->connectedPin()->control()->qobject() );

		if( IntVar == 0 )
		{
			return( pVal );
		}

		NewVal = IntVar->variant().toReal( &ValOk );
	}

	if( !ValOk )
	{
		return( pVal );
	}

	NewVal = qBound( 0.0, NewVal, 1.0 );

	return( NewVal );
}

void JoinColourHSLANode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	qreal		H = value( mPinInputHue, h );
	qreal		S = value( mPinInputSaturation, s );
	qreal		L = value( mPinInputLightness, l );
	qreal		A = value( mPinInputAlpha, a );

	if( h == H && s == S && l == L && a == A )
	{
		return;
	}

	QColor		C = QColor::fromHslF( H, S, L, A );

	if( !C.isValid() || C == mColour )
	{
		return;
	}

	mColour = C;

	mValOutputColour->setColour( C );

	mNode->context()->pinUpdated( mPinOutputColour );
}

