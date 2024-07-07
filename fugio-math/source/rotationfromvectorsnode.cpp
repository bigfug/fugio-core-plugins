#include "rotationfromvectorsnode.h"

#include <fugio/core/uuid.h>
#include <fugio/math/uuid.h>

#include <fugio/math/matrix_interface.h>

#include <QMatrix3x3>
#include <QMatrix4x4>

RotationFromVectorsNode::RotationFromVectorsNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_VECTOR1, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_VECTOR2, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_OUTPUT_MATRIX, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );

	mPinInput1 = pinInput( "Vector3", PIN_INPUT_VECTOR1 );
	mPinInput2 = pinInput( "Vector3", PIN_INPUT_VECTOR2 );

	mValOutput = pinOutput<fugio::VariantInterface *>( "Matrix", mPinOutput, PID_MATRIX4, PIN_OUTPUT_MATRIX );
}

void RotationFromVectorsNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	QVector3D		V1 = variant( mPinInput1 ).value<QVector3D>();
	QVector3D		V2 = variant( mPinInput2 ).value<QVector3D>();

	if( V1.isNull() || V2.isNull() )
	{
		return;
	}

	QMatrix4x4		M4;

	if( V1 != V2 )
	{
		QVector3D	MX = V1.normalized();
		QVector3D	MZ = QVector3D::crossProduct( V1, V2 ).normalized();
		QVector3D	MY = QVector3D::crossProduct( MZ, V1 ).normalized();

		M4.setRow( 0, MX.toVector4D() );
		M4.setRow( 1, MY.toVector4D() );
		M4.setRow( 2, MZ.toVector4D() );
	}

	if( M4 != mValOutput->variant().value<QMatrix4x4>() )
	{
		mValOutput->setVariant( M4 );

		pinUpdated( mPinOutput );
	}
}
