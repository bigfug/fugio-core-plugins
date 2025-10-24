#include "texturecubenode.h"

#include <fugio/core/uuid.h>
#include <fugio/context_interface.h>
#include <fugio/image/image.h>

#include <QVector>

#include "openglplugin.h"

TextureCubeNode::TextureCubeNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_TOP, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_BOTTOM, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_LEFT, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_INPUT_RIGHT, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_INPUT_FRONT, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );
	FUGID( PIN_INPUT_BACK, "e6bf944e-5f46-4994-bd51-13c2aa6415b7" );
	FUGID( PIN_OUTPUT_TEXTURE, "a2bbf374-0dc8-42cb-b85a-6a43b58a348f" );

	mPinImgTop    = pinInput( "Top (+Y)", PIN_INPUT_TOP );
	mPinImgBottom = pinInput( "Bottom (-Y)", PIN_INPUT_BOTTOM );
	mPinImgLeft   = pinInput( "Left (-X)", PIN_INPUT_LEFT );
	mPinImgRight  = pinInput( "Right (+X)", PIN_INPUT_RIGHT );
	mPinImgFront  = pinInput( "Front (+Z)", PIN_INPUT_FRONT );
	mPinImgBack   = pinInput( "Back (-Z)", PIN_INPUT_BACK );

	mTexDst = pinOutput<OpenGLTextureInterface *>( "Texture", mPinTexDst, PID_OPENGL_TEXTURE, PIN_OUTPUT_TEXTURE );

	mTexDst->setTarget( QOpenGLTexture::TargetCubeMap );

	mPinTexDst->setDescription( tr( "The destination OpenGL Texture to copy the source texture into" ) );
}

void TextureCubeNode::processTexture( QSharedPointer<fugio::PinInterface> pPin, int pCubeMapIndex )
{
	fugio::Image			I = variant( pPin ).value<fugio::Image>();

	if( I.isEmpty() )
	{
		return;
	}

	QVector3D		TexSze = mTexDst->size();
	QVector3D		S      = QVector3D( I.size().width(), I.size().height(), 0 );

	if( TexSze != S )
	{
		mTexDst->free();

		mTexDst->setType( QOpenGLTexture::UInt8 );

		switch( I.format() )
		{
#if !defined( GL_ES_VERSION_2_0 )
			case fugio::ImageFormat::BGR8:
				mTexDst->setFormat( QOpenGLTexture::BGR );

				mTexDst->setInternalFormat( QOpenGLTexture::RGB8_UNorm );
				break;

			case fugio::ImageFormat::RGB8:
				mTexDst->setFormat( QOpenGLTexture::RGB );

				mTexDst->setInternalFormat( QOpenGLTexture::RGB8_UNorm );
				break;

			case fugio::ImageFormat::RGBA8:
				mTexDst->setFormat( QOpenGLTexture::RGBA );

				mTexDst->setInternalFormat( QOpenGLTexture::RGBA8_UNorm );
				break;

			case fugio::ImageFormat::BGRA8:
				mTexDst->setFormat( QOpenGLTexture::BGRA );

				mTexDst->setInternalFormat( QOpenGLTexture::RGBA8_UNorm );
				break;

			case fugio::ImageFormat::GRAY16:
				mTexDst->setType( QOpenGLTexture::UInt16 );

				if( QOpenGLContext::currentContext()->isOpenGLES() )
				{
					mTexDst->setFormat( QOpenGLTexture::Luminance );

					mTexDst->setInternalFormat( QOpenGLTexture::LuminanceFormat );
				}
				else
				{
					mTexDst->setFormat( QOpenGLTexture::Red );

					mTexDst->setInternalFormat( QOpenGLTexture::R16_UNorm );
				}
				break;

			case fugio::ImageFormat::GRAY8:
				if( QOpenGLContext::currentContext()->isOpenGLES() )
				{
					mTexDst->setFormat( QOpenGLTexture::Luminance );

					mTexDst->setInternalFormat( QOpenGLTexture::LuminanceFormat );
				}
				else
				{
					mTexDst->setFormat( QOpenGLTexture::Red );

					mTexDst->setInternalFormat( QOpenGLTexture::R8_UNorm );
				}
				break;

			case fugio::ImageFormat::YUYV422:
			case fugio::ImageFormat::UYVY422:
				mTexDst->setFormat( QOpenGLTexture::RG );

				mTexDst->setInternalFormat( QOpenGLTexture::RG8_UNorm );
				break;

			case fugio::ImageFormat::RG32:
				mTexDst->setFormat( QOpenGLTexture::RG );

				mTexDst->setType( QOpenGLTexture::Float32 );

				mTexDst->setInternalFormat( QOpenGLTexture::R32F );
				break;
#endif

			default:
				return;
		}

		mTexDst->setFilter( QOpenGLTexture::Linear, QOpenGLTexture::Linear );

		mTexDst->setWrap( QOpenGLTexture::ClampToEdge, QOpenGLTexture::ClampToEdge, QOpenGLTexture::ClampToEdge );

		mTexDst->setSize( S );
	}

	OPENGL_PLUGIN_DEBUG

	mTexDst->update( I.buffer( 0 ), I.bufferSize( 0 ), I.lineSize( 0 ), pCubeMapIndex );

	OPENGL_PLUGIN_DEBUG
}

void TextureCubeNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	if( !OpenGLPlugin::hasContextStatic() )
	{
		return;
	}

	OPENGL_PLUGIN_DEBUG

#define CUBE_MAP_POSITIVE_X    0
#define CUBE_MAP_NEGATIVE_X    1
#define CUBE_MAP_POSITIVE_Y    2
#define CUBE_MAP_NEGATIVE_Y    3
#define CUBE_MAP_POSITIVE_Z    4
#define CUBE_MAP_NEGATIVE_Z    5

	processTexture( mPinImgTop,    CUBE_MAP_POSITIVE_Y );
	processTexture( mPinImgBottom, CUBE_MAP_NEGATIVE_Y );
	processTexture( mPinImgLeft,   CUBE_MAP_NEGATIVE_X );
	processTexture( mPinImgRight,  CUBE_MAP_POSITIVE_X );
	processTexture( mPinImgFront,  CUBE_MAP_NEGATIVE_Z );
	processTexture( mPinImgBack,   CUBE_MAP_POSITIVE_Z );

	if( mTexDst->dstTexId() )
	{
		pinUpdated( mPinTexDst );
	}
	else
	{
		mTexDst->setSize( 0 );
	}
}


bool TextureCubeNode::initialise()
{
	if( !fugio::NodeControlBase::initialise() )
	{
		return( false );
	}

	if( !OpenGLPlugin::hasContextStatic() )
	{
		return( false );
	}

	return( true );
}
