#ifndef TCPSERVERNODE_H
#define TCPSERVERNODE_H

#include <QObject>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>
#include <QPointer>

#include <fugio/nodecontrolbase.h>
#include <fugio/core/variant_interface.h>

class TCPReceiveRawNode : public fugio::NodeControlBase
{
	Q_OBJECT
	Q_CLASSINFO( "Author", "Alex May" )
	Q_CLASSINFO( "Version", "1.0" )
	Q_CLASSINFO( "Description", "TCP Server" )
	Q_CLASSINFO( "URL", WIKI_NODE_URL( "TCP_Receive_Raw" ) )
	Q_CLASSINFO( "Contact", "http://www.bigfug.com/contact/" )

public:
	Q_INVOKABLE TCPReceiveRawNode( QSharedPointer<fugio::NodeInterface> pNode );

	virtual ~TCPReceiveRawNode( void ) {}

	// NodeControlInterface interface

	virtual void inputsUpdated( qint64 pTimeStamp ) Q_DECL_OVERRIDE;

	virtual bool initialise() Q_DECL_OVERRIDE;
	virtual bool deinitialise() Q_DECL_OVERRIDE;

protected slots:
	void frameStart( qint64 pTimeStamp );

	void serverNewConnection( void );
	void serverAcceptError( QAbstractSocket::SocketError pError );

	void socketReadyRead( void );
	void socketError( QAbstractSocket::SocketError pError );

protected:
	QSharedPointer<fugio::PinInterface>			 mPinPort;

	QSharedPointer<fugio::PinInterface>			 mPinOutputBuffer;
	fugio::VariantInterface						*mValOutputBuffer;

	QSharedPointer<fugio::PinInterface>			 mPinOutputConnected;
	fugio::VariantInterface						*mValOutputConnected;

	QTcpServer									 mServer;

	QDataStream									*mStream;
};

#endif // TCPSERVERNODE_H
