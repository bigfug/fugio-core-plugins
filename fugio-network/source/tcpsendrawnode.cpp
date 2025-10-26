#include "tcpsendrawnode.h"

#include <fugio/core/uuid.h>

#include <fugio/context_interface.h>
#include <fugio/core/array_interface.h>
#include <fugio/variant_interface.h>
#include <fugio/serialise_interface.h>
#include <fugio/performance.h>

#include "networkplugin.h"

TCPSendRawNode::TCPSendRawNode( QSharedPointer<fugio::NodeInterface> pNode )
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

	mStream.setDevice( &mSocket );
}

bool TCPSendRawNode::initialise()
{
	if( !NodeControlBase::initialise() )
	{
		return( false );
	}

	connect( &mSocket, SIGNAL(connected()), this, SLOT(socketConnected()) );
	connect( &mSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected() ) );
	connect( &mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)) );
	connect( &mSocket, SIGNAL(hostFound()), this, SLOT(socketHostFound()) );

	return( true );
}

bool TCPSendRawNode::deinitialise()
{
	mSocket.disconnectFromHost();

	return( NodeControlBase::deinitialise() );
}

void TCPSendRawNode::inputsUpdated( qint64 pTimeStamp )
{
	if( mPinHost->isUpdated( pTimeStamp ) || mPinPort->isUpdated( pTimeStamp ) )
	{
		if( mSocket.state() == QAbstractSocket::ConnectedState )
		{
			mSocket.disconnectFromHost();
		}

		if( mSocket.state() != QAbstractSocket::UnconnectedState )
		{
			return;
		}

		mNode->setStatus( fugio::NodeInterface::Initialising );
		mNode->setStatusMessage( "Connecting" );

		mSocket.connectToHost( variant( mPinHost ).toString(), variant( mPinPort ).toInt() );

		return;
	}

	if( mSocket.state() != QAbstractSocket::ConnectedState )
	{
		return;
	}

	fugio::Performance	Perf( mNode, "inputsUpdated", pTimeStamp );

	sendData( pTimeStamp );
}

void TCPSendRawNode::socketConnected()
{
	mNode->setStatus( fugio::NodeInterface::Initialised );
	mNode->setStatusMessage( "Connected" );

	sendData( NetworkPlugin::instance()->app()->timestamp() );
}

void TCPSendRawNode::socketDisconnected()
{
	mNode->setStatus( fugio::NodeInterface::Warning );
	mNode->setStatusMessage( "Disconnected" );
}

void TCPSendRawNode::socketError( QAbstractSocket::SocketError pError )
{
	Q_UNUSED( pError )

	mNode->setStatus( fugio::NodeInterface::Error );
	mNode->setStatusMessage( mSocket.errorString() );
}

void TCPSendRawNode::socketHostFound()
{
	mNode->setStatusMessage( "Host Found" );
}

void TCPSendRawNode::sendData( qint64 pTimeStamp )
{
	if( !mPinData->isUpdated( mWriteTime ) )
	{
		return;
	}

	const QByteArray		 A = variant( mPinData ).toByteArray();

	if( !A.isEmpty() )
	{
		qDebug() << "Sending:" << A.length();

		mStream.writeRawData( A.data(), A.length() );
	}

	mWriteTime = pTimeStamp + 1;
}
