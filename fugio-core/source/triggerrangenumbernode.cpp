#include "triggerrangenumbernode.h"

#include <QDebug>

#include <fugio/core/uuid.h>

#include <fugio/context_interface.h>
#include <fugio/variant_interface.h>

TriggerRangeNumberNode::TriggerRangeNumberNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode ), mPinIdx( -1 )
{
	FUGID( PIN_OUTPUT_NUMBER, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );

	mValOutput = pinOutput<fugio::VariantInterface *>( "Number", mPinOutput, PID_FLOAT, PIN_OUTPUT_NUMBER );
}

void TriggerRangeNumberNode::inputsUpdated( qint64 pTimeStamp )
{
	int		PinCnt = mNode->enumInputPins().size();
	int		PinVal = mPinIdx;

	for( QSharedPointer<fugio::PinInterface> P : mNode->enumInputPins() )
	{
		if( P->isUpdated( pTimeStamp ) )
		{
			//qDebug() << P->order();

			PinVal = P->order();

			break;
		}
	}

	if( PinVal == mPinIdx )
	{
		return;
	}

	mValOutput->setVariant( double( PinVal ) / double( PinCnt - 1 ) );

	pinUpdated( mPinOutput );

	mPinIdx = PinVal;
}

QList<QUuid> TriggerRangeNumberNode::pinAddTypesInput() const
{
	return( mNode->context()->global()->pinIds().keys() );
}

bool TriggerRangeNumberNode::canAcceptPin( fugio::PinInterface *pPin ) const
{
	return( pPin->direction() == PIN_OUTPUT );
}
