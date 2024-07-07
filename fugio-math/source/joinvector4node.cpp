#include "joinvector4node.h"

#include <fugio/core/uuid.h>
#include <fugio/math/uuid.h>

#include <QVector4D>

JoinVector4Node::JoinVector4Node( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_X, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_Y, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_Z, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_INPUT_W, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_OUTPUT_VECTOR4, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );

	mPinX = pinInput( "X", PIN_INPUT_X );
	mPinY = pinInput( "Y", PIN_INPUT_Y );
	mPinZ = pinInput( "Z", PIN_INPUT_Z );
	mPinW = pinInput( "W", PIN_INPUT_W );

	mVector4 = pinOutput<fugio::VariantInterface *>( "Vector4", mPinVector4, PID_VECTOR4, PIN_OUTPUT_VECTOR4 );
}

void JoinVector4Node::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	qreal			NewX = variant( mPinX ).toReal();
	qreal			NewY = variant( mPinY ).toReal();
	qreal			NewZ = variant( mPinZ ).toReal();
	qreal			NewW = variant( mPinW ).toReal();

	QVector4D		CurVec = mVector4->variant().value<QVector4D>();
	QVector4D		NewVec = QVector4D( NewX, NewY, NewZ, NewW );

	if( CurVec != NewVec )
	{
		mVector4->setVariant( NewVec );

		pinUpdated( mPinVector4 );
	}
}
