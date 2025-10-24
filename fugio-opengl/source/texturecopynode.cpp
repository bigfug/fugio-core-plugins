#include "texturecopynode.h"

#include <fugio/core/uuid.h>
#include <fugio/context_interface.h>
#include <fugio/performance.h>

#include <QSurfaceFormat>

#if QT_VERSION >= QT_VERSION_CHECK( 5, 6, 0 )
#include <QOpenGLExtraFunctions>
#endif

#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLFunctions_4_3_Core>

#include "openglplugin.h"

TextureCopyNode::TextureCopyNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_TEXTURE, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_TEXTURE, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );

	mPinTexSrc = pinInput( "Texture", PIN_INPUT_TEXTURE );

	mTexDst = pinOutput<fugio::OpenGLTextureInterface *>( "Texture", mPinTexDst, PID_OPENGL_TEXTURE, PIN_OUTPUT_TEXTURE );

	mPinTexSrc->setDescription( tr( "The source OpenGL Texture" ) );

	mPinTexDst->setDescription( tr( "The destination OpenGL Texture to copy the source texture into" ) );
}

TextureCopyNode::~TextureCopyNode()
{
}

void TextureCopyNode::inputsUpdated( qint64 pTimeStamp )
{
	fugio::Performance	Perf( mNode, "inputsUpdated", pTimeStamp );

	NodeControlBase::inputsUpdated( pTimeStamp );

	if( !OpenGLPlugin::hasContextStatic() )
	{
		return;
	}

	initializeOpenGLFunctions();

	fugio::OpenGLTextureInterface	*TexSrc = input<fugio::OpenGLTextureInterface *>( mPinTexSrc );
	fugio::OpenGLTextureInterface	*TexDst = mTexDst;

	if( !TexSrc || !TexDst )
	{
		return;
	}

	fugio::OpenGLTextureDescription		SrcDsc = TexSrc->textureDescription();
	fugio::OpenGLTextureDescription		DstDsc = TexDst->textureDescription();

	if( memcmp( &SrcDsc, &DstDsc, sizeof( fugio::OpenGLTextureDescription ) ) != 0 )
	{
		TexDst->setTextureDescription( SrcDsc );

		TexDst->update();
	}

	if( TexDst->dstTexId() )
	{
		const QVector3D		TexSze = TexSrc->size();

#if !defined( QT_OPENGL_ES_2 )
		auto GL43 = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_3_Core>( QOpenGLContext::currentContext() );

		if( GL43 && !GL43->initializeOpenGLFunctions() )
		{
			GL43 = Q_NULLPTR;
		}

		if( GL43 )
		{
			GL43->glCopyImageSubData( TexSrc->srcTexId(), TexSrc->target(), 0, 0, 0, 0,
									  TexDst->dstTexId(), TexDst->target(), 0, 0, 0, 0,
									  TexSze.x(), TexSze.y(), qMax<GLsizei>( TexSze.z(), 1 ) );
		}
		else
#endif
		{
#if defined( QOPENGLEXTRAFUNCTIONS_H )
			QOpenGLExtraFunctions	*GLEX = QOpenGLContext::currentContext()->extraFunctions();

			if( GLEX )
			{
				GLuint	FBO = mFBO.fbo();

				if( FBO )
				{
					glBindFramebuffer( GL_FRAMEBUFFER, FBO );

					glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TexSrc->target(), TexSrc->srcTexId(), 0 );

					glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, TexDst->target(), TexDst->dstTexId(), 0 );

					GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT1 };

					GLEX->glDrawBuffers( 1, drawBuffers );

					GLEX->glBlitFramebuffer( 0, 0, TexSze.x(), TexSze.y(), 0, 0, TexSze.x(), TexSze.y(), GL_COLOR_BUFFER_BIT, GL_NEAREST );

					glBindFramebuffer( GL_FRAMEBUFFER, 0 );
				}
			}
#endif
		}

		pinUpdated( mPinTexDst );
	}
}

