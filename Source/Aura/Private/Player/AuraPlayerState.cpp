// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

AAuraPlayerState::AAuraPlayerState()
{
	// How often server will try to update clients. This class is authored at server and replicated to clients.
	// Needs a high frequency to properly update attributes  
	SetNetUpdateFrequency(100.f);
}
