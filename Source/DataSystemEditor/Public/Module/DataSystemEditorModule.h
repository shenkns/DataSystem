// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Modules/ModuleManager.h"

struct FAssetData;

DATASYSTEMEDITOR_API DECLARE_LOG_CATEGORY_EXTERN(LogDataSystemEditor, Log, All);

class DATASYSTEMEDITOR_API FDataSystemEditorModule : public IModuleInterface
{
public:

	static inline FDataSystemEditorModule& Get() { return FModuleManager::LoadModuleChecked<FDataSystemEditorModule>("DataSystemEditor"); }

public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

#if UE_EDITOR
	void PreSaveDataAsset(const FAssetData& Data);
	void OnRemovedDataAsset(const FAssetData& Data);
	
	void CheckDataAssetsTags();
#endif
	
private:

#if UE_EDITOR
	// Init Stats Configuration
	void RegisterSystemSettings() const;
	void UnregisterSystemSettings() const;
#endif


};
