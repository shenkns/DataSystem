// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Managers/DataManager.h"

#include "Log.h"
#include "ManagersSystem.h"
#include "Module/DataSystemSettings.h"
#include "Engine/AssetManager.h"
#include "Module/DataSystemModule.h"

DEFINE_LOG_CATEGORY_LOCAL(LogDataSystem);

void UDataManager::InitManager()
{
	Super::InitManager();

	FindDataAssets();
}

void UDataManager::FindDataAssets()
{
	const UDataSystemSettings* Settings = GetDefault<UDataSystemSettings>();
	if(!Settings) return;
	
	DataAssets.Empty();

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	AssetRegistry.ScanPathsSynchronous(Settings->DataAssetsPaths);

	FARFilter Filter;
	Filter.bRecursiveClasses = true;

	for (const FString& Path : Settings->DataAssetsPaths)
	{
		if(AssetRegistry.PathExists(FName(*Path)))
		{
			Filter.PackagePaths.Add(FName(*Path));
		}
	}
	
	Filter.bRecursivePaths = true;

	TArray<FAssetData> AssetDataArray;
	AssetRegistryModule.Get().GetAssets(Filter, AssetDataArray);
	
	for (const FAssetData& Data : AssetDataArray)
	{
		if (!IsValid(Data.GetAsset())) continue;
		if (Data.IsAssetLoaded())
		{
			if(UData* DataAsset = Cast<UData>(Data.GetAsset()))
			{
				DataAssets.Add(DataAsset);
			}
		}
	}

	LOG(Display, "{} Data Assets Browsed", DataAssets.Num());
}

TArray<UData*> UDataManager::GetDataAssets(TSubclassOf<UData> Class) const
{
	if(!Class) Class = UData::StaticClass();

	return DataAssets.FilterByPredicate([Class](const UData* Src)
	{
		return Src && Src->GetClass()->IsChildOf(Class);
	});
}

UData* UDataManager::GetDataAsset(FName Tag, TSubclassOf<UData> Class) const
{
	if(!Class) Class = UData::StaticClass();

	UData* const* Out = DataAssets.FindByPredicate([Tag, Class](const UData* Src)
	{
		return Src && Src->GetClass()->IsChildOf(Class) && Src->Tag == Tag;
	});

	return Out ? *Out : nullptr;
}
