#include "lcdnumbernode.h"

#include <QLCDNumber>

LcdNumberNode::LcdNumberNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode ), mValInput( 0 )
{
	FUGID( PIN_INPUT_NUMBER, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );

	mPinInput = pinInput( "Number", PIN_INPUT_NUMBER );
}

LcdNumberNode::~LcdNumberNode()
{

}

bool LcdNumberNode::initialise()
{
	if( !fugio::NodeControlBase::initialise() )
	{
		return( false );
	}

	return( true );
}

QWidget *LcdNumberNode::gui()
{
	QLCDNumber		*GUI = new QLCDNumber();

	GUI->setSegmentStyle( QLCDNumber::Flat );

	GUI->display( mValInput );

	connect( this, SIGNAL(valueUpdated(double)), GUI, SLOT(display(double)) );

	return( GUI );
}

void LcdNumberNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	double		NewVal = variant( mPinInput ).toDouble();

	if( NewVal != mValInput )
	{
		mValInput = NewVal;

		emit valueUpdated( mValInput );
	}
}
