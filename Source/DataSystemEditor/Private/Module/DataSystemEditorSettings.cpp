// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Module/DataSystemEditorSettings.h"

#include "Log.h"
#include "Data/Data.h"
#include "CoreUObject/Public/UObject/SavePackage.h"
#include "Log/Details/LocalLogCategory.h"

DATASYSTEMEDITOR_API DEFINE_LOG_CATEGORY(LogDataSystemEditorSettings);
DEFINE_LOG_CATEGORY_LOCAL(LogDataSystemEditorSettings);

#if UE_EDITOR
void UDataSystemEditorSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (const FMapProperty* MapProp = CastField<FMapProperty>(PropertyChangedEvent.MemberProperty))
	{
		LOG(Display, "CPPType {}", *MapProp->GetCPPType(nullptr, 0));

		const int32 Idx = PropertyChangedEvent.GetArrayIndex("AssetTagsMap_Key");
		if (Idx >= 0)
		{
			FScriptMapHelper ScriptMapHelper(MapProp, &DataTags);
			
			if (const TTuple<FName, TSoftObjectPtr<UData>>* PairPtr = reinterpret_cast<TTuple<FName, TSoftObjectPtr<UData>>*>(ScriptMapHelper.GetPairPtr(Idx)))
			{
				const TTuple<FName, TSoftObjectPtr<UData>>& Pair = *PairPtr;
				LOG(Display, "ChangeName {}", *Pair.Key.ToString());
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
						LOG(Display, "Saved Package: {}", *FilePath);
					}
					else
					{
						LOG(Error, "Saved Package FAILED!: {}", *FilePath);
					}
				}
			}
		}
	}
}
#endif
