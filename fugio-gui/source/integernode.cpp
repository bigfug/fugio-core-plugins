#include "integernode.h"

#include <QSpinBox>
#include <QDateTime>
#include <QSettings>

#include <fugio/node_interface.h>
#include <fugio/context_interface.h>

IntegerNode::IntegerNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_OUTPUT_NUMBER, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );

	pinInput( "Trigger", PID_FUGIO_NODE_TRIGGER );

	mValue = pinOutput<fugio::VariantInterface *>( "Number", mPinValue, PID_INTEGER, PIN_OUTPUT_NUMBER );

	mPinValue->setDescription( tr( "The output Number" ) );
}

QWidget *IntegerNode::gui()
{
	QSpinBox		*GUI = new QSpinBox();

	GUI->setMaximum(  999999999 );
	GUI->setMinimum( -999999999 );

	GUI->setValue( mValue->variant().toInt() );

	connect( GUI, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)) );

	connect( this, SIGNAL(valueUpdated(int)), GUI, SLOT(setValue(int)) );

	return( GUI );
}

void IntegerNode::loadSettings( QSettings &pSettings )
{
	valueChanged( pSettings.value( "Value" ).toInt() );
}

void IntegerNode::saveSettings( QSettings &pSettings ) const
{
	pSettings.setValue( "Value", mValue->variant().toInt() );
}

void IntegerNode::valueChanged( int pValue )
{
	if( pValue == mValue->variant().toInt() )
	{
		return;
	}

	mValue->setVariant( pValue );

	pinUpdated( mPinValue );

	emit valueUpdated( pValue );
}

void IntegerNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	pinUpdated( mPinValue );
}
