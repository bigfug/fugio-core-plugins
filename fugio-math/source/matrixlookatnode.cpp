#include "matrixlookatnode.h"

#include <QMatrix4x4>

#include <fugio/core/uuid.h>
#include <fugio/math/uuid.h>

MatrixLookAtNode::MatrixLookAtNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_OUTPUT_MATRIX, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_MATRIX, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_POSITION, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_INPUT_CENTER, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_INPUT_UP, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );

	mOutputMatrix = pinOutput<fugio::VariantInterface *>( "Matrix", mPinOutputMatrix, PID_MATRIX4, PIN_OUTPUT_MATRIX );

	mPinInputMatrix = pinInput( tr( "Matrix" ), PIN_INPUT_MATRIX);

	mPinInputMatrix->setValue( QMatrix4x4() );

	mPinInputPosition = pinInput( tr( "Position" ), PIN_INPUT_POSITION );

	mPinInputPosition->setValue( QVector3D( 0, 0, 5 ) );

	mPinInputCenter = pinInput( tr( "Center" ), PIN_INPUT_CENTER );

	mPinInputCenter->setValue( QVector3D( 0, 0, 0 ) );

	mPinInputUp = pinInput( tr( "Up" ), PIN_INPUT_UP );

	mPinInputUp->setValue( QVector3D( 0, 1, 0 ) );

	mPinInputMatrix->registerPinInputType( PID_MATRIX4 );
	mPinInputPosition->registerPinInputType( PID_VECTOR3 );
	mPinInputCenter->registerPinInputType( PID_VECTOR3 );
	mPinInputUp->registerPinInputType( PID_VECTOR3 );
}

void MatrixLookAtNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	QVector3D		VecPos;
	QVector3D		VecCenter;
	QVector3D		VecUp;

	VecPos    = variant( mPinInputPosition ).value<QVector3D>();
	VecCenter = variant( mPinInputCenter ).value<QVector3D>();
	VecUp     = variant( mPinInputUp ).value<QVector3D>();

	QMatrix4x4		MatCam;

	MatCam = variant( mPinInputMatrix ).value<QMatrix4x4>();

	MatCam.lookAt( VecPos, VecCenter, VecUp );

	if( MatCam == mOutputMatrix->variant().value<QMatrix4x4>() )
	{
		return;
	}

	mOutputMatrix->setVariant( MatCam );

	pinUpdated( mPinOutputMatrix );
}
