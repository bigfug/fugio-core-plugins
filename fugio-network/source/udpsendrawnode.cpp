#include "udpsendrawnode.h"

#include <fugio/core/uuid.h>

#include <fugio/context_interface.h>
#include <fugio/core/array_interface.h>
#include <fugio/core/variant_interface.h>
#include <fugio/serialise_interface.h>
#include <fugio/performance.h>
#include <fugio/pin_variant_iterator.h>

UDPSendRawNode::UDPSendRawNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode ), mWriteTime( 0 )
{
	FUGID( PIN_INPUT_HOST, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_PORT, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_INPUT_DATA, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );

	mPinHost = pinInput( "Host", PIN_INPUT_HOST );
	mPinPort = pinInput( "Port", PIN_INPUT_PORT );
	mPinData = pinInput( "Data", PIN_INPUT_DATA );

	mPinHost->registerPinInputType( PID_STRING );
	mPinPort->registerPinInputType( PID_INTEGER );
	mPinData->registerPinInputType( PID_BYTEARRAY );

	mPinHost->setValue( "localhost" );
	mPinPort->setValue( 7878 );
}

void UDPSendRawNode::inputsUpdated( qint64 pTimeStamp )
{
	fugio::Performance	Perf( mNode, "inputsUpdated", pTimeStamp );

	if( mAddress == QHostAddress::Null || mPinHost->isUpdated( pTimeStamp ) )
	{
		mAddress = QHostAddress::Null;

		QHostInfo::lookupHost( variant( mPinHost ).toString(), this, SLOT(hostLookup(QHostInfo)) );
	}

	if( mAddress == QHostAddress::Null )
	{
		return;
	}

	if( !mPinData->isUpdated( mWriteTime ) )
	{
		return;
	}

	int							Port = variant<int>( mPinPort );

	fugio::PinVariantIterator	Data( mPinData );

	for( int i = 0 ; i < Data.count() ; i++ )
	{
		const QVariant	VarDat = Data.index( i );

		if( VarDat.canConvert( QVariant::ByteArray ) )
		{
			mSocket.writeDatagram( VarDat.toByteArray(), mAddress, Port );
		}
	}

	mWriteTime = pTimeStamp + 1;
}

void UDPSendRawNode::hostLookup( QHostInfo pHostInfo )
{
	if( !pHostInfo.addresses().isEmpty() )
	{
		mAddress = pHostInfo.addresses().first();

		mNode->context()->updateNode( mNode );
	}
}
