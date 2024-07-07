#include "lastupdatedinputnode.h"

#include <fugio/global_interface.h>
#include <fugio/context_interface.h>
#include <fugio/core/uuid.h>

#include <QDoubleSpinBox>
#include <QDateTime>
#include <QSettings>
#include <QFormLayout>

LastUpdatedInputNode::LastUpdatedInputNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_1, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_OUTPUT, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );

	mPinInput = pinInput( "Input 1", PIN_INPUT_1 );

	mPinOutput = pinOutput( "Output", PIN_OUTPUT_OUTPUT );
}

void LastUpdatedInputNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	QSharedPointer<fugio::PinControlInterface>		InpPin;
	qint64											InpTim = 0;

	for( QSharedPointer<fugio::PinInterface> P : mNode->enumInputPins() )
	{
		if( P->updated() > InpTim && P->isConnected() && P->connectedPin()->hasControl()  )
		{
			InpPin = P->connectedPin()->control();

			InpTim = P->updated();
		}
	}

	if( InpPin )
	{
		mPinOutput->setControl( InpPin );

		pinUpdated( mPinOutput );
	}
}


QList<QUuid> LastUpdatedInputNode::pinAddTypesInput() const
{
	return( mNode->context()->global()->pinIds().keys() );
}

bool LastUpdatedInputNode::canAcceptPin(fugio::PinInterface *pPin) const
{
	return( pPin->direction() == PIN_OUTPUT );
}
