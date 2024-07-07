#include "numberrangenode.h"

#include <fugio/global_interface.h>
#include <fugio/context_interface.h>
#include <fugio/core/uuid.h>

#include <QDoubleSpinBox>
#include <QDateTime>
#include <QSettings>
#include <QFormLayout>

NumberRangeNode::NumberRangeNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_VALUE, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_IN_MIN, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_IN_MAX, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_INPUT_OUT_MIN, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_INPUT_OUT_MAX, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );
	FUGID( PIN_OUTPUT_VALUE, "e6bf944e-5f46-4994-bd51-13c2aa6415b7" );

	mPinInputValue = pinInput( "Input Value", PIN_INPUT_VALUE );

	mPinInputMinInput  = pinInput( "Input Min", PIN_INPUT_IN_MIN );
	mPinInputMaxInput  = pinInput( "Input Max", PIN_INPUT_IN_MAX );
	mPinInputMinOutput = pinInput( "Output Min", PIN_INPUT_OUT_MIN );
	mPinInputMaxOutput = pinInput( "Output Max", PIN_INPUT_OUT_MAX );

	mPinInputMinInput->setValue( 0 );
	mPinInputMaxInput->setValue( 1 );
	mPinInputMinOutput->setValue( 0 );
	mPinInputMaxOutput->setValue( 1 );

	mValue = pinOutput<fugio::VariantInterface *>( "Output Value", mPinOutputValue, PID_FLOAT, PIN_OUTPUT_VALUE );
}

NumberRangeNode::~NumberRangeNode( void )
{
}

void NumberRangeNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	double		ValI = variant( mPinInputValue ).toDouble();
	double		MinI = variant( mPinInputMinInput ).toDouble();
	double		MaxI = variant( mPinInputMaxInput ).toDouble();
	double		MinO = variant( mPinInputMinOutput ).toDouble();
	double		MaxO = variant( mPinInputMaxOutput ).toDouble();

	double		ValO = 0;

	ValI = qBound( MinI, ValI, MaxI );

	double		RngI = MaxI - MinI;
	double		RngO = MaxO - MinO;

	ValI = ( ValI - MinI ) / RngI;

	ValO = ( ValI * RngO ) + MinO;

	if( ValO != mValue->variant().toDouble() )
	{
		mValue->setVariant( ValO );

		pinUpdated( mPinOutputValue );
	}
}
