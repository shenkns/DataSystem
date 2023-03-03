// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "UObject/Object.h"

#include "DataSystemSettings.generated.h"

UCLASS(Config=Game, DefaultConfig)
class DATASYSTEM_API UDataSystemSettings : public UObject
{
	GENERATED_BODY()

public:

	// Configuration For Data System
	
	UPROPERTY(Config, EditDefaultsOnly, Category = "Data")
	TArray<FString> DataAssetsPaths = { "/Game" };

	// Debug
	UPROPERTY(Config, EditDefaultsOnly, Category = "Debug")
	bool bShowDebugMessages;
};
