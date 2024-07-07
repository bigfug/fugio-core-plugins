#include "ringmodulatornode.h"

#include <fugio/core/uuid.h>
#include <fugio/audio/uuid.h>

#include <fugio/context_interface.h>
#include <fugio/context_signals.h>
#include <fugio/pin_signals.h>

#include <qmath.h>

RingModulatorNode::RingModulatorNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	FUGID( PIN_INPUT_AUDIO, "9e154e12-bcd8-4ead-95b1-5a59833bcf4e" );
	FUGID( PIN_INPUT_SIGNAL, "1b5e9ce8-acb9-478d-b84b-9288ab3c42f5" );
	FUGID( PIN_OUTPUT_AUDIO, "261cc653-d7fa-4c34-a08b-3603e8ae71d5" );

	mPinAudioInput1  = pinInput( "Audio", PIN_INPUT_AUDIO );
	mPinAudioInput2  = pinInput( "Signal", PIN_INPUT_SIGNAL );

	mAudioOutput = pinOutput<fugio::AudioProducerInterface *>( "Audio", mPinAudioOutput, PID_AUDIO, PIN_OUTPUT_AUDIO );
}

bool RingModulatorNode::initialise()
{
	if( !NodeControlBase::initialise() )
	{
		return( false );
	}

	connect( mPinAudioInput1->qobject(), SIGNAL(linked(QSharedPointer<fugio::PinInterface>)), this, SLOT(audio1Linked(QSharedPointer<fugio::PinInterface>)) );
	connect( mPinAudioInput1->qobject(), SIGNAL(unlinked(QSharedPointer<fugio::PinInterface>)), this, SLOT(audio1Unlinked(QSharedPointer<fugio::PinInterface>)) );

	connect( mPinAudioInput2->qobject(), SIGNAL(linked(QSharedPointer<fugio::PinInterface>)), this, SLOT(audio2Linked(QSharedPointer<fugio::PinInterface>)) );
	connect( mPinAudioInput2->qobject(), SIGNAL(unlinked(QSharedPointer<fugio::PinInterface>)), this, SLOT(audio2Unlinked(QSharedPointer<fugio::PinInterface>)) );

	mInstanceDataMutex.lock();

	for( AudioInstanceData *AID : mInstanceData )
	{
		AID->setEnabed( true );
	}

	mInstanceDataMutex.unlock();

	return( true );
}

bool RingModulatorNode::deinitialise()
{
	disconnect( mPinAudioInput1->qobject(), SIGNAL(linked(QSharedPointer<fugio::PinInterface>)), this, SLOT(audio1Linked(QSharedPointer<fugio::PinInterface>)) );
	disconnect( mPinAudioInput1->qobject(), SIGNAL(unlinked(QSharedPointer<fugio::PinInterface>)), this, SLOT(audio1Unlinked(QSharedPointer<fugio::PinInterface>)) );

	disconnect( mPinAudioInput2->qobject(), SIGNAL(linked(QSharedPointer<fugio::PinInterface>)), this, SLOT(audio2Linked(QSharedPointer<fugio::PinInterface>)) );
	disconnect( mPinAudioInput2->qobject(), SIGNAL(unlinked(QSharedPointer<fugio::PinInterface>)), this, SLOT(audio2Unlinked(QSharedPointer<fugio::PinInterface>)) );

	mInstanceDataMutex.lock();

	for( AudioInstanceData *AID : mInstanceData )
	{
		AID->setEnabed( false );
	}

	mInstanceDataMutex.unlock();

	return( NodeControlBase::deinitialise() );
}

void RingModulatorNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );
}

fugio::AudioInstanceBase *RingModulatorNode::audioAllocInstance( qreal pSampleRate, fugio::AudioSampleFormat pSampleFormat, int pChannels )
{
	AudioInstanceData		*InsDat = new AudioInstanceData( qSharedPointerDynamicCast<fugio::AudioProducerInterface>( mNode->control() ), pSampleRate, pSampleFormat, pChannels );

	if( InsDat )
	{
		InsDat->mAudioIns1 = 0;
		InsDat->mAudioIns2 = 0;

		InsDat->mAudPts = 0;
		InsDat->mAudSmp = 0;

		InsDat->mAudioDat1.resize( pChannels );
		InsDat->mAudioDat2.resize( 1 );

		if( fugio::AudioProducerInterface *IAP = input<fugio::AudioProducerInterface *>( mPinAudioInput1 ) )
		{
			InsDat->mAudioIns1 = IAP->audioAllocInstance( pSampleRate, pSampleFormat, pChannels );
		}

		if( fugio::AudioProducerInterface *IAP = input<fugio::AudioProducerInterface *>( mPinAudioInput2 ) )
		{
			InsDat->mAudioIns2 = IAP->audioAllocInstance( pSampleRate, pSampleFormat, 1 );
		}

		mInstanceDataMutex.lock();

		mInstanceData.append( InsDat );

		mInstanceDataMutex.unlock();
	}

	return( InsDat );
}

void RingModulatorNode::audio1Linked( QSharedPointer<fugio::PinInterface> pPin )
{
	if( !pPin->hasControl() )
	{
		return;
	}

	fugio::AudioProducerInterface *IAP = qobject_cast<fugio::AudioProducerInterface *>( pPin->control()->qobject() );

	if( !IAP )
	{
		return;
	}

	QMutexLocker	MutLck( &mInstanceDataMutex );

	for( AudioInstanceData *AID : mInstanceData )
	{
		AID->mAudioIns1 = IAP->audioAllocInstance( AID->sampleRate(), AID->sampleFormat(), IAP->audioChannels() );
	}
}

void RingModulatorNode::audio1Unlinked(QSharedPointer<fugio::PinInterface> pPin)
{
	if( !pPin->hasControl() )
	{
		return;
	}

	fugio::AudioProducerInterface *IAP = qobject_cast<fugio::AudioProducerInterface *>( pPin->control()->qobject() );

	if( !IAP )
	{
		return;
	}

	QMutexLocker	MutLck( &mInstanceDataMutex );

	for( AudioInstanceData *AID : mInstanceData )
	{
		if( AID->mAudioIns1 )
		{
			delete AID->mAudioIns1;

			AID->mAudioIns1 = nullptr;
		}
	}
}

void RingModulatorNode::audio2Linked( QSharedPointer<fugio::PinInterface> pPin )
{
	if( !pPin->hasControl() )
	{
		return;
	}

	fugio::AudioProducerInterface *IAP = qobject_cast<fugio::AudioProducerInterface *>( pPin->control()->qobject() );

	if( !IAP )
	{
		return;
	}

	QMutexLocker	MutLck( &mInstanceDataMutex );

	for( AudioInstanceData *AID : mInstanceData )
	{
		AID->mAudioIns2 = IAP->audioAllocInstance( AID->sampleRate(), AID->sampleFormat(), 1 );
	}
}

void RingModulatorNode::audio2Unlinked(QSharedPointer<fugio::PinInterface> pPin)
{
	if( !pPin->hasControl() )
	{
		return;
	}

	fugio::AudioProducerInterface *IAP = qobject_cast<fugio::AudioProducerInterface *>( pPin->control()->qobject() );

	if( !IAP )
	{
		return;
	}

	QMutexLocker	MutLck( &mInstanceDataMutex );

	for( AudioInstanceData *AID : mInstanceData )
	{
		if( AID->mAudioIns2 )
		{
			delete AID->mAudioIns2;

			AID->mAudioIns2 = nullptr;
		}
	}
}

void RingModulatorNode::audio( qint64 pSamplePosition, qint64 pSampleCount, int pChannelOffset, int pChannelCount, void **pBuffers, AudioInstanceData *pInstanceData ) const
{
	if( pInstanceData->mAudioIns1 )
	{
		pInstanceData->mAudioIns1->audio( pSamplePosition, pSampleCount, pChannelOffset, pChannelCount, pBuffers );
	}
	else
	{
		return;
	}

	if( pInstanceData->mAudioIns2 )
	{
		pInstanceData->mAudioDat2.resize( pSampleCount );

		float		*SmpPtr = pInstanceData->mAudioDat2.data();

		memset( SmpPtr, 0, sizeof( float ) * pSampleCount );

		pInstanceData->mAudioIns2->audio( pSamplePosition, pSampleCount, 0, 1, (void **)&SmpPtr  );
	}
	else
	{
		pInstanceData->mAudioDat2.clear();
	}

	if( !pInstanceData->mAudioDat2.isEmpty() )
	{
		for( int c = pChannelOffset ; c < pChannelCount ; c++ )
		{
			float				*AudDst = reinterpret_cast<float **>( pBuffers )[ c ];

			for( qint64 i = 0 ; i < pSampleCount ; i++ )
			{
				AudDst[ i ] *= pInstanceData->mAudioDat2[ i ];
			}
		}
	}
}

int RingModulatorNode::audioChannels() const
{
	fugio::AudioProducerInterface *IAP = input<fugio::AudioProducerInterface *>( mPinAudioInput1 );

	return( IAP ? IAP->audioChannels() : 0 );
}

qreal RingModulatorNode::audioSampleRate() const
{
	return( 48000 );
}

fugio::AudioSampleFormat RingModulatorNode::audioSampleFormat() const
{
	return( fugio::AudioSampleFormat::Format32FS );
}

qint64 RingModulatorNode::audioLatency() const
{
	fugio::AudioProducerInterface *IAP = input<fugio::AudioProducerInterface *>( mPinAudioInput1 );

	return( IAP ? IAP->audioLatency() : 0 );
}
