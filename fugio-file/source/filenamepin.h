#ifndef FILENAMEPIN_H
#define FILENAMEPIN_H

#include <QObject>

#include <fugio/core/uuid.h>

#include <fugio/pincontrolbase.h>

#include <fugio/file/filename_interface.h>
#include <fugio/core/variant_helper.h>
#include <fugio/inspector_widget_interface.h>

class FilenamePin : public fugio::PinControlBase, public fugio::FilenameInterface, public fugio::VariantHelper<QString>, public fugio::InspectorWidgetInterface
{
	Q_OBJECT
	Q_INTERFACES( fugio::VariantInterface fugio::FilenameInterface fugio::InspectorWidgetInterface )

public:
	Q_INVOKABLE explicit FilenamePin( QSharedPointer<fugio::PinInterface> pPin );

	virtual ~FilenamePin( void ) Q_DECL_OVERRIDE {}

	//-------------------------------------------------------------------------
	// fugio::PinControlInterface

	virtual QString toString( void ) const Q_DECL_OVERRIDE
	{
		return( mPin->isConnected() ? QString() : filename() );
	}

	virtual QString description( void ) const Q_DECL_OVERRIDE
	{
		return( "Filename" );
	}

	virtual void loadSettings( QSettings &pSettings ) Q_DECL_OVERRIDE;
	virtual void saveSettings( QSettings &pSettings ) const Q_DECL_OVERRIDE;

	//-------------------------------------------------------------------------
	// InterfaceFilename

	virtual QString filename( void ) const Q_DECL_OVERRIDE
	{
		if( pin()->direction() == PIN_INPUT )
		{
			return( pin()->value().toString() );
		}

		return( variant().toString() );
	}

	virtual void setFilename( const QString &pFilename ) Q_DECL_OVERRIDE
	{
		if( pin()->direction() == PIN_INPUT )
		{
			pin()->setValue( pFilename );
		}
		else
		{
			setVariant( pFilename );
		}
	}

	//-------------------------------------------------------------------------
	// fugio::InspectorWidgetInterface

	virtual QWidget *inspectorWidget( void ) Q_DECL_OVERRIDE;
};

#endif // FILENAMEPIN_H
