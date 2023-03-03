// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Data/Data.h"

#include "UObject/ObjectSaveContext.h"
#include "Module/DataSystemModule.h"
#include "Data/CustomData.h"

UCustomData* UData::GetCustomData(TSubclassOf<UCustomData> Class) const
{
	if(!Class) Class = UCustomData::StaticClass();
	
	UCustomData* const* Out = CustomData.FindByPredicate([Class](const UCustomData* Src)
	{
		return Src && Src->GetClass()->IsChildOf(Class);
	});

	return Out ? *Out : nullptr;
}

TArray<UCustomData*> UData::GetCustomDataArray(TSubclassOf<UCustomData> Class) const
{
	if(!Class) Class = UCustomData::StaticClass();

	return CustomData.FilterByPredicate([Class](const UCustomData* Src)
	{
		return Src && Src->GetClass()->IsChildOf(Class);
	});
}

// Detect Unique Tags And Update Icon
#if UE_EDITOR
void UData::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	if (!GIsCookerLoadingPackage && !IsRunningCommandlet())
	{
		const FAssetData Data(this);
		FDataSystemModule::Get().PreSaveDataAsset(Data);
	}
}
#endif