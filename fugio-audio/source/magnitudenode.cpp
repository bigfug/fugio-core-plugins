#include "magnitudenode.h"

#include <fugio/core/uuid.h>
#include <fugio/audio/uuid.h>

#include <fugio/context_interface.h>
#include <fugio/variant_interface.h>
#include <fugio/audio/fft_interface.h>
#include <fugio/audio/audio_producer_interface.h>

#include <fugio/context_signals.h>

#include <qmath.h>

MagnitudeNode::MagnitudeNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode ), mMagnitude( 0 ), mSamplePosition( 0 ), mProducerInstance( nullptr )
{
	FUGID( PIN_INPUT_AUDIO, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_SAMPLES, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_OUTPUT_MAGNITUDE, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );

	mPinAudio = pinInput( "Audio", PIN_INPUT_AUDIO );

	mPinSampleCount = pinInput( "Samples", PIN_INPUT_SAMPLES );

	mPinSampleCount->setValue( 48000/25 );

	mValOutput = pinOutput<fugio::VariantInterface *>( "Magnitude", mPinOutput, PID_FLOAT, PIN_OUTPUT_MAGNITUDE );

	mPinAudio->setDescription( tr( "The source audio signal" ) );

	mPinSampleCount->setDescription( tr( "The number of samples to count in each calculation" ) );

	mPinOutput->setDescription( tr( "The largest magnitude of the audio signal" ) );
}

bool MagnitudeNode::initialise()
{
	if( !fugio::NodeControlBase::initialise() )
	{
		return( false );
	}

	connect( mNode->context()->qobject(), SIGNAL(frameProcess(qint64)), this, SLOT(onContextProcess(qint64)) );

	return( true );
}

bool MagnitudeNode::deinitialise()
{
	disconnect( mNode->context()->qobject(), SIGNAL(frameProcess(qint64)), this, SLOT(onContextProcess(qint64)) );

	return( NodeControlBase::deinitialise() );
}

void MagnitudeNode::onContextProcess( qint64 pTimeStamp )
{
	const int		SampleCount = variant( mPinSampleCount ).toInt();

	if( SampleCount <= 0 )
	{
		return;
	}

	if( mProducerInstance && !mProducerInstance->isValid() )
	{
		delete mProducerInstance;

		mProducerInstance = nullptr;
	}

	fugio::AudioProducerInterface	*API = input<fugio::AudioProducerInterface *>( mPinAudio );

	if( !API )
	{
		if( mProducerInstance )
		{
			delete mProducerInstance;

			mProducerInstance = nullptr;

			mSamplePosition = 0;
		}

		return;
	}

	if( !mProducerInstance )
	{
		mProducerInstance = API->audioAllocInstance( 48000, fugio::AudioSampleFormat::Format32FS, 1 );
	}

	if( !mProducerInstance )
	{
		return;
	}

	qint64	CurPos = ( pTimeStamp - API->audioLatency() ) * ( 48000 / 1000 );

	if( !mSamplePosition )
	{
		mSamplePosition = CurPos;
	}

	if( CurPos - mSamplePosition >= SampleCount )
	{
		int			ChannelCount = 1;

		mAudDat.resize( ChannelCount );

		for( auto &V : mAudDat )
		{
			V.resize( SampleCount );
		}

		QVector<float *>	AudPtr;

		AudPtr.resize( ChannelCount );

		for( int i = 0 ; i < ChannelCount ; i++ )
		{
			AudPtr[ i ] = mAudDat[ i ].data();

			memset( AudPtr[ i ], 0, sizeof( float ) * SampleCount );
		}

		mProducerInstance->audio( mSamplePosition, SampleCount, 0, 1, (void **)AudPtr.data() );

		float		Mag = 0;

		for( int c = 0 ; c < ChannelCount ; c++ )
		{
			float	*S = AudPtr[ c ];

			for( int i = 0 ; i < SampleCount ; i++, S++ )
			{
				float	M = fabs( *S );

				if( M > Mag )
				{
					Mag = M;
				}
			}
		}

		mMagnitude = Mag;

		mSamplePosition += SampleCount;
	}

	if( mMagnitude != mValOutput->variant().toFloat() )
	{
		mValOutput->setVariant( mMagnitude );

		pinUpdated( mPinOutput );
	}
}
