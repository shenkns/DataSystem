// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Modules/ModuleManager.h"

DATASYSTEM_API DECLARE_LOG_CATEGORY_EXTERN(LogDataSystem, Log, All);

class FDataSystemModule : public IModuleInterface
{
public:

#if UE_EDITOR
	DECLARE_EVENT_OneParam(FDataSystemModule, FDataAssetEvent, const FAssetData&);
	FDataAssetEvent& OnPreSaveDataAsset() { return PreSaveDataAssetEvent; };
#endif

public:

	static inline FDataSystemModule& Get() { return FModuleManager::LoadModuleChecked<FDataSystemModule>("DataSystem"); };

	virtual bool SupportsDynamicReloading() override { return true; };

#if UE_EDITOR
	void PreSaveDataAsset(const FAssetData& Data);
#endif
	
protected:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

#if UE_EDITOR
	// Init ProfileSystem Configuration
	void RegisterSystemSettings() const;
	void UnregisterSystemSettings() const;
#endif

private:

#if UE_EDITOR
	FDataAssetEvent PreSaveDataAssetEvent;
#endif
};
