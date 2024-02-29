#include "choicepin.h"

#include <QComboBox>

#include <fugio/node_interface.h>
#include <fugio/context_interface.h>

ChoicePin::ChoicePin( QSharedPointer<fugio::PinInterface> pPin )
	: PinControlBase( pPin )
{

}

QString ChoicePin::toString() const
{
	return( mPin->value().toString() );
}

QStringList ChoicePin::choices() const
{
	return( mChoices );
}

QWidget *ChoicePin::inspectorWidget()
{
	QComboBox		*W = new QComboBox();

	if( W )
	{
		W->addItems( mChoices );

		W->setCurrentText( toString() );
	}

	connect( W, &QComboBox::currentTextChanged, this, [=]( const QString &pText )
	{
		if( toString() != pText )
		{
			mPin->setValue( pText );

			mPin->node()->context()->pinUpdated( mPin, mPin->node()->context()->global()->timestamp() );
		}
	} );

	return( W );
}

void ChoicePin::setChoices( QStringList pChoices )
{
	if( mChoices != pChoices )
	{
		mChoices = pChoices;

		emit choicesChanged();
	}
}
