#include "splitsizenode.h"

#include <fugio/core/uuid.h>
#include <fugio/context_interface.h>

#include <QSizeF>
#include <QRectF>

SplitSizeNode::SplitSizeNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_SIZE, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_WIDTH, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_OUTPUT_HEIGHT, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );

	mPinInput = pinInput( tr( "Size" ), PIN_INPUT_SIZE );

	mPinInput->registerPinInputType( PID_SIZE );

	mWidth  = pinOutput<fugio::VariantInterface *>( tr( "Width" ), mPinWidth, PID_FLOAT, PIN_OUTPUT_WIDTH );

	mHeight = pinOutput<fugio::VariantInterface *>( tr( "Height" ), mPinHeight, PID_FLOAT, PIN_OUTPUT_HEIGHT );
}

void SplitSizeNode::inputsUpdated( qint64 pTimeStamp )
{
	if( mPinInput->isUpdated( pTimeStamp ) )
	{
		fugio::VariantInterface		*V = input<fugio::VariantInterface *>( mPinInput );

		QSizeF						 S;

		if( V )
		{
			switch( QMetaType::Type( V->variant().type() ) )
			{
				case QMetaType::QSize:
					S = V->variant().toSize();
					break;

				case QMetaType::QSizeF:
					S = V->variant().toSizeF();
					break;

				case QMetaType::QRect:
					S = V->variant().toRect().size();
					break;

				case QMetaType::QRectF:
					S = V->variant().toRectF().size();
					break;

				default:
					break;
			}
		}

		if( mWidth->variant().toReal() != S.width() )
		{
			mWidth->setVariant( S.width() );

			pinUpdated( mPinWidth );
		}

		if( mHeight->variant().toReal() != S.height() )
		{
			mHeight->setVariant( S.height() );

			pinUpdated( mPinHeight );
		}
	}
}
