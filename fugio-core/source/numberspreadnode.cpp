#include "numberspreadnode.h"

#include <fugio/core/uuid.h>

#include <fugio/context_interface.h>

#include <qmath.h>

NumberSpreadNode::NumberSpreadNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_NUMBER, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_CENTER, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_SPREAD, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_OUTPUT_NUMBER, "249f2932-f483-422f-b811-ab679f006381" );

	mPinValue  = pinInput( "Number", PIN_INPUT_NUMBER );
	mPinCenter = pinInput( "Center", PIN_INPUT_CENTER );
	mPinSpread = pinInput( "Spread", PIN_INPUT_SPREAD );

	mPinValue->setValue( 1.0f );
	mPinSpread->setValue( 1.0f );

	QSharedPointer<fugio::PinInterface>		P;

	pinOutput<fugio::PinControlInterface *>( "Number", P, PID_FLOAT, PIN_OUTPUT_NUMBER );
}

void NumberSpreadNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	const float		Spread = variant( mPinSpread ).toFloat();
	const float		Center = variant( mPinCenter ).toFloat();
	const float		Value  = variant( mPinValue  ).toFloat();

	QList<QSharedPointer<fugio::PinInterface>>		PinLst = mNode->enumOutputPins();

	for( int i = 0 ; i < PinLst.size() ; i++ )
	{
		QSharedPointer<fugio::PinInterface>		P = PinLst[ i ];

		if( !P->hasControl() )
		{
			continue;
		}

		float		PinBeg = float( i + 0 ) / float( PinLst.size() - 1 );
//		float		PinEnd = float( i + 1 ) / float( PinLst.size() - 1 );
		float		PinVal = 0.0f;

//		if( PinBeg <= Center && PinEnd >= Center )
//		{
//			PinVal = Value;
//		}
//		else
		{
			float		PinPos = ( Center - PinBeg ) * ( M_PI * Spread );

			if( PinPos >= -M_PI && PinPos <= M_PI )
			{
				PinVal = qCos( PinPos ) * Value;
			}
		}

		if( fugio::VariantInterface *I = qobject_cast<fugio::VariantInterface *>( P->control()->qobject() ) )
		{
			I->setVariant( PinVal );

			pinUpdated( P );
		}
	}
}

QList<QUuid> NumberSpreadNode::pinAddTypesOutput() const
{
	QList<QUuid>		PinLst;

	PinLst << PID_FLOAT;

	return( PinLst );
}

bool NumberSpreadNode::canAcceptPin( fugio::PinInterface *pPin ) const
{
	return( pPin->direction() == PIN_INPUT );
}
