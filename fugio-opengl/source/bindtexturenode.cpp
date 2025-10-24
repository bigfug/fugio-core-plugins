#include "bindtexturenode.h"

#include <fugio/core/uuid.h>
#include <fugio/opengl/uuid.h>

#include <fugio/context_interface.h>
#include <fugio/opengl/texture_interface.h>
#include <fugio/opengl/state_interface.h>

#include "openglplugin.h"

BindTextureNode::BindTextureNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_TEXTURE0, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_TEXTURE1, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_TEXTURE2, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );
	FUGID( PIN_INPUT_TEXTURE3, "249f2932-f483-422f-b811-ab679f006381" );
	FUGID( PIN_INPUT_STATE, "ce8d578e-c5a4-422f-b3c4-a1bdf40facdb" );
	FUGID( PIN_INPUT_GEOMETRY, "e6bf944e-5f46-4994-bd51-13c2aa6415b7" );
	FUGID( PIN_OUTPUT_RENDER, "a2bbf374-0dc8-42cb-b85a-6a43b58a348f" );

	QUuid TexturePinIds[4] = {
		PIN_INPUT_TEXTURE0,
		PIN_INPUT_TEXTURE1,
		PIN_INPUT_TEXTURE2,
		PIN_INPUT_TEXTURE3
	};

	for( int i = 0 ; i < 4 ; i++ )
	{
		QSharedPointer<fugio::PinInterface>	P = pinInput( QString( "Texture%1" ).arg( i ), TexturePinIds[ i ] );

		P->setDescription( tr( "The OpenGL texture to bind to texture unit %1" ).arg( i ) );
	}

	mPinState    = pinInput( "State", PIN_INPUT_STATE );
	mPinGeometry = pinInput( "Geometry", PIN_INPUT_GEOMETRY );

	mValOutput = pinOutput<fugio::RenderInterface *>( "Render", mPinOutput, PID_RENDER, PIN_OUTPUT_RENDER );

	mPinState->setDescription( tr( "The OpenGL rendering state to apply" ) );

	mPinGeometry->setDescription( tr( "The input 3D Geometry to render" ) );

	mPinOutput->setDescription( tr( "The output geometry" ) );
}

bool BindTextureNode::initialise()
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

void BindTextureNode::inputsUpdated( qint64 pTimeStamp )
{
	if( !pTimeStamp )
	{
		return;
	}

	pinUpdated( mPinOutput );
}

void BindTextureNode::render( qint64 pTimeStamp, QUuid pSourcePinId )
{
	Q_UNUSED( pSourcePinId )

	initializeOpenGLFunctions();

	OpenGLTextureInterface		*T;

	QList<QSharedPointer<fugio::PinInterface>>		PinLst = mNode->enumInputPins();

	for( int i = 0 ; i < 4 ; i++ )
	{
		glActiveTexture( GL_TEXTURE0 + i );

		if( ( T = input<OpenGLTextureInterface *>( PinLst[ i ] ) ) != nullptr )
		{
			T->srcBind();

//			glEnable( T->target() );
		}
	}

	//-------------------------------------------------------------------------

	OpenGLStateInterface		*CurrentState = 0;
	OpenGLStateInterface		*NextState;
	fugio::RenderInterface		*Geometry;

	//-------------------------------------------------------------------------

	for( QSharedPointer<fugio::PinInterface> InpPin : PinLst )
	{
		if( !InpPin->isConnectedToActiveNode() )
		{
			continue;
		}

		QSharedPointer<PinControlInterface>		 PinCtl = InpPin->connectedPin()->control();

		if( PinCtl.isNull() )
		{
			continue;
		}

		if( ( Geometry = qobject_cast<fugio::RenderInterface *>( PinCtl->qobject() ) ) != 0 )
		{
			Geometry->render( pTimeStamp );

			continue;
		}

		if( ( NextState = qobject_cast<OpenGLStateInterface *>( PinCtl->qobject() ) ) != 0 )
		{
			if( CurrentState != 0 )
			{
				CurrentState->stateEnd();
			}

			CurrentState = NextState;

			CurrentState->stateBegin();
		}
	}

	if( CurrentState != 0 )
	{
		CurrentState->stateEnd();
	}

	//-------------------------------------------------------------------------

	for( int i = 0 ; i < PinLst.size() ; i++ )
	{
		glActiveTexture( GL_TEXTURE0 + i );

		if( ( T = input<OpenGLTextureInterface *>( PinLst[ i ] ) ) != nullptr )
		{
			T->release();

//			glDisable( T->target() );
		}
	}
}
