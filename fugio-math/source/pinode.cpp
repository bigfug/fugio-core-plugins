#include "pinode.h"

#include <fugio/core/uuid.h>

#include <qmath.h>

PiNode::PiNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_OUTPUT_PI, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );

	mValOutput = pinOutput<fugio::VariantInterface *>( "PI", mPinOutput, PID_FLOAT, PIN_OUTPUT_PI );

	mValOutput->setVariant( double( M_PI ) );
}

