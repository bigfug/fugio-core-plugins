#include "mainwindownode.h"

#include <QApplication>
#include <QKeyEvent>

#include <fugio/context_signals.h>

MainWindowNode::MainWindowNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_OUTPUT_EVENTS, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_POSITION, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );

	mKeyboard = pinOutput<fugio::KeyboardInterface *>( "Keyboard", mPinKeyboard, PID_KEYBOARD, PIN_OUTPUT_EVENTS );

	mEvents = pinOutput<fugio::InputEventsInterface *>( "Events", mPinEvents, PID_INPUT_EVENTS, PIN_OUTPUT_POSITION );
}

bool MainWindowNode::initialise()
{
	if( !fugio::NodeControlBase::initialise() )
	{
		return( false );
	}

	qApp->installEventFilter( this );

	connect( mNode->context()->qobject(), SIGNAL(frameStart()), this, SLOT(contextFrameStart()) );

	return( true );
}

bool MainWindowNode::deinitialise()
{
	disconnect( mNode->context()->qobject(), SIGNAL(frameStart()), this, SLOT(contextFrameStart()) );

	qApp->removeEventFilter( this );

	return( NodeControlBase::deinitialise() );
}

bool MainWindowNode::eventFilter( QObject *pObject, QEvent *pEvent )
{
	QInputEvent		*IE = dynamic_cast<QInputEvent *>( pEvent );

	if( IE )
	{
		mEvents->inputProcessEvent( IE );
	}

	if( pEvent->type() == QEvent::KeyPress )
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>( pEvent );

		if( !keyEvent->isAutoRepeat() )
		{
			fugio::KeyboardEvent		KeyEvt;

			KeyEvt.mType = fugio::KeyboardEvent::PRESS;
			KeyEvt.mText = keyEvent->text();
			KeyEvt.mCode = keyEvent->key();

			KeyEvt.translateModifiers( keyEvent->modifiers() );

			mEvtLst << KeyEvt;
		}
	}
	else if( pEvent->type() == QEvent::KeyRelease )
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>( pEvent );

		if( !keyEvent->isAutoRepeat() )
		{
			fugio::KeyboardEvent		KeyEvt;

			KeyEvt.mType = fugio::KeyboardEvent::RELEASE;
			KeyEvt.mText = keyEvent->text();
			KeyEvt.mCode = keyEvent->key();

			KeyEvt.translateModifiers( keyEvent->modifiers() );

			mEvtLst << KeyEvt;
		}
	}

	// standard event processing
	return( QObject::eventFilter( pObject, pEvent ) );
}

void MainWindowNode::contextFrameStart()
{
	mKeyboard->keyboardClearEvents();

	if( !mEvtLst.isEmpty() )
	{
		mKeyboard->keyboardAddEvents( mEvtLst );

		pinUpdated( mPinKeyboard );

		mEvtLst.clear();
	}

	mEvents->inputFrameInitialise();

	if( mEvents->hasEvents() )
	{
		pinUpdated( mPinEvents );
	}
}
