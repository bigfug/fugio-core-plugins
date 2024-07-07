#include "randomnumbernode.h"

#include <fugio/global_interface.h>
#include <fugio/context_interface.h>
#include <fugio/core/uuid.h>

RandomNumberNode::RandomNumberNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode ), mDistribution( 0.0, 1.0 )
{
	FUGID( PIN_OUTPUT_RANDOM, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );

	mPinTrigger = pinInput( "Trigger", PID_FUGIO_NODE_TRIGGER );

	QSharedPointer<fugio::PinInterface>	PinTmp;

	pinOutput<fugio::VariantInterface *>( tr( "Random" ), PinTmp, PID_FLOAT, PIN_OUTPUT_RANDOM );
}

void RandomNumberNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	for( QSharedPointer<fugio::PinInterface> P : mNode->enumOutputPins() )
	{
		if( fugio::VariantInterface *V = qobject_cast<fugio::VariantInterface *>( P->control()->qobject() ) )
		{
			V->setVariant( mDistribution( mGenerator ) );

			pinUpdated( P );
		}
	}
}

QList<QUuid> RandomNumberNode::pinAddTypesOutput() const
{
	static QList<QUuid> PinLst;

	if( PinLst.isEmpty() )
	{
		PinLst << PID_FLOAT;
	}

	return( PinLst );
}

bool RandomNumberNode::canAcceptPin(fugio::PinInterface *pPin) const
{
	if( pPin->direction() != PIN_OUTPUT )
	{
		return( false );
	}

	return( true );
}
