// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Module/DataSystemModule.h"

#include "Module/DataSystemSettings.h"
#include "LogSystem.h"

#if UE_EDITOR
#include "ISettingsModule.h"
#endif

IMPLEMENT_MODULE(FDataSystemModule, DataSystem)

DATASYSTEM_API DEFINE_LOG_CATEGORY(LogDataSystem);

#if UE_EDITOR
void FDataSystemModule::PreSaveDataAsset(const FAssetData& Data)
{
	PreSaveDataAssetEvent.Broadcast(Data);
}
#endif

void FDataSystemModule::StartupModule()
{
#if UE_EDITOR
	RegisterSystemSettings();
#endif
}

void FDataSystemModule::ShutdownModule()
{
#if UE_EDITOR
	UnregisterSystemSettings();
#endif
}

#if UE_EDITOR
void FDataSystemModule::RegisterSystemSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project",
			"Plugins",
			"Data System",
			FText::FromString(TEXT("Data System")),
			FText::FromString(TEXT("Configuration settings for Data System")),
			GetMutableDefault<UDataSystemSettings>()
		);

		LOG_STATIC(LogDataSystem, "Data System Settings Registered")
	}
}

void FDataSystemModule::UnregisterSystemSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Data System");

		LOG_STATIC(LogDataSystem, "Data System Settings Unregistered")
	}
}
#endif