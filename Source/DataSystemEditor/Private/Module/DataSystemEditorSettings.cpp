// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Module/DataSystemEditorSettings.h"

#include "Data/Data.h"
#include "LogSystem.h"
#include "CoreUObject/Public/UObject/SavePackage.h"

DATASYSTEMEDITOR_API DEFINE_LOG_CATEGORY(LogDataSystemEditorSettings);

#if UE_EDITOR
void UDataSystemEditorSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (const FMapProperty* MapProp = CastField<FMapProperty>(PropertyChangedEvent.MemberProperty))
	{
		LOG(LogDataSystemEditorSettings, "CPPType %s", *MapProp->GetCPPType(nullptr, 0));

		const int32 Idx = PropertyChangedEvent.GetArrayIndex("AssetTagsMap_Key");
		if (Idx >= 0)
		{
			FScriptMapHelper ScriptMapHelper(MapProp, &DataTags);
			
			if (const TTuple<FName, TSoftObjectPtr<UData>>* PairPtr = reinterpret_cast<TTuple<FName, TSoftObjectPtr<UData>>*>(ScriptMapHelper.GetPairPtr(Idx)))
			{
				const TTuple<FName, TSoftObjectPtr<UData>>& Pair = *PairPtr;
				LOG(LogDataSystemEditorSettings, "ChangeName %s", *Pair.Key.ToString());
				const TSoftObjectPtr<UData> AssetSoft = Pair.Value;
				if (UData* Asset = AssetSoft.LoadSynchronous())
				{
					DataTags.KeySort([](const FName& A, const FName& B) { return A.LexicalLess(B); });

					UPackage* Outermost = Asset->GetOutermost();

					Asset->Tag = Pair.Key;
					Asset->PostEditChange();
					Asset->MarkPackageDirty();
					Outermost->SetDirtyFlag(true);

					const FString FilePath = *FPackageName::GetLongPackagePath(Outermost->GetName());

					if (UPackage::SavePackage(Outermost, Asset, Asset->GetFlags(), *FilePath))
					{
						LOG(LogDataSystemEditorSettings, "Saved Package: %s", *FilePath);
					}
					else
					{
						LOG_ERROR(LogDataSystemEditorSettings, "Saved Package FAILED!: %s", *FilePath);
					}
				}
			}
		}
	}
}
#endif
