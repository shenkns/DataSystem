// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Module/DataSystemEditorModule.h"

#include "Log.h"

#if UE_EDITOR
#include "ISettingsModule.h"
#include "ImageUtils.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Module/DataSystemEditorSettings.h"
#include "Module/DataSystemSettings.h"
#include "Module/DataSystemModule.h"
#include "Data/Data.h"
#endif

IMPLEMENT_MODULE(FDataSystemEditorModule, DataSystemEditor)

DATASYSTEMEDITOR_API DEFINE_LOG_CATEGORY(LogDataSystemEditor);
DEFINE_LOG_CATEGORY_LOCAL(LogDataSystemEditor);

#if UE_EDITOR
void FDataSystemEditorModule::PreSaveDataAsset(const FAssetData& Data)
{
	if(UData* DataAsset = Cast<UData>(Data.GetAsset()))
	{
		/*if (UDataSystemEditorSettings* Settings = GetMutableDefault<UDataSystemEditorSettings>())
		{
			auto& Map = Settings->DataTags;
			const TSoftObjectPtr<UData> SavedSoft = TSoftObjectPtr<UData>(Data.ToSoftObjectPath());

			FName CurrentName = DataAsset->Tag;
			const FName* OldNamePtr = Map.FindKey(SavedSoft);
			if (OldNamePtr && *OldNamePtr != CurrentName)
			{
				Map.Remove(*OldNamePtr);
			}

			LOG_STATIC(LogDataSystemEditor, "%s Data Asset Old Tag Removed From Tags Data", *Data.AssetName.ToString())
			
			TSoftObjectPtr<UData>& Asset = Map.FindOrAdd(CurrentName);

			// Generate New Unique Tag
			if (Asset.IsValid() && CurrentName != NAME_None)
			{
				if (Asset != SavedSoft)
				{
					FString TagStr = CurrentName.ToString();
					FString L, R;

					TagStr.Split(TEXT("_Unique_"), &L, &R);
					
					if (R.Len() > 0)
					{
						check(R.IsNumeric());
						uint32 Number;
						LexFromString(Number, *R);
						Number++;
						TagStr = FString::Printf(TEXT("%s_Unique_%u"), *L, Number);
					}
					else
					{
						TagStr = FString::Printf(TEXT("%s_Unique_0"), *TagStr);
					}
					DataAsset->Tag = FName(TagStr);
					Settings->DataTags.KeySort([](const FName& A, const FName& B) { return A.LexicalLess(B); });
					Settings->PostEditChange();
					Settings->SaveConfig();

					LOG_STATIC(LogDataSystemEditor, "%s Data Asset Duplicating Tag Uniqued", *Data.AssetName.ToString())
				}
			}
			else
			{
				Asset = SavedSoft;
				Settings->DataTags.KeySort([](const FName& A, const FName& B) { return A.LexicalLess(B); });
				Settings->PostEditChange();
				Settings->SaveConfig();

				LOG_STATIC(LogDataSystemEditor, "%s Data Asset New Tag Added To Tags Data", *Data.AssetName.ToString())
			}
		}*/

		// Update Icon
		if (UTexture2D* Texture = DataAsset->Icon)
		{
			const int32 SrcWidth = Texture->GetSizeX();
			const int32 SrcHeight = Texture->GetSizeY();
			constexpr int32 DstWidth = ThumbnailTools::DefaultThumbnailSize;
			constexpr int32 DstHeight = ThumbnailTools::DefaultThumbnailSize;

			constexpr FColor BackGround = FColor(75, 29, 44, 255);
			FObjectThumbnail NewThumbnail;

			{
				TArray<uint8>& RawFileData = NewThumbnail.AccessImageData();
				RawFileData.Init(0, SrcHeight * SrcWidth * 4);

				const uint8* MipData = Texture->Source.LockMip(0);
				uint8* DestPtr = &RawFileData[0];
				for (int32 y = 0; y < SrcHeight; y++)
				{
					const uint8* SrcPtr = &MipData[y * SrcWidth * 4];
					for (int32 x = 0; x < SrcWidth; x++)
					{
						const FColor* Color = reinterpret_cast<const FColor*>(SrcPtr);
						if (Color->A > 0)
						{
							*DestPtr++ = *SrcPtr++;
							*DestPtr++ = *SrcPtr++;
							*DestPtr++ = *SrcPtr++;
							*DestPtr++ = 255;
							*SrcPtr++;
						}
						else
						{
							*DestPtr++ = BackGround.B;
							*DestPtr++ = BackGround.G;
							*DestPtr++ = BackGround.R;
							*DestPtr++ = 255;
							SrcPtr += 4;
						}
					}
				}

				Texture->Source.UnlockMip(0);

				const TArrayView<const FColor> SrcColor = TArrayView<const FColor>(reinterpret_cast<const FColor*>(&RawFileData[0]), SrcWidth * SrcHeight);
				TArray<uint8> DstData;
				DstData.Init(0, DstWidth * DstHeight * 4);
				const TArrayView<FColor> D = TArrayView<FColor>(reinterpret_cast<FColor*>(&DstData[0]), DstHeight * DstWidth);


				FImageUtils::ImageResize(SrcWidth, SrcHeight, SrcColor, DstHeight, DstWidth, D, false);

				RawFileData = DstData;
				NewThumbnail.SetImageSize(DstWidth, DstHeight);
			}

			LOG(Display, "{} Data Asset New Thumbnail Generated And Resized", *Data.AssetName.ToString());

			//Assign The Thumbnail And Dirty
			UPackage* AssetPackage = DataAsset->GetOutermost();
			if (ensure(AssetPackage))
			{
				UPackage* OutermostPackage = DataAsset->GetOutermost();
				FObjectThumbnail* Thumbnail = ThumbnailTools::CacheThumbnail(DataAsset->GetFullName(), &NewThumbnail, OutermostPackage);

				LOG(Display, "{} Data Asset Thumbnail Cached", *Data.AssetName.ToString());
				
				if (ensure(Thumbnail))
				{
					// Let The Content Browser Know That We Have Changed The Thumbnail
					Thumbnail->MarkAsDirty();

					// Signal That The Asset Was Changed If It Is Loaded So Thumbnail Pools Will Update
					if (Data.IsAssetLoaded())
					{
						Data.GetAsset()->PostEditChange();
					}

					//Set That Thumbnail As A Valid Custom Thumbnail So It Will Be Saved Out
					Thumbnail->SetCreatedAfterCustomThumbsEnabled();

					LOG(Display, "{} Data Asset Thumbnail Updated", *Data.AssetName.ToString());
				}
			}
		}
	}
}

void FDataSystemEditorModule::OnRemovedDataAsset(const FAssetData& Data)
{
	if (Cast<UData>(Data.GetAsset()))
	{
		if (UDataSystemEditorSettings* Settings = GetMutableDefault<UDataSystemEditorSettings>())
		{
			// Generate Soft Reference
			TMap<FName, TSoftObjectPtr<UData>>& Map = Settings->DataTags;
			const TSoftObjectPtr<UData> SavedSoft = TSoftObjectPtr<UData>(Data.ToSoftObjectPath());

			// Remove From Config
			if (const FName* OldNamePtr = Map.FindKey(SavedSoft))
			{
				Map.Remove(*OldNamePtr);

				Settings->PostEditChange();
				Settings->SaveConfig();

				LOG(Display, "{} Data Asset Removed From Tags Data And Saved", *Data.AssetName.ToString());
			}
		}

		LOG(Display, "{} Data Asset Removed And Cleared Tags Data", *Data.AssetName.ToString());
	}
}

void FDataSystemEditorModule::StartupModule()
{
#if UE_EDITOR
	RegisterSystemSettings();
#endif
	
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	AssetRegistryModule.Get().OnAssetRemoved().AddRaw(this, &FDataSystemEditorModule::OnRemovedDataAsset);
	
	FDataSystemModule::Get().OnPreSaveDataAsset().AddRaw(this, &FDataSystemEditorModule::PreSaveDataAsset);
	
	LOG(Display, "Data Assets Changes Binded");

	//CheckDataAssetsTags();
}

void FDataSystemEditorModule::ShutdownModule()
{
#if UE_EDITOR
	UnregisterSystemSettings();
#endif
}

void FDataSystemEditorModule::CheckDataAssetsTags()
{
	UDataSystemEditorSettings* EditorSettings = GetMutableDefault<UDataSystemEditorSettings>();
	UDataSystemSettings* Settings = GetMutableDefault<UDataSystemSettings>();
	if(!EditorSettings || !Settings) return;
	
	TArray<UData*> DataAssets;

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	AssetRegistry.ScanPathsSynchronous(Settings->DataAssetsPaths);

	FARFilter Filter;
	Filter.bRecursiveClasses = true;
	Filter.PackagePaths.Add(*UData::StaticClass()->GetPathName());

	for (const FString& Path : Settings->DataAssetsPaths)
	{
		Filter.PackagePaths.Add(FName(*Path));
	}
	Filter.bRecursivePaths = true;

	TArray<FAssetData> AssetDataArray;
	AssetRegistryModule.Get().GetAssets(Filter, AssetDataArray);
	
	LOG(Display, "{} Data Assets Found In Pathes", AssetDataArray.Num());
	
	TMap<FName, TSoftObjectPtr<UData>>& DataTags = EditorSettings->DataTags;
	
	TArray<FName> DataKeys;
	DataTags.GenerateKeyArray(DataKeys);

	for(int i = DataTags.Num() - 1; i >= 0; i--)
	{
		const UData* DataAsset = DataTags.Find(DataKeys[i])->LoadSynchronous();
		if(!IsValid(DataAsset))
		{
			DataTags.Remove(DataKeys[i]);
		}
	}
	
	for (const FAssetData& Data : AssetDataArray)
	{
		const UData* DataAsset = Cast<UData>(Data.GetAsset());
		
		if (!IsValid(DataAsset)) continue;
		if (Data.IsAssetLoaded())
		{
			TSoftObjectPtr<UData> SoftObject = TSoftObjectPtr<UData>(Data.ToSoftObjectPath());
			
			if(FName const* SavedTag = DataTags.FindKey(SoftObject))
			{
				if(DataAsset->Tag == *SavedTag) continue;

				PreSaveDataAsset(Data);

				LOG(Display, "{} Data Asset Tag Updated Or Already Used", *Data.AssetName.ToString());
			}

			*DataTags.Add(DataAsset->Tag, SoftObject);

			LOG(Display, "{} Data Asset Added To Data Tags And Saved", *Data.AssetName.ToString());
		}
	}
}

void FDataSystemEditorModule::RegisterSystemSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project",
			"Plugins",
			"Data System Editor",
			FText::FromString(TEXT("Data System Editor")),
			FText::FromString(TEXT("Configuration settings for Data System Editor")),
			GetMutableDefault<UDataSystemEditorSettings>()
		);

		LOG(Display, "Save-Load System Settings Registered");
	}
}

void FDataSystemEditorModule::UnregisterSystemSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Data System Editor");

		LOG(Display, "Data System Editor Settings Unregistered");
	}
}
#endif
