#include "matrixperspectivenode.h"

#include <QMatrix4x4>

#include <fugio/core/uuid.h>
#include <fugio/math/uuid.h>
#include <fugio/context_interface.h>

MatrixPerspectiveNode::MatrixPerspectiveNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_OUTPUT_MATRIX, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_VERTICAL_ANGLE, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_ASPECT_RATIO, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_INPUT_NEAR_PLANE, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_INPUT_FAR_PLANE, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );

	mOutputMatrix = pinOutput<fugio::VariantInterface *>( "Matrix", mPinOutputMatrix, PID_MATRIX4, PIN_OUTPUT_MATRIX );

	mPinAngle       = pinInput( tr( "Vertical Angle" ), PIN_INPUT_VERTICAL_ANGLE );
	mPinAspectRatio = pinInput( tr( "Aspect Ratio" ), PIN_INPUT_ASPECT_RATIO );
	mPinNearPlane   = pinInput( tr( "Near Plane" ), PIN_INPUT_NEAR_PLANE );
	mPinFarPlane    = pinInput( tr( "Far Plane" ), PIN_INPUT_FAR_PLANE );

	mPinAngle->registerPinInputType( PID_FLOAT );

	mPinAngle->setValue( 45.0 );
	mPinAspectRatio->setValue( 4.0 / 3.0 );
	mPinNearPlane->setValue( 0.1 );
	mPinFarPlane->setValue( 100.0 );
}

void MatrixPerspectiveNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	QMatrix4x4		MatPrj;

	qreal			Angle       = variant( mPinAngle ).toReal();
	qreal			AspectRatio = variant( mPinAspectRatio ).toReal();
	qreal			NearPlane   = variant( mPinNearPlane ).toReal();
	qreal			FarPlane    = variant( mPinFarPlane ).toReal();

	MatPrj.perspective( Angle, AspectRatio, NearPlane, FarPlane );

	mOutputMatrix->setVariant( MatPrj );

	pinUpdated( mPinOutputMatrix );
}
