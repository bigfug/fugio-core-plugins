#include "loadnode.h"

#include <QFileInfo>
#include <QFile>

LoadNode::LoadNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_FILENAME,		"9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_BYTEARRAY,	"1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );

	mPinInputTrigger = pinInput( "Trigger", PID_FUGIO_NODE_TRIGGER );

	mValInputFilename = pinInput<fugio::FilenameInterface *>( "Filename", mPinInputFilename, PID_FILENAME, PIN_INPUT_FILENAME );

	mValOutputByteArray = pinOutput<fugio::VariantInterface *>( "ByteArray", mPinOutputByteArray, PID_BYTEARRAY, PIN_OUTPUT_BYTEARRAY );
}

void LoadNode::inputsUpdated( qint64 pTimeStamp )
{
	QString			Filename = mLastFilename;

	if( !pTimeStamp || mPinInputFilename->isUpdated( pTimeStamp ) )
	{
		if( mPinInputFilename->isConnected() )
		{
			fugio::FilenameInterface	*F = input<fugio::FilenameInterface *>( mPinInputFilename );

			if( F )
			{
				Filename = F->filename();
			}
			else
			{
				Filename = variant( mPinInputFilename ).toString();
			}
		}
		else
		{
			Filename = mValInputFilename->filename();
		}

		if( Filename.isEmpty() )
		{
			return;
		}
	}

	if( Filename != mLastFilename )
	{
		mLastFilename = Filename;
		mLastModified = QDateTime();
	}

	if( !QFileInfo::exists( Filename ) )
	{
		return;
	}

	QFileInfo		FileInfo( Filename );

	if( !FileInfo.isReadable() )
	{
		return;
	}

	if( FileInfo.lastModified() <= mLastModified )
	{
		return;
	}

	QFile			File( Filename );

	if( File.open( QFile::ReadOnly ) )
	{
		mValOutputByteArray->setVariant( File.readAll() );

		pinUpdated( mPinOutputByteArray );

		File.close();
	}
}
