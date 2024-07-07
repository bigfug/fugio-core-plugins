#include "matrixmultiplynode.h"

#include <QMatrix4x4>
#include <QVector4D>

#include <fugio/core/uuid.h>
#include <fugio/math/uuid.h>
#include <fugio/context_interface.h>

MatrixMultiplyNode::MatrixMultiplyNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_MATRIX, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_VECTOR, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_OUTPUT_MATRIX, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );

	mPinMatrix = pinInput( "Matrix", PIN_INPUT_MATRIX );
	mPinVector = pinInput( "Vector", PIN_INPUT_VECTOR );

	mPinMatrix->registerPinInputType( PID_MATRIX4 );
	mPinVector->registerPinInputType( PID_VECTOR3 );
	mPinVector->registerPinInputType( PID_VECTOR4 );

	mOutput = pinOutput<fugio::VariantInterface *>( "Matrix", mPinOutput, PID_VECTOR3, PIN_OUTPUT_MATRIX );
}

void MatrixMultiplyNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	QMatrix4x4		Mat = variant( mPinMatrix ).value<QMatrix4x4>();
	QVector4D		Vec;

	QVariant		Src = variant( mPinVector );

	switch( QMetaType::Type( Src.type() ) )
	{
		case QMetaType::QVector3D:
			Vec = QVector4D( Src.value<QVector3D>(), 1 );
			break;

		case QMetaType::QVector4D:
			Vec = Src.value<QVector4D>();
			break;

		default:
			break;
	}

	QVector4D		Out = Mat * Vec;

	mOutput->setVariant( QVector3D( Out ) );

	pinUpdated( mPinOutput );
}
