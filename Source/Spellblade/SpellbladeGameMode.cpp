// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Spellblade.h"
#include "SpellbladeGameMode.h"

ASpellbladeGameMode::ASpellbladeGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FObjectFinder<UClass> PlayerPawnBPClass(TEXT("Class'/Game/Blueprints/MyCharacter.MyCharacter_C'"));
	if (PlayerPawnBPClass.Object != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Object;
	}
}
