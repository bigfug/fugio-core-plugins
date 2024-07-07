#include "matrix4node.h"

#include <QSettings>

#include <fugio/math/uuid.h>

Matrix4Node::Matrix4Node( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_OUTPUT_MATRIX, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );

	mPinOutputValue = pinOutput<fugio::MatrixInterface *>( "Matrix", mPinOutput, PID_MATRIX4, PIN_OUTPUT_MATRIX );
}

Matrix4Node::~Matrix4Node()
{

}
