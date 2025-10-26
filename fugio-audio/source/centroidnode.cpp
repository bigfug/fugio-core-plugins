#include "centroidnode.h"

#include <fugio/core/uuid.h>
#include <fugio/audio/uuid.h>

#include <fugio/context_interface.h>
#include <fugio/variant_interface.h>
#include <fugio/audio/fft_interface.h>

#include <qmath.h>

CentroidNode::CentroidNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode ), mCentroid( 0 )
{
	FUGID( PIN_INPUT_FFT, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_OUTPUT_CENTROID, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );

	mPinInputFFT = pinInput( "FFT", PIN_INPUT_FFT );

	mValOutput = pinOutput<fugio::VariantInterface *>( "Centroid", mPinOutput, PID_FLOAT, PIN_OUTPUT_CENTROID );

	mPinInputFFT->setDescription( tr( "The frequency information calculated using a Fast Fourier Transform (FFT) node" ) );

	mPinOutput->setDescription( tr( "The centroid frequency - the most pronounced frequency in the FFT" ) );
}

void CentroidNode::inputsUpdated( qint64 pTimeStamp )
{
	if( mPinInputFFT->isUpdated( pTimeStamp ) )
	{
		fugio::FftInterface	*FFT = input<fugio::FftInterface *>( mPinInputFFT );

		if( !FFT || !FFT->fft() )
		{
			return;
		}

		const double	v = double( FFT->sampleRate() ) / double( FFT->samples() );

		double			u = 0;
		double			d = 0;

		for( int i = 0 ; i < FFT->samples() / 2 ; i++ )
		{
			double		x = FFT->fft()[ i * 2 ];

			x = sqrt( x * x );

			u += double( i ) * v * x;
			d += x;
		}

		if( d )
		{
			mCentroid = u / d;
		}
		else
		{
			mCentroid = 0;
		}
	}

	if( mCentroid != mValOutput->variant().toFloat() )
	{
		mValOutput->setVariant( mCentroid );

		pinUpdated( mPinOutput );
	}
}
