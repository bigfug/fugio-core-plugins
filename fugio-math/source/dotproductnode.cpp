#include "dotproductnode.h"

#include <QVector3D>

#include <fugio/core/uuid.h>
#include <fugio/math/uuid.h>

#include <fugio/context_interface.h>

DotProductNode::DotProductNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_NUMBER1, "c13a41c6-544b-46bb-a9f2-19dd156d236c" );
	FUGID( PIN_INPUT_NUMBER2, "a145ebb2-9166-4a29-bccf-8ee184e27406" );
	FUGID( PIN_OUTPUT_NUMBER, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );

	mPinInput1 = pinInput( "Vector3", PIN_INPUT_NUMBER1 );
	mPinInput2 = pinInput( "Vector3", PIN_INPUT_NUMBER2 );

	mPinInput1->registerPinInputType( PID_VECTOR3 );
	mPinInput2->registerPinInputType( PID_VECTOR3 );

	mValOutput = pinOutput<fugio::VariantInterface *>( "Number", mPinOutput, PID_FLOAT, PIN_OUTPUT_NUMBER );
}

void DotProductNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	QVector3D	v1 = variant( mPinInput1 ).value<QVector3D>();
	QVector3D	v2 = variant( mPinInput2 ).value<QVector3D>();

	qreal		d = QVector3D::dotProduct( v1, v2 );

	if( d != mValOutput->variant().toReal() )
	{
		mValOutput->setVariant( d );

		pinUpdated( mPinOutput );
	}
}
