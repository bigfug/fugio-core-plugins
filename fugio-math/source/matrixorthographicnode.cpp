#include "matrixorthographicnode.h"

#include <QMatrix4x4>

#include <fugio/core/uuid.h>
#include <fugio/math/uuid.h>
#include <fugio/context_interface.h>

MatrixOrthographicNode::MatrixOrthographicNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_OUTPUT_MATRIX, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_LEFT, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_RIGHT, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_INPUT_BOTTOM, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_INPUT_TOP, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );
	FUGID( PIN_INPUT_NEAR, "e6bf944e-5f46-4994-bd51-13c2aa6415b7" );
	FUGID( PIN_INPUT_FAR, "a2bbf374-0dc8-42cb-b85a-6a43b58a348f" );

	mOutputMatrix = pinOutput<fugio::VariantInterface *>( "Matrix", mPinOutputMatrix, PID_MATRIX4, PIN_OUTPUT_MATRIX );

	mPinInputLeft   = pinInput( tr( "Left" ), PIN_INPUT_LEFT );
	mPinInputRight  = pinInput( tr( "Right" ), PIN_INPUT_RIGHT );
	mPinInputBottom = pinInput( tr( "Bottom" ), PIN_INPUT_BOTTOM );
	mPinInputTop    = pinInput( tr( "Top" ), PIN_INPUT_TOP );
	mPinInputNear   = pinInput( tr( "Near" ), PIN_INPUT_NEAR );
	mPinInputFar    = pinInput( tr( "Far" ), PIN_INPUT_FAR );

	mPinInputLeft->registerPinInputType( PID_FLOAT );
	mPinInputRight->registerPinInputType( PID_FLOAT );
	mPinInputBottom->registerPinInputType( PID_FLOAT );
	mPinInputTop->registerPinInputType( PID_FLOAT );
	mPinInputNear->registerPinInputType( PID_FLOAT );
	mPinInputFar->registerPinInputType( PID_FLOAT );

	mPinInputLeft->setValue( -10.0 );
	mPinInputRight->setValue( 10.0 );
	mPinInputBottom->setValue( -10.0 );
	mPinInputTop->setValue( 10.0 );
	mPinInputNear->setValue( 0.1 );
	mPinInputFar->setValue( 100.0 );
}

void MatrixOrthographicNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	QMatrix4x4		MatPrj;

	qreal			Left   = variant( mPinInputLeft ).toReal();
	qreal			Right  = variant( mPinInputRight ).toReal();
	qreal			Bottom = variant( mPinInputBottom ).toReal();
	qreal			Top    = variant( mPinInputTop ).toReal();
	qreal			Near   = variant( mPinInputNear ).toReal();
	qreal			Far    = variant( mPinInputFar ).toReal();

	MatPrj.ortho( Left, Right, Bottom, Top, Near, Far );

	mOutputMatrix->setVariant( MatPrj );

	pinUpdated( mPinOutputMatrix );
}
