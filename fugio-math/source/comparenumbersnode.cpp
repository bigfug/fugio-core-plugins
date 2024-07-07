#include "comparenumbersnode.h"

#include <fugio/core/uuid.h>

#include <fugio/context_interface.h>

CompareNumbersNode::CompareNumbersNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_NUMBER1, "c13a41c6-544b-46bb-a9f2-19dd156d236c" );
	FUGID( PIN_INPUT_NUMBER2, "608ac771-490b-4ae6-9c81-12b9af526d09" );
	FUGID( PIN_OUTPUT_EQUAL, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_FUZZY, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_OUTPUT_GT, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );

	mPinInput1 = pinInput( "Number 1", PIN_INPUT_NUMBER1 );
	mPinInput2 = pinInput( "Number 2", PIN_INPUT_NUMBER2 );

	mValEqual  = pinOutput<fugio::VariantInterface *>( "Equal", mPinEqual, PID_BOOL, PIN_OUTPUT_EQUAL );
	mValFuzzy  = pinOutput<fugio::VariantInterface *>( "Fuzzy", mPinFuzzy, PID_BOOL, PIN_OUTPUT_FUZZY );
	mValHigher = pinOutput<fugio::VariantInterface *>( "N1>N2", mPinHigher, PID_BOOL, PIN_OUTPUT_GT );
}

void CompareNumbersNode::inputsUpdated( qint64 pTimeStamp )
{
	Q_UNUSED( pTimeStamp )

	qreal		v1 = variant( mPinInput1 ).toReal();
	qreal		v2 = variant( mPinInput2 ).toReal();

	bool		Eq = bool( v1 == v2 );
	bool		Fz = qFuzzyCompare( v1, v2 );
	bool		Gt = bool( v1 > v2 );

	if( !pTimeStamp || Eq != mValEqual->variant().toBool() )
	{
		mValEqual->setVariant( Eq );

		pinUpdated( mPinEqual );
	}

	if( !pTimeStamp || Fz != mValFuzzy->variant().toBool() )
	{
		mValFuzzy->setVariant( Fz );

		pinUpdated( mPinFuzzy );
	}

	if( !pTimeStamp || Gt != mValHigher->variant().toBool() )
	{
		mValHigher->setVariant( Gt );

		pinUpdated( mPinHigher );
	}
}
