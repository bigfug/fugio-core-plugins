#ifndef MATRIXMULTIPLYNODE_H
#define MATRIXMULTIPLYNODE_H

#include <fugio/node_interface.h>
#include <fugio/node_control_interface.h>
#include <fugio/pin_interface.h>

#include <fugio/variant_interface.h>

#include <fugio/nodecontrolbase.h>

class MatrixMultiplyNode : public fugio::NodeControlBase
{
	Q_OBJECT
	Q_CLASSINFO( "Author", "Alex May" )
	Q_CLASSINFO( "Version", "1.0" )
	Q_CLASSINFO( "Description", "Create a 'Look At' Matrix4" )
	Q_CLASSINFO( "URL", WIKI_URL( "Multiply-Matrix" ) )
	Q_CLASSINFO( "Contact", "https://github.com/bigfug" )

public:
	Q_INVOKABLE explicit MatrixMultiplyNode( QSharedPointer<fugio::NodeInterface> pNode );

	virtual ~MatrixMultiplyNode( void ) {}

	//-------------------------------------------------------------------------
	// NodeControlInterface

	virtual void inputsUpdated( qint64 pTimeStamp );

private:
	QSharedPointer<fugio::PinInterface>			 mPinMatrix;
	QSharedPointer<fugio::PinInterface>			 mPinVector;

	QSharedPointer<fugio::PinInterface>			 mPinOutput;
	fugio::VariantInterface						*mOutput;
};

#endif // MATRIXMULTIPLYNODE_H
