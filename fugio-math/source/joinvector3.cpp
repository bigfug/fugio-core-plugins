#include "joinvector3.h"

#include <fugio/core/uuid.h>
#include <fugio/math/uuid.h>

#include <QVector3D>

JoinVector3Node::JoinVector3Node( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_X, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_Y, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_Z, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_OUTPUT_VECTOR3, "249f2932-f483-422f-b811-ab679f006381" );

	mPinX = pinInput( "X", PIN_INPUT_X );
	mPinY = pinInput( "Y", PIN_INPUT_Y );
	mPinZ = pinInput( "Z", PIN_INPUT_Z );

	mVector3 = pinOutput<fugio::VariantInterface *>( "Vector3", mPinVector3, PID_VECTOR3, PIN_OUTPUT_VECTOR3 );
}

void JoinVector3Node::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	qreal			NewX = variant( mPinX ).toReal();
	qreal			NewY = variant( mPinY ).toReal();
	qreal			NewZ = variant( mPinZ ).toReal();

	QVector3D		CurVec = mVector3->variant().value<QVector3D>();
	QVector3D		NewVec = QVector3D( NewX, NewY, NewZ );

	if( CurVec != NewVec )
	{
		mVector3->setVariant( NewVec );

		pinUpdated( mPinVector3 );
	}
}
