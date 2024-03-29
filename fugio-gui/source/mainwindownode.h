#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>

#include <fugio/nodecontrolbase.h>

#include <fugio/gui/keyboard_interface.h>
#include <fugio/gui/input_events_interface.h>

class MainWindowNode : public fugio::NodeControlBase
{
	Q_OBJECT
	Q_CLASSINFO( "Author", "Alex May" )
	Q_CLASSINFO( "Version", "1.0" )
	Q_CLASSINFO( "Description", "" )
	Q_CLASSINFO( "URL", WIKI_NODE_URL( "Main_Window_(GUI)" ) )
	Q_CLASSINFO( "Contact", "https://github.com/bigfug" )

public:
	Q_INVOKABLE MainWindowNode( QSharedPointer<fugio::NodeInterface> pNode );

	virtual ~MainWindowNode( void ) {}

	// NodeControlInterface interface
public:
	virtual bool initialise() Q_DECL_OVERRIDE;
	virtual bool deinitialise() Q_DECL_OVERRIDE;

	// QObject interface
private:
	virtual bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;

protected slots:
	void contextFrameStart( void );

protected:
	QSharedPointer<fugio::PinInterface>			 mPinKeyboard;
	fugio::KeyboardInterface					*mKeyboard;

	QSharedPointer<fugio::PinInterface>			 mPinEvents;
	fugio::InputEventsInterface					*mEvents;

	QList<fugio::KeyboardEvent>					 mEvtLst;
};

#endif // MAINWINDOW_H
