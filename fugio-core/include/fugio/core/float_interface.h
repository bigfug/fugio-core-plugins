#ifndef FLOAT_INTERFACE_H
#define FLOAT_INTERFACE_H

#include <fugio/global.h>

FUGIO_NAMESPACE_BEGIN

class FloatInterface
{
public:
	virtual ~FloatInterface( void ) {}

	virtual void setValue( double pValue ) = 0;
	virtual void setMinimum( double pMinimum ) = 0;
	virtual void setMaximum( double pMaximum ) = 0;
	virtual void setRange( double pMinimum, double pMaximum ) = 0;

	virtual double value( void ) const = 0;
};

FUGIO_NAMESPACE_END

Q_DECLARE_INTERFACE( fugio::FloatInterface, "com.bigfug.fugio.float/1.0" )

#endif // FLOAT_INTERFACE_H
