#include "splitvector4node.h"

#include <QVector4D>

#include <fugio/context_interface.h>
#include <fugio/math/uuid.h>

SplitVector4Node::SplitVector4Node( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_VECTOR, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_X, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_OUTPUT_Y, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_OUTPUT_Z, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_OUTPUT_W, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );

	mPinInput = pinInput( "Vector4", PIN_INPUT_VECTOR );

	mPinInput->registerPinInputType( PID_VECTOR4 );

	mOutX = pinOutput<fugio::VariantInterface *>( "X", mPinOutX, PID_FLOAT, PIN_OUTPUT_X );

	mOutY = pinOutput<fugio::VariantInterface *>( "Y", mPinOutY, PID_FLOAT, PIN_OUTPUT_Y );

	mOutZ = pinOutput<fugio::VariantInterface *>( "Z", mPinOutZ, PID_FLOAT, PIN_OUTPUT_Z );

	mOutW = pinOutput<fugio::VariantInterface *>( "W", mPinOutW, PID_FLOAT, PIN_OUTPUT_W );
}

void SplitVector4Node::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	QVector4D				 V4 = variant( mPinInput ).value<QVector4D>();

	if( mOutX->variant().toFloat() != V4.x() )
	{
		mOutX->setVariant( V4.x() );

		pinUpdated( mPinOutX );
	}

	if( mOutY->variant().toFloat() != V4.y() )
	{
		mOutY->setVariant( V4.y() );

		pinUpdated( mPinOutY );
	}

	if( mOutZ->variant().toFloat() != V4.z() )
	{
		mOutZ->setVariant( V4.z() );

		pinUpdated( mPinOutZ );
	}

	if( mOutW->variant().toFloat() != V4.w() )
	{
		mOutW->setVariant( V4.w() );

		pinUpdated( mPinOutW );
	}
}
