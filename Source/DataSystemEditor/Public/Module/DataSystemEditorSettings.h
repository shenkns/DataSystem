// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "UObject/Object.h"

#include "UObject/SoftObjectPtr.h"

#include "DataSystemEditorSettings.generated.h"

class UData;

DATASYSTEMEDITOR_API DECLARE_LOG_CATEGORY_EXTERN(LogDataSystemEditorSettings, Log, All);

UCLASS(Config=Editor, DefaultConfig)
class DATASYSTEMEDITOR_API UDataSystemEditorSettings : public UObject
{
	GENERATED_BODY()

public:

	// Configuration For Data System Editor
	
	UPROPERTY(Config, VisibleDefaultsOnly, Category = "Data")
	TMap<FName, TSoftObjectPtr<UData>> DataTags;

public:

#if UE_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
