#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include <QObject>

#include <fugio/core/uuid.h>
#include <fugio/global_interface.h>
#include <fugio/plugin_interface.h>

#include "config.h"

using namespace fugio;

class ColourPlugin : public QObject, public fugio::PluginInterface
{
	Q_OBJECT
	Q_INTERFACES( fugio::PluginInterface )
    Q_PLUGIN_METADATA( IID FUGIO_PLUGIN_IID FILE "manifest.json" )

public:
	explicit ColourPlugin( void );

	virtual ~ColourPlugin( void );

	//-------------------------------------------------------------------------
	// fugio::PluginInterface

	virtual InitResult initialise( fugio::GlobalInterface *pApp, bool pLastChance );

	virtual void deinitialise( void );

private:
	static ClassEntry		 mNodeClasses[];
	static ClassEntry		 mPinClasses[];

	fugio::GlobalInterface			*mApp;
};

#endif // COREPLUGIN_H
