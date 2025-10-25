
#include "renderpin.h"

#include <QSettings>

#include <fugio/node_interface.h>
#include <fugio/node_control_interface.h>
#include <fugio/pin_interface.h>
#include <fugio/opengl/node_render_interface.h>

RenderPin::RenderPin( QSharedPointer<fugio::PinInterface> pPin )
	: PinControlBase( pPin )
{
}

RenderPin::~RenderPin()
{
}

void RenderPin::render( qint64 pTimeStamp )
{
	if( !mPin->node()->isInitialised() || !mPin->node()->hasControl() )
	{
		return;
	}

	QObject *O = mPin->node()->control()->qobject();

	qDebug() << "DLL 1 (Provider) 'MyInterface' ID:"
			 << qMetaTypeId<fugio::NodeRenderInterface*>();

	fugio::NodeRenderInterface	*IG = qobject_cast<fugio::NodeRenderInterface *>( O );

	fugio::NodeRenderInterface	*IG2 = dynamic_cast<fugio::NodeRenderInterface *>( O );

	if( IG )
	{
		IG->render( pTimeStamp, mPin->localId() );
	}

	if( IG2 )
	{
		IG2->render( pTimeStamp, mPin->localId() );
	}
}
