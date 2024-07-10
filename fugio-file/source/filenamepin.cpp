#include "filenamepin.h"

#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>

#include <fugio/node_interface.h>
#include <fugio/context_interface.h>

FilenamePin::FilenamePin( QSharedPointer<fugio::PinInterface> pPin )
	: PinControlBase( pPin ), VariantHelper( QMetaType::QString, PID_STRING )
{

}

void FilenamePin::loadSettings( QSettings &pSettings )
{
	QString		FileName = ( mPin->direction() == PIN_INPUT ? mPin->value().toString() : mValues.first() );

	FileName = pSettings.value( "filename", FileName ).toString();

	if( !FileName.isEmpty() )
	{
		QFileInfo	FileInfo( pSettings.fileName() );
		QDir		FileDir( FileInfo.absolutePath() );
		QFileInfo	DestInfo( FileDir.absoluteFilePath( FileName ) );

		FileName = DestInfo.exists() ? DestInfo.canonicalFilePath() : FileName;

		setFilename( FileName );
	}
}

void FilenamePin::saveSettings( QSettings &pSettings ) const
{
	QString		FileName = ( mPin->direction() == PIN_INPUT ? mPin->value().toString() : mValues.first() );

	if( !FileName.isEmpty() )
	{
		QFileInfo	FileInfo( pSettings.fileName() );
		QDir		FileDir( FileInfo.absolutePath() );

		FileName = FileDir.relativeFilePath( FileName );

		pSettings.setValue( "filename", FileName );
	}
}

QWidget *FilenamePin::inspectorWidget()
{
	QWidget			*WidgetContainer = new QWidget();

	QLineEdit		*W = new QLineEdit( WidgetContainer );

	QPushButton			*B = new QPushButton( WidgetContainer );

	W->setText( filename() );

	W->setReadOnly( true );

	QHBoxLayout *layout = new QHBoxLayout();

	layout->addWidget(W);
	layout->addWidget(B);

	WidgetContainer->setLayout(layout);

	connect( B, &QPushButton::clicked, this, [=]( bool checked )
	{
		QString FN = QFileDialog::getOpenFileName( nullptr, QString(), filename() );

		if( !FN.isEmpty() )
		{
			setFilename( FN );
		}

		pin()->node()->context()->pinUpdated( pin(), pin()->node()->context()->global()->timestamp() );

		W->setText( FN );
	}, Qt::AutoConnection );

	return( WidgetContainer );
}
