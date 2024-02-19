#ifndef TIMEPIN_H
#define TIMEPIN_H

#include <QTime>

#include "config.h"

#include <fugio/core/variant_helper.h>

#include <fugio/serialise_interface.h>

#include <fugio/pincontrolbase.h>

class TimePin : public fugio::PinControlBase, public fugio::VariantHelper<QTime>, public fugio::SerialiseInterface
{
	Q_OBJECT
	Q_INTERFACES( fugio::VariantInterface fugio::SerialiseInterface )

	Q_CLASSINFO( "Author", "Alex May" )
	Q_CLASSINFO( "Version", "1.0" )
	Q_CLASSINFO( "Description", "" )
	Q_CLASSINFO( "URL", WIKI_PIN_URL( "Time" ) )
	Q_CLASSINFO( "Contact", "https://github.com/bigfug" )

public:
	Q_INVOKABLE explicit TimePin( QSharedPointer<fugio::PinInterface> pPin );

	virtual ~TimePin( void ) {}

	//-------------------------------------------------------------------------
	// fugio::PinControlInterface

	virtual QString toString( void ) const Q_DECL_OVERRIDE;

	virtual QString description( void ) const Q_DECL_OVERRIDE
	{
		return( "Time" );
	}

	//-------------------------------------------------------------------------
	// fugio::SerialiseInterface

	virtual void serialise( QDataStream &pDataStream ) const Q_DECL_OVERRIDE;

	virtual void deserialise( QDataStream &pDataStream ) Q_DECL_OVERRIDE;
};

#endif // TIMEPIN_H
