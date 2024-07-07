#include "imagenode.h"

#include <QUrl>
#include <QImageReader>

#include <fugio/image/uuid.h>
#include <fugio/core/uuid.h>

ImageNode::ImageNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_SIZE, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_IMAGE, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );

	mSize = pinInput( "Size", PIN_INPUT_SIZE );

	mSize->registerPinInputType( PID_SIZE );

	pinOutput<fugio::VariantInterface>( "Image", mImage, PID_IMAGE, PIN_OUTPUT_IMAGE );
}

QWidget *ImageNode::gui( void )
{
	return( 0 );
}

void ImageNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	fugio::Image	DstImg = mImage.mEnt->variant().value<fugio::Image>();

	QSize			ImgSze = variant<QSize>( mSize );

	if( ImgSze != DstImg.size() )
	{
		DstImg.setFormat( fugio::ImageFormat::BGRA8 );
		DstImg.setLineSize( 0, 4 * ImgSze.width() );
		DstImg.setSize( ImgSze.width(), ImgSze.height() );

		pinUpdated( mImage.mPin );
	}
}
