#include "splitvector3.h"

#include <QVector3D>

#include <fugio/context_interface.h>
#include <fugio/math/uuid.h>

SplitVector3Node::SplitVector3Node( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_VECTOR3, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_X, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_OUTPUT_Y, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_OUTPUT_Z, "249f2932-f483-422f-b811-ab679f006381" );

	mPinInput = pinInput( "Vector3", PIN_INPUT_VECTOR3 );

	mPinInput->registerPinInputType( PID_VECTOR3 );

	mOutX = pinOutput<fugio::VariantInterface *>( "X", mPinOutX, PID_FLOAT, PIN_OUTPUT_X );

	mOutY = pinOutput<fugio::VariantInterface *>( "Y", mPinOutY, PID_FLOAT, PIN_OUTPUT_Y );

	mOutZ = pinOutput<fugio::VariantInterface *>( "Z", mPinOutZ, PID_FLOAT, PIN_OUTPUT_Z );
}

void SplitVector3Node::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	QVector3D				 V3 = variant( mPinInput ).value<QVector3D>();

	if( mOutX->variant().toFloat() != V3.x() )
	{
		mOutX->setVariant( V3.x() );

		mNode->context()->pinUpdated( mPinOutX );
	}

	if( mOutY->variant().toFloat() != V3.y() )
	{
		mOutY->setVariant( V3.y() );

		mNode->context()->pinUpdated( mPinOutY );
	}

	if( mOutZ->variant().toFloat() != V3.z() )
	{
		mOutZ->setVariant( V3.z() );

		mNode->context()->pinUpdated( mPinOutZ );
	}
}
