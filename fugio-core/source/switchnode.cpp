#include "switchnode.h"

#include <QSettings>

#include "fugio/global_interface.h"

SwitchNode::SwitchNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode ), mCurrentIndex( -1 )
{
	FUGID( PIN_INPUT_VALUE,			"9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_INPUT_1,		"1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_INPUT_2,		"261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_OUTPUT_VALUE,		"249f2932-f483-422f-b811-ab679f006381" );

	mPinInputValue = pinInput( "InputValue", PIN_INPUT_VALUE );

	mPinInputValue->setValue( 0.0 );

	pinInput( "Input1", PIN_INPUT_INPUT_1 );
	pinInput( "Input2", PIN_INPUT_INPUT_2 );

	mPinOutputValue = pinOutput( "Value", PIN_OUTPUT_VALUE );
}

SwitchNode::~SwitchNode( void )
{
}

void SwitchNode::inputsUpdated( qint64 pTimeStamp )
{
	bool		IOK = false;
	double		IDX;

	NodeControlBase::inputsUpdated( pTimeStamp );

	if( mPinInputValue->isConnected() )
	{
		QSharedPointer<fugio::PinControlInterface>		 CTL = mPinInputValue->connectedPin()->control();

		fugio::VariantInterface						*VAR;

		if( ( VAR = qobject_cast<fugio::VariantInterface *>( CTL->qobject() ) ) != 0 )
		{
			IDX = VAR->variant().toDouble( &IOK );
		}
	}
	else
	{
		IDX = mPinInputValue->value().toDouble( &IOK );
	}

	if( !IOK )
	{
		return;
	}

	QList< QSharedPointer<fugio::PinInterface> >	PINS = mNode->enumInputPins();

	PINS.removeAll( mPinInputValue );

	if( PINS.isEmpty() )
	{
		return;
	}

	int		CIDX = qMin( PINS.size() - 1, int( double( PINS.size() ) * IDX ) );

	if( mCurrentIndex != CIDX )
	{
		if( PINS[ CIDX ]->isConnected() )
		{
			mPinOutputValue->setControl( PINS[ CIDX ]->connectedPin()->control() );
		}

		mCurrentIndex = CIDX;
	}

	pinUpdated( mPinOutputValue );
}


bool SwitchNode::canAcceptPin(fugio::PinInterface *pPin) const
{
	if( pPin->direction() == PIN_INPUT )
	{
		return( false );
	}

	return( true );
}
