#include "colourmasknode.h"

#include <fugio/core/uuid.h>
#include <fugio/image/uuid.h>

#include <fugio/image/image.h>
#include <fugio/colour/colour_interface.h>

#include <fugio/performance.h>

ColourMaskNode::ColourMaskNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_IMAGE, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_COLOUR, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_HUE_MATCH, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_OUTPUT_IMAGE, "249f2932-f483-422f-b811-ab679f006381" );

	mPinInputImage = pinInput( "Image", PIN_INPUT_IMAGE );
	mPinColour     = pinInput( "Colour", PIN_INPUT_COLOUR );
	mPinHueMatch   = pinInput( "Hue Match", PIN_INPUT_HUE_MATCH );

	mPinInputImage->registerPinInputType( PID_IMAGE );

	mPinColour->registerPinInputType( PID_COLOUR );

	mPinHueMatch->registerPinInputType( PID_FLOAT );

	mValOutputImage = pinOutput<fugio::VariantInterface *>( "Image", mPinOutputImage, PID_IMAGE, PIN_OUTPUT_IMAGE );
}

void ColourMaskNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	fugio::Image	SrcImg = variant<fugio::Image>( mPinInputImage );

	if( !SrcImg.isValid() )
	{
		mNode->setStatus( fugio::NodeInterface::Warning );
		mNode->setStatusMessage( "Image is not valid" );

		return;
	}

	if( SrcImg.format() != fugio::ImageFormat::HSV8 )
	{
		mNode->setStatus( fugio::NodeInterface::Warning );
		mNode->setStatusMessage( "Image must be HSV8" );

		return;
	}

	if( mNode->status() != fugio::NodeInterface::Initialised )
	{
		mNode->setStatus( fugio::NodeInterface::Initialised );
		mNode->setStatusMessage( QString() );
	}

	fugio::Performance	Perf( mNode, "inputsUpdated", pTimeStamp );

	QColor		SrcCol = variant( mPinColour ).value<QColor>();

	qint16		HVal   = SrcCol.hsvHueF() * 180.0f;
	qint16		HVar   = variant( mPinHueMatch ).toFloat() * 90.0f;

	fugio::Image	DstImg = mValOutputImage->variant().value<fugio::Image>();

	if( DstImg.size() != SrcImg.size() )
	{
		DstImg.setFormat( fugio::ImageFormat::GRAY8 );
		DstImg.setSize( SrcImg.width(), SrcImg.height() );
		DstImg.setLineSize( 0, SrcImg.width() );
	}

	for( int y = 0 ; y < SrcImg.height() ; y++ )
	{
		const quint8		*SrcPtr = SrcImg.buffer( 0 ) + SrcImg.lineSize( 0 ) * y;
		quint8				*DstPtr = DstImg.internalBuffer( 0 ) + DstImg.lineSize( 0 ) * y;

		for( int x = 0 ; x < SrcImg.width() ; x++ )
		{
			int		 Idx = x * 3;

			qint16	 HSrc = qint16( SrcPtr[ Idx + 0 ] );

			if( qAbs( HSrc - HVal ) <= HVar )
			{
				DstPtr[ x ] = 255;
			}
			else
			{
				DstPtr[ x ] = 0;
			}
		}
	}

	pinUpdated( mPinOutputImage );
}
