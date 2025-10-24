#include "instancebuffernode.h"

#include <fugio/opengl/buffer_interface.h>

InstanceBufferNode::InstanceBufferNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_BUFFER, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_BUFFER, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );

	mPinInput = pinInput( "Input", PIN_INPUT_BUFFER );
	mPinOutput = pinOutput( "Output", PIN_OUTPUT_BUFFER );
}

void InstanceBufferNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	if( !mPinInput->isConnected() || mPinOutput->control() != mPinInput->connectedPin()->control() )
	{
		if( mPinOutput->hasControl() )
		{
			fugio::OpenGLBufferInterface	*BufInt = qobject_cast<fugio::OpenGLBufferInterface *>( mPinOutput->control()->qobject() );

			if( BufInt )
			{
				BufInt->setInstanced( false );
			}

			mPinOutput->setControl( QSharedPointer<fugio::PinControlInterface>() );

			pinUpdated( mPinOutput );
		}
	}

	if( mPinInput->isConnected() && !mPinOutput->hasControl() )
	{
		QSharedPointer<fugio::PinControlInterface>		 BufCtl = mPinInput->connectedPin()->control();

		fugio::OpenGLBufferInterface					*BufInt = qobject_cast<fugio::OpenGLBufferInterface *>( BufCtl ? BufCtl->qobject() : nullptr );

		if( BufInt )
		{
			mPinOutput->setControl( BufCtl );

			BufInt->setInstanced( true );

			pinUpdated( mPinOutput );
		}
	}
}
