#include "viewportnode.h"

#include <QSurface>

#include <fugio/core/uuid.h>
#include <fugio/opengl/uuid.h>

#include <fugio/context_interface.h>
#include <fugio/opengl/state_interface.h>

#include "openglplugin.h"

ViewportNode::ViewportNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode ), mUpdateViewport( true )
{
	FUGID( PIN_INPUT_X, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_Y, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_WIDTH, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_INPUT_HEIGHT, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_OUTPUT_VIEWPORT, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );

	mPinX = pinInput( "X", PIN_INPUT_X );
	mPinY = pinInput( "Y", PIN_INPUT_Y );
	mPinW = pinInput( "Width", PIN_INPUT_WIDTH );
	mPinH = pinInput( "Height", PIN_INPUT_HEIGHT );

	mPinX->setValue( 0.0 );
	mPinY->setValue( 0.0 );
	mPinW->setValue( 1.0 );
	mPinH->setValue( 1.0 );

	mValOutput = pinOutput<fugio::RenderInterface *>( "Render", mPinOutput, PID_RENDER, PIN_OUTPUT_VIEWPORT );

	mPinX->setDescription( tr( "The X position of the Viewport, either as a ratio of the current viewport (0.0 to 1.0), or in pixels" ) );
	mPinY->setDescription( tr( "The Y position of the Viewport, either as a ratio of the current viewport (0.0 to 1.0), or in pixels" ) );
	mPinW->setDescription( tr( "The width of the Viewport, either as a ratio of the current viewport (0.0 to 1.0), or in pixels" ) );
	mPinH->setDescription( tr( "The height of the Viewport, either as a ratio of the current viewport (0.0 to 1.0), or in pixels" ) );

	mPinOutput->setDescription( tr( "The output geometry that can be connected to an OpenGL Window" ) );
}

void ViewportNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	mUpdateViewport = true;

	pinUpdated( mPinOutput );
}


QList<QUuid> ViewportNode::pinAddTypesInput() const
{
	QList<QUuid>		PinIds;

	PinIds << PID_OPENGL_STATE;
	PinIds << PID_RENDER;

	return( PinIds );
}

bool ViewportNode::canAcceptPin( fugio::PinInterface *pPin ) const
{
	return( pPin->direction() == PIN_OUTPUT );
}

void ViewportNode::render( qint64 pTimeStamp, QUuid pSourcePinId )
{
	Q_UNUSED( pTimeStamp )
	Q_UNUSED( pSourcePinId )

	if( !OpenGLPlugin::hasContextStatic() )
	{
		return;
	}

#if 0
	GLint		CurVP[ 4 ];

	glGetIntegerv( GL_VIEWPORT, CurVP );

	QSurface		*Surface = QOpenGLContext::currentContext()->surface();

	if( !Surface )
	{
		return;
	}

	if( mUpdateViewport )
	{
		mVP.setX( variant( mPinX ).toReal() );
		mVP.setY( variant( mPinY ).toReal() );
		mVP.setWidth( variant( mPinW ).toReal() );
		mVP.setHeight( variant( mPinH ).toReal() );

		QSizeF			 SurfaceSize = Surface->size();

		if( mVP.x() <= 1.0 )
		{
			mVP.setX( mVP.x() * SurfaceSize.width() );
		}

		if( mVP.y() <= 1.0 )
		{
			mVP.setY( mVP.y() * SurfaceSize.height() );
		}

		if( mVP.width() <= 1.0 )
		{
			mVP.setWidth( mVP.width() * SurfaceSize.width() );
		}

		if( mVP.height() <= 1.0 )
		{
			mVP.setHeight( mVP.height() * SurfaceSize.height() );
		}

		mUpdateViewport = false;
	}

	glViewport( mVP.x(), mVP.y(), mVP.width(), mVP.height() );

	int		PinIdx = 0;

	fugio::OpenGLStateInterface		*CurrentState = 0;

	foreach( QSharedPointer<fugio::PinInterface> P, mNode->enumInputPins() )
	{
		Q_ASSERT( PinIdx == P->order() );

		PinIdx++;

		if( !P->isConnected() || P->connectedPin().isNull() || !P->connectedNode()->isActive() || P->connectedPin()->control().isNull() )
		{
			continue;
		}

		QObject					*O = P->connectedPin()->control()->object();

		if( O == 0 )
		{
			continue;
		}

		if( true )
		{
			fugio::RenderInterface		*Geometry = qobject_cast<fugio::RenderInterface *>( O );

			if( Geometry != 0 )
			{
				Geometry->drawGeometry();

				continue;
			}
		}

		if( true )
		{
			fugio::OpenGLStateInterface		*NextState = qobject_cast<fugio::OpenGLStateInterface *>( O );

			if( NextState != 0 )
			{
				if( CurrentState != 0 )
				{
					CurrentState->stateEnd();
				}

				CurrentState = NextState;

				CurrentState->stateBegin();

				continue;
			}
		}
	}

	if( CurrentState != 0 )
	{
		CurrentState->stateEnd();
	}

	glViewport( CurVP[ 0 ], CurVP[ 1 ], CurVP[ 2 ], CurVP[ 3 ] );
#endif
}
