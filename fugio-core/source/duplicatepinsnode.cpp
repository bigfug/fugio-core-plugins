#include "duplicatepinsnode.h"

#include <fugio/core/uuid.h>

#include <fugio/context_interface.h>
#include <fugio/context_signals.h>
#include <fugio/node_signals.h>
#include <fugio/pin_signals.h>

DuplicatePinsNode::DuplicatePinsNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_INPUT, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_OUTPUT, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );

	mPinInput = pinInput( "Input", PIN_INPUT_INPUT );

	mPinOutput = pinOutput( "Output", PIN_OUTPUT_OUTPUT );
}

bool DuplicatePinsNode::initialise()
{
	if( !NodeControlBase::initialise() )
	{
		return( false );
	}

	if( mPinInput->isConnected() && mPinInput->connectedPin()->hasControl() )
	{
		inputLinked( mPinInput->connectedPin() );
	}
	else
	{
		inputUnlinked( mPinInput->connectedPin() );
	}

	connect( mPinInput->qobject(), SIGNAL(linked(QSharedPointer<fugio::PinInterface>)), this, SLOT(inputLinked(QSharedPointer<fugio::PinInterface>)) );
	connect( mPinInput->qobject(), SIGNAL(unlinked(QSharedPointer<fugio::PinInterface>)), this, SLOT(inputUnlinked(QSharedPointer<fugio::PinInterface>)) );

	connect( mNode->qobject(), SIGNAL(pinAdded(QSharedPointer<fugio::NodeInterface>,QSharedPointer<fugio::PinInterface>)), this, SLOT(pinAdded(QSharedPointer<fugio::NodeInterface>,QSharedPointer<fugio::PinInterface>)) );

	return( true );
}

bool DuplicatePinsNode::deinitialise()
{
	return( NodeControlBase::deinitialise() );
}

void DuplicatePinsNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	for( QSharedPointer<fugio::PinInterface> P : mNode->enumOutputPins() )
	{
		pinUpdated( P );
	}
}

void DuplicatePinsNode::inputLinked( QSharedPointer<fugio::PinInterface> pNewPin )
{
	for( QSharedPointer<fugio::PinInterface> P : mNode->enumOutputPins() )
	{
		P->setControl( pNewPin->control() );
	}
}

void DuplicatePinsNode::inputUnlinked(QSharedPointer<fugio::PinInterface> pOldPin)
{
	Q_UNUSED( pOldPin )

	for( QSharedPointer<fugio::PinInterface> P : mNode->enumOutputPins() )
	{
		P->setControl( QSharedPointer<fugio::PinControlInterface>() );
	}
}

bool DuplicatePinsNode::canAcceptPin( fugio::PinInterface *pPin ) const
{
	return( pPin->direction() == PIN_INPUT );
}

void DuplicatePinsNode::pinAdded( QSharedPointer<fugio::NodeInterface> pNode, QSharedPointer<fugio::PinInterface> pPin )
{
	Q_UNUSED( pNode )

	if( pPin->direction() != PIN_OUTPUT )
	{
		return;
	}

	if( mPinInput->isConnected() && mPinInput->connectedPin()->hasControl() )
	{
		pPin->setControl( mPinInput->connectedPin()->control() );
	}
	else
	{
		pPin->setControl( QSharedPointer<fugio::PinControlInterface>() );
	}
}
