#include "buttonnode.h"

#include <QSettings>
#include <QPushButton>
#include <QDateTime>

#include <fugio/context_interface.h>
#include <fugio/context_signals.h>
#include <fugio/core/uuid.h>
#include <fugio/gui/uuid.h>

ButtonNode::ButtonNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode ), mToggle( false ), mClicked( false ), mPressed( false ), mReleased( false )
{
	FUGID( PIN_OUTPUT_TRIGGER, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_BOOLEAN, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_OUTPUT_TOGGLE, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );

//	mPinLabel = pinInput( "Label", PII_LABEL );

	mValTrigger = pinOutput<fugio::VariantInterface *>( "Trigger", mPinTrigger, PID_TRIGGER, PIN_OUTPUT_TRIGGER );

	mValBoolean = pinOutput<fugio::VariantInterface *>( "Boolean", mPinBoolean, PID_BOOL, PIN_OUTPUT_BOOLEAN );

	mValToggle  = pinOutput<fugio::VariantInterface *>( "Toggle", mPinToggle, PID_BOOL, PIN_OUTPUT_TOGGLE );

//	mPinLabel->setValue( tr( "Button" ) );

	mPinTrigger->setDescription( tr( "When the button is pressed (not released) this trigger will fire" ) );

	mPinBoolean->setDescription( tr( "This pin is true when the button is pressed, false when it isn't" ) );

	mPinToggle->setDescription( tr( "This boolean will switch between true and false every time the button is pressed" ) );
}

ButtonNode::~ButtonNode()
{

}

QWidget *ButtonNode::gui()
{
//	QPushButton	*GUI = new QPushButton( variant( mPinLabel ).toString() );
	QPushButton	*GUI = new QPushButton( tr( "Button" ) );

	connect( GUI, SIGNAL(clicked()), this, SLOT(onClick()) );

	connect( GUI, SIGNAL(pressed()), this, SLOT(onPressed()) );

	connect( GUI, SIGNAL(released()), this, SLOT(onReleased()) );

	return( GUI );
}

void ButtonNode::onClick()
{
	mClicked = true;

	connect( mNode->context()->qobject(), SIGNAL(frameStart(qint64)), this, SLOT(onContextFrameStart(qint64)) );
}

void ButtonNode::onPressed()
{
	mPressed = true;
	mReleased = false;

	connect( mNode->context()->qobject(), SIGNAL(frameStart(qint64)), this, SLOT(onContextFrameStart(qint64)) );
}

void ButtonNode::onReleased()
{
	mReleased = true;
	mPressed = false;

	connect( mNode->context()->qobject(), SIGNAL(frameStart(qint64)), this, SLOT(onContextFrameStart(qint64)) );
}

void ButtonNode::onContextFrameStart(qint64)
{
	if( mClicked )
	{
		mToggle = ( mToggle ? false : true );

		mValToggle->setVariant( mToggle );

		pinUpdated( mPinToggle );

		mClicked = false;
	}

	if( mPressed )
	{
		pinUpdated( mPinTrigger );

		mValBoolean->setVariant( true );

		pinUpdated( mPinBoolean );

		mPressed = false;
	}

	if( mReleased )
	{
		mValBoolean->setVariant( false );

		pinUpdated( mPinBoolean );

		mReleased = false;
	}

	disconnect( mNode->context()->qobject(), SIGNAL(frameStart(qint64)), this, SLOT(onContextFrameStart(qint64)) );
}


void ButtonNode::loadSettings( QSettings &pSettings )
{
	mToggle = pSettings.value( "toggle", mToggle ).toBool();

	mValToggle->setVariant( mToggle );
}

void ButtonNode::saveSettings( QSettings &pSettings ) const
{
	pSettings.setValue( "toggle", mToggle );
}
