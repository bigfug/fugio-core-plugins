#include "matrixtranslatenode.h"

#include <fugio/context_interface.h>
#include <fugio/math/uuid.h>

#include <QMatrix4x4>

MatrixTranslateNode::MatrixTranslateNode( QSharedPointer<fugio::NodeInterface> pNode ) :
	NodeControlBase( pNode )
{
	FUGID( PIN_OUTPUT_MATRIX, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_MATRIX, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_X, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_INPUT_Y, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_INPUT_Z, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );

	mValue = pinOutput<fugio::VariantInterface *>( "Matrix", mPinValue, PID_MATRIX4, PIN_OUTPUT_MATRIX);

	mPinInputMatrix = pinInput( "Matrix", PIN_INPUT_MATRIX );

	mPinInputX = pinInput( "X", PIN_INPUT_X );
	mPinInputY = pinInput( "Y", PIN_INPUT_Y );
	mPinInputZ = pinInput( "Z", PIN_INPUT_Z );

	mPinInputX->setValue( 0.0 );
	mPinInputY->setValue( 0.0 );
	mPinInputZ->setValue( 0.0 );
}

void MatrixTranslateNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	qreal				 x = variant( mPinInputX ).toReal();
	qreal				 y = variant( mPinInputY ).toReal();
	qreal				 z = variant( mPinInputZ ).toReal();

	QMatrix4x4			 m = variant( mPinInputMatrix ).value<QMatrix4x4>();

	m.translate( x, y, z );

	if( m != mValue->variant().value<QMatrix4x4>() )
	{
		mValue->setVariant( m );

		pinUpdated( mPinValue );
	}
}
