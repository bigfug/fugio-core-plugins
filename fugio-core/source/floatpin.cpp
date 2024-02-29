#include "floatpin.h"
#include <fugio/core/uuid.h>
#include <fugio/node_interface.h>
#include <fugio/context_interface.h>

#include <QSettings>
#include <QDoubleSpinBox>

FloatPin::FloatPin( QSharedPointer<fugio::PinInterface> pPin )
	: PinControlBase( pPin ), VariantHelper( QMetaType::Double, PID_FLOAT ),
	mMinimum( std::numeric_limits<double>::lowest() ), mMaximum( std::numeric_limits<double>::max() )
{
}

QString FloatPin::toString() const
{
	QStringList		L;

	for( double v : mValues )
	{
		L << QString::number( v );
	}

	return( L.join( ',' ) );
}

void FloatPin::loadSettings( QSettings &pSettings )
{
	fugio::PinControlBase::loadSettings( pSettings );

	if( pSettings.childGroups().contains( "values" ) )
	{
		int	Count = pSettings.beginReadArray( "values" );

		mValues.resize( Count );

		for( int i = 0 ; i < mValues.size() ; i++ )
		{
			pSettings.setArrayIndex( i );

			mValues[ i ] = pSettings.value( "i", mValues[ i ] ).toDouble();
		}

		pSettings.endArray();
	}
}

void FloatPin::saveSettings( QSettings &pSettings ) const
{
	fugio::PinControlBase::saveSettings( pSettings );

	pSettings.beginWriteArray( "values", mValues.size() );

	for( int i = 0 ; i < mValues.size() ; i++ )
	{
		pSettings.setArrayIndex( i );

		pSettings.setValue( "i", mValues.at( i ) );
	}

	pSettings.endArray();
}

void FloatPin::setValue( double pValue )
{
	pin()->setValue( pValue );
}

void FloatPin::setMinimum( double pMinimum )
{
	mMinimum = pMinimum;
}

void FloatPin::setMaximum(double pMaximum)
{
	mMaximum = pMaximum;
}

void FloatPin::setRange( double pMinimum, double pMaximum )
{
	mMinimum = pMinimum;
	mMaximum = pMaximum;
}

double FloatPin::value() const
{
	QSharedPointer<fugio::PinInterface>		P = pin();
	double									R = P->value().toDouble();

	if( P && P->isConnected() && P->connectedPin()->hasControl() )
	{
		VariantInterface *V = qobject_cast<VariantInterface *>( P->connectedPin()->control()->qobject() );

		if( V )
		{
			R = V->variant().toDouble();
		}
	}

	if( R < mMinimum )
	{
		R = mMinimum;
	}

	if( R > mMaximum )
	{
		R = mMaximum;
	}

	return( R );
}

QWidget *FloatPin::inspectorWidget()
{
	if( pin()->direction() == PIN_OUTPUT )
	{
		return( Q_NULLPTR );
	}

	QDoubleSpinBox		*W = new QDoubleSpinBox();

	W->setRange( mMinimum, mMaximum );
	W->setValue( pin()->value().toDouble() );
	W->setDecimals( 3 );

	if( mMinimum > std::numeric_limits<double>::lowest() && mMaximum < std::numeric_limits<double>::max() )
	{
		W->setSingleStep( ( mMaximum - mMinimum ) / 10.0 );
	}

	connect( W, qOverload<double>( &QDoubleSpinBox::valueChanged ), this, [=]( double pValue )
	{
		setValue( pValue );

		mPin->node()->context()->pinUpdated( mPin, mPin->node()->context()->global()->timestamp() );
	}, Qt::AutoConnection );

	return( W );
}
