// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Managers/Manager.h"

#include "Data/Data.h"

#include "DataManager.generated.h"

UCLASS()
class DATASYSTEM_API UDataManager : public UManager
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TArray<UData*> DataAssets;

public:

	virtual void InitManager() override;

	// Profile Data Getters
	UFUNCTION(BlueprintPure, Category = "Profile|Data", meta = (CompactNodeTitle = "DataAssets", DeterminesOutputType = "Class"))
	TArray<UData*> GetDataAssets(TSubclassOf<UData> Class) const;

	UFUNCTION(BlueprintPure, Category = "Profile|Data", meta = (CompactNodeTitle = "DataAsset", DeterminesOutputType = "Class"))
	UData* GetDataAsset(FName Tag, TSubclassOf<UData> Class) const;

	template<typename T>
	TArray<T*> GetDataAssets() const;

	template<typename T>
	T* GetDataAsset(FName Tag) const;

private:

	void FindDataAssets();
};

template <typename T>
TArray<T*> UDataManager::GetDataAssets() const
{
	// Filter DataAssets
	TArray<UData*> Out = DataAssets.FilterByPredicate([&](const UData* Src)
	{
		return Src && Src->GetClass()->IsChildOf(T::StaticClass());	
	});

	return reinterpret_cast<TArray<T*>&>(Out);
}

template <typename T>
T* UDataManager::GetDataAsset(FName Tag) const
{
	// Find DataAsset
	UData* const* Out = DataAssets.FindByPredicate([Tag](const UData* Src)
	{
		return Src && Src->GetClass()->IsChildOf(T::StaticClass()) && Src->Tag == Tag;
	});

	return Out ? static_cast<T*>(*Out) : nullptr;
}
