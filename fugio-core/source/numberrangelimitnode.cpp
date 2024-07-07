#include "numberrangelimitnode.h"

#include <fugio/global_interface.h>
#include <fugio/context_interface.h>
#include <fugio/core/uuid.h>

#include <QDoubleSpinBox>
#include <QDateTime>
#include <QSettings>
#include <QFormLayout>

NumberRangeLimitNode::NumberRangeLimitNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_NUMBER, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_MIN, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_MAX, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_OUTPUT_NUMBER, "249f2932-f483-422f-b811-ab679f006381" );

	mPinInputValue = pinInput( "Number", PIN_INPUT_NUMBER );

	mPinInputMinInput  = pinInput( "Number Min", PIN_INPUT_MIN );
	mPinInputMaxInput  = pinInput( "Number Max", PIN_INPUT_MAX );

	mPinInputMinInput->setValue( 0 );
	mPinInputMaxInput->setValue( 1 );

	mValue = pinOutput<fugio::VariantInterface *>( "Number", mPinOutputValue, PID_FLOAT, PIN_OUTPUT_NUMBER );
}

NumberRangeLimitNode::~NumberRangeLimitNode( void )
{
}

void NumberRangeLimitNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	double		ValI = variant( mPinInputValue ).toDouble();
	double		MinI = variant( mPinInputMinInput ).toDouble();
	double		MaxI = variant( mPinInputMaxInput ).toDouble();

	ValI = qBound( MinI, ValI, MaxI );

	if( ValI != mValue->variant().toDouble() )
	{
		mValue->setVariant( ValI );

		pinUpdated( mPinOutputValue );
	}
}
