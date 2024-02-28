#ifndef FLOATPIN_H
#define FLOATPIN_H

#include <QObject>

#include <fugio/pin_interface.h>
#include <fugio/pin_control_interface.h>

#include <fugio/core/variant_helper.h>
#include <fugio/core/float_interface.h>
#include <fugio/inspector_widget_interface.h>

#include <fugio/serialise_interface.h>

#include <fugio/pincontrolbase.h>

class FloatPin : public fugio::PinControlBase, public fugio::VariantHelper<double>, public fugio::SerialiseInterface, public fugio::FloatInterface,
				 public fugio::InspectorWidgetInterface
{
	Q_OBJECT
	Q_INTERFACES( fugio::VariantInterface fugio::SerialiseInterface fugio::FloatInterface fugio::InspectorWidgetInterface )
//	Q_PROPERTY( double mValue READ value WRITE setValue NOTIFY valueChanged )

	Q_CLASSINFO( "Author", "Alex May" )
	Q_CLASSINFO( "Version", "1.0" )
	Q_CLASSINFO( "Description", "" )
	Q_CLASSINFO( "URL", WIKI_PIN_URL( "Float" ) )
	Q_CLASSINFO( "Contact", "http://www.bigfug.com/contact/" )

public:
	Q_INVOKABLE explicit FloatPin( QSharedPointer<fugio::PinInterface> pPin );

	virtual ~FloatPin( void ) {}

	//-------------------------------------------------------------------------
	// Q_PROPERTY

//	Q_INVOKABLE double value( void ) const
//	{
//		return( mValue );
//	}

//	Q_INVOKABLE void setValue( double pValue )
//	{
//		if( pValue != mValue )
//		{
//			mValue = pValue;

//			emit valueChanged( pValue );
//		}
//	}

	//-------------------------------------------------------------------------
	// fugio::PinControlInterface

	virtual QString toString( void ) const Q_DECL_OVERRIDE;

	virtual QString description( void ) const Q_DECL_OVERRIDE
	{
		return( "Float" );
	}

	virtual void loadSettings( QSettings &pSettings ) Q_DECL_OVERRIDE;
	virtual void saveSettings( QSettings &pSettings ) const Q_DECL_OVERRIDE;

	//-------------------------------------------------------------------------
	// fugio::FloatInterface

	virtual void setValue( double pValue ) Q_DECL_OVERRIDE;

	virtual void setMinimum( double pMinimum ) Q_DECL_OVERRIDE;
	virtual void setMaximum( double pMaximum ) Q_DECL_OVERRIDE;
	virtual void setRange( double pMinimum, double pMaximum ) Q_DECL_OVERRIDE;

	virtual double value( void ) const Q_DECL_OVERRIDE;

	//-------------------------------------------------------------------------
	// fugio::InspectorWidgetInterface

	virtual QWidget *inspectorWidget( void ) Q_DECL_OVERRIDE;

	//-------------------------------------------------------------------------
	// fugio::SerialiseInterface

	virtual void serialise( QDataStream &pDataStream ) const Q_DECL_OVERRIDE
	{
		if( mValues.size() == 1 )
		{
			pDataStream << mValues.first();
		}
		else
		{
			pDataStream << mValues;
		}
	}

	virtual void deserialise( QDataStream &pDataStream ) Q_DECL_OVERRIDE
	{
#if QT_VERSION >= QT_VERSION_CHECK( 5, 7, 0 )
		double				V;

		pDataStream.startTransaction();

		pDataStream >> V;

		if( pDataStream.commitTransaction() )
		{
			mValues.resize( 1 );

			setVariant( 0, V );

			return;
		}

		pDataStream.rollbackTransaction();
#endif

		QVector<double>	L;

		pDataStream >> L;

		mValues = L;
	}

signals:
//	void valueChanged( double pValue );

private:
	double				mMinimum;
	double				mMaximum;
};

#endif // FLOATPIN_H
