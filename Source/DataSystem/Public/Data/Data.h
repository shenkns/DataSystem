// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Engine/DataAsset.h"

#include "Data/CustomData.h"

#include "Data.generated.h"

class UTexture2D;

UCLASS()
class DATASYSTEM_API UData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// DataAsset Configuration
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FName Tag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Data")
	TArray<UCustomData*> CustomData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UTexture2D* Icon;

public:

	// CustomData Getters
	UFUNCTION(BlueprintPure, Category = "Data", meta = (CompactNodeTitle = "CustomData", DeterminesOutputType = "Class"))
	UCustomData* GetCustomData(TSubclassOf<UCustomData> Class) const;

	UFUNCTION(BlueprintPure, Category = "Data", meta = (CompactNodeTitle = "CustomData", DeterminesOutputType = "Class"))
	TArray<UCustomData*> GetCustomDataArray(TSubclassOf<UCustomData> Class) const;

	template<typename T>
	T* GetCustomData() const;

	template<typename T>
	TArray<T*> GetCustomDataArray() const;

#if UE_EDITOR
	virtual void PreSave(const class ITargetPlatform* TargetPlatform) override;
#endif
};

template <typename T>
T* UData::GetCustomData() const
{
	// Find CustomData
	UCustomData* const* Out = CustomData.FindByPredicate([&](const UCustomData* Src)
	{
		return Src && Src->GetClass()->IsChildOf(T::StaticClass());
	});

	return Out ? static_cast<T*>(*Out) : nullptr;
}

template <typename T>
TArray<T*> UData::GetCustomDataArray() const
{
	// Filter CustomData
	const TArray<UCustomData*> Out = CustomData.FilterByPredicate([&](const UCustomData* Src)
	{
		return Src && Src->GetClass()->IsChildOf(T::StaticClass());
	});
	
	

	return reinterpret_cast<TArray<T*>&>(Out);
}
