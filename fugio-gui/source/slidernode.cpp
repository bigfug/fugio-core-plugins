#include "slidernode.h"

#include <QSlider>
#include <QSettings>

SliderNode::SliderNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_OUTPUT_NUMBER, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );

	pinOutput<fugio::VariantInterface>( "Number", mValue, PID_FLOAT, PIN_OUTPUT_NUMBER );
}

SliderNode::~SliderNode()
{

}


QWidget *SliderNode::gui()
{
	QSlider		*Slider = new QSlider();

	Slider->setOrientation( Qt::Horizontal );

	Slider->setMinimum( 0 );
	Slider->setMaximum( 1000 );

	Slider->setValue( int( mValue.mEnt->variant().toFloat() * 1000.0f ) );

	connect( Slider, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)) );

	connect( this, SIGNAL(valueChanged(int)), Slider, SLOT(setValue(int)) );

	return( Slider );
}

void SliderNode::sliderValueChanged( int pValue )
{
	float		NewVal = float( pValue ) / 1000.0f;

	if( NewVal != mValue.mEnt->variant().toFloat() )
	{
		mValue.mEnt->setVariant( NewVal );

		pinUpdated( mValue.mPin );
	}
}

void SliderNode::loadSettings( QSettings &pSettings)
{
	float		NewVal = pSettings.value( "value", mValue.mEnt->variant() ).toFloat();

	if( NewVal != mValue.mEnt->variant().toFloat() )
	{
		mValue.mEnt->setVariant( NewVal );

		pinUpdated( mValue.mPin );
	}

	emit valueChanged( int( NewVal * 1000.0f ) );
}

void SliderNode::saveSettings( QSettings &pSettings ) const
{
	pSettings.setValue( "value", mValue.mEnt->variant().toFloat() );
}
