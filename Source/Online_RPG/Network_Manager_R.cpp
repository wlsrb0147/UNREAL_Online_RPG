// Fill out your copyright notice in the Description page of Project Settings.

#include "Network_Manager_R.h"

#include "InventoryComponent.h"
#include "AItemManager.h"
#include "InventoryHUD.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "LoginController.h"
#include "Networking.h"
#include "PostLoadAsset.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Sound_Manager_R.h"
#include "WIdget_Login.h"
#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "PlayerCharacter.h"


const char* host = "localhost";
const char* user = "red";
const char* pw = "red";
const char* db = "mydb";

UNetwork_Manager_R::UNetwork_Manager_R()
{
	
	
	//SelectUser(TEXT("fffl"),TEXT("fppp1"));

	//InsertUser(TEXT("RED77"), TEXT("RED77"), TEXT("REDRED77"));
	//ULevelSequence* LevelSequence = LoadObject<ULevelSequence>(nullptr, TEXT("/Game/Path/To/YourSequence.YourSequence"));

	Level_Sequence = LoadObject<ULevelSequence>(nullptr, TEXT("/Game/MAIN/Sequence/SQ_GameStart.SQ_GameStart"));
	
	if (Level_Sequence == nullptr)
	{
		//////UE_LOG(LogTemp, Error, TEXT("Nope11"));
		// 에셋을 찾지 못한 경우 에러 처리
		return;
	}
	
}


ASound_Manager_R* UNetwork_Manager_R::Get_Sound_Instance()
{
	FActorSpawnParameters SpawnParams;
	if (!Sound_Instance)
		Sound_Instance = GetWorld()->SpawnActor<ASound_Manager_R>(Sound_Class, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);

	return Sound_Instance;
}

AItemManager* UNetwork_Manager_R::GetItemManager()
{
	FActorSpawnParameters SpawnParams;
	if (!ItemManagerInstance)
		ItemManagerInstance = GetWorld()->SpawnActor<AItemManager>(ItemManagerInstance_Class, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);

	return ItemManagerInstance;
}


void UNetwork_Manager_R::LoadStartAsset()
{
	//UE_LOG(LogTemp, Error, TEXT("LoadStartAsset"));
    // 사운드 에셋 비동기 로드
    //FPrimaryAssetId SoundAssetId("PostLoadAsset", TEXT("BP_PostLoadAsset"));
    //FPrimaryAssetId SoundAssetId("UPostLoadAsset", TEXT("Default__BP_PostLoadAsset_C"));
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    // FPrimaryAssetId에서 FSoftObjectPath 배열을 얻어내는 과정
    //TArray<FSoftObjectPath> AssetPaths;
    //FSoftObjectPath AssetPath = UAssetManager::Get().GetPrimaryAssetPath(SoundAssetId);

    TArray<FPrimaryAssetId> OutAssets;
    UAssetManager::Get().GetPrimaryAssetIdList("PostLoadAsset", OutAssets);
	//UAssetManager* AssetManager = UAssetManager::GetIfValid();
    if (OutAssets.IsEmpty()) {
        //UE_LOG(LogTemp, Warning, TEXT("OutAssets Empty"));
    }
    TArray<FSoftObjectPath> AssetPaths;
    for (FPrimaryAssetId Id : OutAssets) {
        ////UE_LOG(LogTemp, Warning, TEXT("OutAssets 33333"));
        FSoftObjectPath AssetPath = UAssetManager::Get().GetPrimaryAssetPath(Id);
        if (!AssetPath.IsValid())
        {
            //UE_LOG(LogTemp, Error, TEXT("AssetPath is not valid."));
        }
        else
        {
            //UE_LOG(LogTemp, Error, TEXT("AssetPath is  valid. 777 %s"), *AssetPath.ToString());
        }
        AssetPaths.Add(AssetPath);
    }
//AssetPaths.Add(AssetPath);
//
	// 비동기 로드 콜백
	auto OnLoadComplete = [this, AssetPaths]()
	{
		//UPostLoadAsset* SoundAsset = Cast<UPostLoadAsset>(LoadedAsset);
		//UE_LOG(LogTemp, Warning, TEXT("UPostLoadAsset Load 성공"));
	
		for (const FSoftObjectPath& AssetPath : AssetPaths)
		{
			FPrimaryAssetId MyAssetId = UAssetManager::Get().GetPrimaryAssetIdForPath(AssetPath);
			if(UAssetManager::Get().GetPrimaryAssetObject(MyAssetId))
			{
				//UE_LOG(LogTemp, Warning, TEXT("nullptr 아님 "));
			}else
			{
				//UE_LOG(LogTemp, Warning, TEXT("nullptr 임 "));
			}
			
			//UAssetManager::Get().GetPrimaryAssetObject(MyAssetId)-
			//UE_LOG(LogTemp, Warning, TEXT("Loaded object type: %s"), *UAssetManager::Get().GetPrimaryAssetObject(MyAssetId)->GetClass()->GetName());
			UPostLoadAsset* LoadedAsset = Cast<UPostLoadAsset>(	UAssetManager::Get().GetPrimaryAssetObject(MyAssetId));
			UObject* LoadedObject = UAssetManager::Get().GetPrimaryAssetObject(MyAssetId);
			UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(LoadedObject);

			if (BPClass)
			{
				UObject* BPObject = BPClass->GetDefaultObject(); // 기본 객체를 가져옵니다.
				UPostLoadAsset* PostLoadAsset = Cast<UPostLoadAsset>(BPObject); // 이제 원하는 타입으로 캐스팅을 시도합니다.

				if (PostLoadAsset)
				{
					// 성공적으로 캐스팅되었습니다. 이제 PostLoadAsset을 사용할 수 있습니다.
					//UE_LOG(LogTemp, Warning, TEXT("Casting to UPostLoadAsset successful. %s"), *PostLoadAsset->Ingame_Sound_queue->GetName());
					Get_Sound_Instance();
					
					Sound_Instance->Ingame_Sound_queue = PostLoadAsset->Ingame_Sound_queue;
					Sound_Instance->Walk_Sound_queue = PostLoadAsset->Walk_Sound_queue;
					Sound_Instance->Walk_Sound_Grass_queue = PostLoadAsset->Walk_Sound_Grass_queue;
					Sound_Instance->Walk_Sound_Water_queue = PostLoadAsset->Walk_Sound_Water_queue;
					Sound_Instance->Boss_BGM = PostLoadAsset->Boss_BGM;
					Sound_Instance->Shoot_Sound_queue = PostLoadAsset->Shoot_Sound_queue;
					Sound_Instance->Fire_Sound = PostLoadAsset->Fire_Sound;
					Sound_Instance->Explosion_Sound = PostLoadAsset->Explosion_Sound;
					Sound_Instance->Kraken_Sound = PostLoadAsset->Kraken_Sound;
					Sound_Instance->KrakenAttack_Sound = PostLoadAsset->KrakenAttack_Sound;
					Sound_Instance->Manager_Init();

					//TODO : 색깔이 번쩍번쩍 하게 변합니다. 찾아보면 알림도 있음
					//Ingame BGM ON
					Sound_Play(SOUND_TYPE::BGM_Ingame, 1, FVector(0, 0, 0), FRotator(0, 0, 0));

				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("Casting to UPostLoadAsset failed."));
				}
			}
			TSoftObjectPtr<UPostLoadAsset> SoftObjectPtr(AssetPath);
			//UPostLoadAsset* LoadedAsset = SoftObjectPtr.Get();
			////UE_LOG(LogTemp, Warning, TEXT("Path 로 하나씩 다시 가져오기 %s"), *AssetPath.ToString());
			if(AssetPath.IsValid())
			{
				//UE_LOG(LogTemp, Warning, TEXT("valid 긴해 "));
			}else
			{
				//UE_LOG(LogTemp, Warning, TEXT("valid 아님"));
			}
			// GetStreamableManager().LoadSynchronous()를 호출하여 UObject로 로드된 에셋을 가져옵니다.
			//UObject* LoadedObject = UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
	
			// 로드된 UObject를 UPostLoadAsset 타입으로 캐스트합니다.
			//UObject* LoadedAsset = Cast<UObject>(Streamable.GetLoadedAsset(AssetPath));
        
			//UPostLoadAsset* LoadedAsset = Cast<UPostLoadAsset>(LoadedObject);
	
			if (LoadedAsset)
			{
				// 이제 로드된 에셋을 사용할 수 있습니다.
				// 예를 들어, 사운드 재생, 메시 렌더링 등...
				//UE_LOG(LogTemp, Log, TEXT("Loaded Asset Name: %s"), *LoadedAsset->GetName());
				// ...사용 코드...
			}else
			{
				//UE_LOG(LogTemp, Log, TEXT(" load 에셋 실패"));
			
			}
		}
		// if (SoundAsset && SoundAsset->Sound1) // Sound1을 예로 듭니다. 필요에 따라 Sound2 등 다른 사운드 참조 가능
		// {
		//     UGameplayStatics::PlaySoundAtLocation(this, SoundAsset->Sound1, GetActorLocation());
		// }
	};
//
// // 비동기적으로 사운드 에셋 로드 요청
// //Streamable.RequestAsyncLoad(AssetPath, FStreamableDelegate::CreateLambda(OnLoadComplete));
 Streamable.RequestAsyncLoad(AssetPaths,FStreamableDelegate::CreateLambda(OnLoadComplete));
//Streamable.RequestAsyncLoad(SoundAssetId, FStreamableDelegate::CreateLambda(OnLoadComplete));
	/*
	////UE_LOG(LogTemp, Error, TEXT("LoadStartAsset"));
	//////UE_LOG(LogTemp, Error, TEXT("LoadStartAsset"));
    // 사운드 에셋 비동기 로드
    FPrimaryAssetId SoundAssetId("PostLoadAsset", TEXT("BP_PostLoadAsset"));
    //FPrimaryAssetId SoundAssetId("UPostLoadAsset", TEXT("Default__BP_PostLoadAsset_C"));
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	
    // FPrimaryAssetId에서 FSoftObjectPath 배열을 얻어내는 과정
    //TArray<FSoftObjectPath> AssetPaths;
    //FSoftObjectPath AssetPath = UAssetManager::Get().GetPrimaryAssetPath(SoundAssetId);

	TArray<FPrimaryAssetId> OutAssets;
	UAssetManager& Manager = UAssetManager::Get();
	Manager.GetPrimaryAssetIdList("PostLoadAsset", OutAssets);
	if (OutAssets.IsEmpty()) {
		////UE_LOG(LogTemp, Warning, TEXT("OutAssets Empty"));
	}
	TArray<FSoftObjectPath> AssetPaths;
	for (FPrimaryAssetId Id : OutAssets) {
    TArray<FSoftObjectPath> AssetPaths;
    for (FPrimaryAssetId Id : OutAssets) {
        ////UE_LOG(LogTemp, Warning, TEXT("OutAssets 33333"));
        FSoftObjectPath AssetPath = UAssetManager::Get().GetPrimaryAssetPath(Id);
        if (!AssetPath.IsValid())
        {
            ////UE_LOG(LogTemp, Error, TEXT("AssetPath is not valid."));
        }
        else
        {
            ////UE_LOG(LogTemp, Error, TEXT("AssetPath is  valid. 777"));
        }
        AssetPaths.Add(AssetPath);
    }
//AssetPaths.Add(AssetPath);
	return;

// 비동기 로드 콜백
auto OnLoadComplete = [this, AssetPaths, &Streamable, &AssetManager]()
{
    //UPostLoadAsset* SoundAsset = Cast<UPostLoadAsset>(LoadedAsset);
    ////UE_LOG(LogTemp, Warning, TEXT("UPostLoadAsset Load 성공"));

	for (const FSoftObjectPath& AssetPath : AssetPaths)
	{
		FPrimaryAssetId MyAssetId = AssetManager->GetPrimaryAssetIdForPath(AssetPath);
		UPostLoadAsset* LoadedAsset = Cast<UPostLoadAsset>(	AssetManager->GetPrimaryAssetObject(MyAssetId));
		TSoftObjectPtr<UPostLoadAsset> SoftObjectPtr(AssetPath);
		//UPostLoadAsset* LoadedAsset = SoftObjectPtr.Get();
		////UE_LOG(LogTemp, Warning, TEXT("Path 로 하나씩 다시 가져오기 %s"), *AssetPath.ToString());
		if(AssetPath.IsValid())
		{
			////UE_LOG(LogTemp, Warning, TEXT("valid 긴해 "));
		}else
		{
			////UE_LOG(LogTemp, Warning, TEXT("valid 아님"));
		}
		// GetStreamableManager().LoadSynchronous()를 호출하여 UObject로 로드된 에셋을 가져옵니다.
		//UObject* LoadedObject = UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);

		// 로드된 UObject를 UPostLoadAsset 타입으로 캐스트합니다.
		//UObject* LoadedAsset = Cast<UObject>(Streamable.GetLoadedAsset(AssetPath));
        
		//UPostLoadAsset* LoadedAsset = Cast<UPostLoadAsset>(LoadedObject);

		if (LoadedAsset)
		{
			// 이제 로드된 에셋을 사용할 수 있습니다.
			// 예를 들어, 사운드 재생, 메시 렌더링 등...
			////UE_LOG(LogTemp, Log, TEXT("Loaded Asset Name: %s"), *LoadedAsset->GetName());
			// ...사용 코드...
		}else
		{
			////UE_LOG(LogTemp, Log, TEXT(" load 에셋 실패"));
			
		}
	}
    // if (SoundAsset && SoundAsset->Sound1) // Sound1을 예로 듭니다. 필요에 따라 Sound2 등 다른 사운드 참조 가능
    // {
    //     UGameplayStatics::PlaySoundAtLocation(this, SoundAsset->Sound1, GetActorLocation());
    // }
};

// 비동기적으로 사운드 에셋 로드 요청
//Streamable.RequestAsyncLoad(AssetPath, FStreamableDelegate::CreateLambda(OnLoadComplete));
	
Streamable.RequestAsyncLoad(AssetPaths,FStreamableDelegate::CreateLambda(OnLoadComplete));
//Streamable.RequestAsyncLoad(SoundAssetId, FStreamableDelegate::CreateLambda(OnLoadComplete));

*/
	///////////////////////////////////////////////////////////////////////
	///
	
	// UAssetManager::Get().ReinitializeFromConfig();
	// FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	//
	// // 비동기 로드 콜백
	// // TSoftObjectPtr는 특정 에셋에 대한 소프트 참조입니다.
	// FSoftObjectPath DirectAssetPath(TEXT("/Game/MAIN/BP_PostLoadAsset.BP_PostLoadAsset_C"));
	// TSoftObjectPtr<UPostLoadAsset> SoftObjectPtr(DirectAssetPath);
	// //LoadAssetPointer = TSoftObjectPtr<UPostLoadAsset>(DirectAssetPath);
	//
	//
	// auto OnLoadComplete = [SoftObjectPtr]()
	// {
	// 	//UPostLoadAsset* SoundAsset = Cast<UPostLoadAsset>(LoadedAsset);
	// 	////UE_LOG(LogTemp, Warning, TEXT("UPostLoadAsset Load 성공"));
	// 	
	// 	// 에셋 매니저를 사용하여 로드된 에셋을 가져옵니다.
	// 	UPostLoadAsset* LoadedAsset = SoftObjectPtr.Get();
	// 	//UPostLoadAsset* LoadedAsset = Cast<UPostLoadAsset>(StaticLoadObject(UPostLoadAsset::StaticClass(), nullptr, *DirectAssetPath.ToString()));
	//
	// 	if (LoadedAsset)
	// 	{
	// 		// 에셋이 성공적으로 로드되었습니다. 이제 여기서 로드된 에셋을 사용할 수 있습니다.
	// 		////UE_LOG(LogTemp, Log, TEXT("Asset Loaded Successfully. %s"), *LoadedAsset->Shoot_Sound_queue->GetName() );
	// 		// 이후 작업을 수행합니다. 예를 들어, 사운드 재생 등...
	// 		
	// 	}
	// 	else
	// 	{
	// 		////UE_LOG(LogTemp, Error, TEXT("Failed to load the asset."));
	// 	}
	// 	
	// 	//UObject* LoadedAsset = Streamable.GetLoadedAsset(DirectAssetPath);
 //    
	// 	// 로드된 에셋이 UPostLoadAsset 타입인지 확인하고 캐스트합니다.
	// 	//UPostLoadAsset* SoundAsset = Cast<UPostLoadAsset>(LoadedAsset);
	// 	
	// 	// if (SoundAsset && SoundAsset->Sound1) // Sound1을 예로 듭니다. 필요에 따라 Sound2 등 다른 사운드 참조 가능
	// 	// {
	// 	// 	UGameplayStatics::PlaySoundAtLocation(this, SoundAsset->Sound1, GetActorLocation());
	// 	// }
	// };
	//
	//
	// if (!DirectAssetPath.IsValid())
	// {
	// 	////UE_LOG(LogTemp, Error, TEXT("DirectAssetPath is not valid."));
	// }
	// else
	// {
	// 	////UE_LOG(LogTemp, Error, TEXT("DirectAssetPath valid."));
	// 	//Streamable.RequestAsyncLoad(DirectAssetPath, FStreamableDelegate::CreateLambda(OnLoadComplete));
	// 	Streamable.RequestAsyncLoad(SoftObjectPtr.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(OnLoadComplete));
	// }
	//
	// // 비동기적으로 사운드 에셋 로드 요청
	// //Streamable.RequestAsyncLoad(AssetPath, FStreamableDelegate::CreateLambda(OnLoadComplete));
	// //Streamable.RequestAsyncLoad(AssetPaths,FStreamableDelegate::CreateLambda(OnLoadComplete));
	// TArray<FName> LoadBundles;
	// //Manager.LoadPrimaryAssets(OutAssets, LoadBundles, FStreamableDelegate::CreateLambda(OnLoadComplete));
	// //Streamable.RequestAsyncLoad(SoundAssetId, FStreamableDelegate::CreateLambda(OnLoadComplete));


	/////////////////

	// UAssetManager& __Manager = UAssetManager::Get();
	//
	// TArray<FPrimaryAssetId> Assets;
	// __Manager.GetPrimaryAssetIdList(TEXT("PostLoadAsset"), Assets);
	//
	//
	// if(Assets.Num() > 0)
	// {
	// 	////UE_LOG(LogTemp, Warning, TEXT("Asset Num > 0"));
	//
	// 	int32 AccessIdx = 2;
	// 	FSoftObjectPtr AssetPtr(__Manager.GetPrimaryAssetPath(Assets[AccessIdx ]));
	// 	// 3. 에셋의 경로를 통해 에셋에 대한 FSoftObjectPtr을 얻어온다.
	//
	// 	if(AssetPtr.IsPending())
	// 	{
	// 		AssetPtr.LoadSynchronous();
	// 		// 4. 에셋을 로딩해온다.
	// 	}
	// 	UPostLoadAsset* Item = Cast<UPostLoadAsset>(AssetPtr.Get());
	//
	// 	if(Item)
	// 	{
	// 		////UE_LOG(LogTemp, Warning, TEXT("Asset loaded successfully!"));
	// 	}
	// 	else ////UE_LOG(LogTemp, Warning, TEXT("Asset loaded fail..."));
	// }
	// else ////UE_LOG(LogTemp, Warning, TEXT("Asset Num = 0"));
	//
	//

	// StreamableManager 생성
	// FStreamableManager StreamableManager;
	//
	// // 비동기적으로 로드할 에셋의 경로 지정
	// FStringAssetReference AssetRef(TEXT("/Game/Path/To/YourAsset.YourAsset"));
	//
	// // 에셋을 로드할 때 실행될 콜백 함수 정의
	// FStreamableDelegate Callback;
	// Callback.BindLambda([]() {
	// 	// 에셋 로딩이 완료되면 실행될 코드 작성
	// 	////UE_LOG(LogTemp, Warning, TEXT("Asset loaded successfully!"));
	// });
	//
	// // 비동기적으로 에셋 로드
	// StreamableManager.RequestAsyncLoad(AssetRef, Callback);
}

void UNetwork_Manager_R::Sound_Play(SOUND_TYPE Sound_Type, int32 Audio_idx, FVector Location, FRotator Rotator, APawn* MyPawn)
{
	if (!Sound_Instance) Get_Sound_Instance();	
	Sound_Instance->Sound_Play(Sound_Type, Audio_idx, Location, Rotator, MyPawn);

}

void UNetwork_Manager_R::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	if (bWasSuccessful && Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			// JSON 응답 파싱을 시도합니다.

			// JSON 응답을 배열로 파싱 시도합니다.
			TSharedPtr<FJsonValue> JsonValue;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			//////UE_LOG(LogTemp, Error, TEXT("response...  %s"), *Response->GetContentAsString());
			if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid() && JsonValue->Type == EJson::Array)
			{
				TArray<TSharedPtr<FJsonValue>> JsonArray = JsonValue->AsArray();
				// 배열 내의 객체를 처리합니다.
				for (auto& Item : JsonArray)
				{
					TSharedPtr<FJsonObject> JsonObject = Item->AsObject();

					if (JsonObject.IsValid())
					{
						if (JsonObject->HasField(TEXT("document")))
						{
							// "document" 객체에 접근합니다.
							TSharedPtr<FJsonObject> DocumentObject = JsonObject->GetObjectField(TEXT("document"));
							if (DocumentObject.IsValid())
							{
								if (1 == 2)
								{
									//Join 시도

								}
								else
								{
									//이미 있는것이니 성공 띄우고 몇초 뒤에 로그인 시켜버리면됨
									//////UE_LOG(LogTemp, Error, TEXT("성공창 "));
									if (Current_Widget)
									{
										//////UE_LOG(LogTemp, Error, TEXT("성공해서 위젯 없애버려볼까 "));
										Current_Widget->RemoveFromParent();
										if (Success_Widget)
										{
											GetSpawnData();
											Sound_Play(SOUND_TYPE::Btn_GameStart, 1, FVector(0, 0, 0), FRotator(0, 0, 0));

											// UUserWidget* TmpWidget = CreateWidget<UUserWidget>(this, Success_Widget, TEXT("SuccessWidget"));
											// if(TmpWidget)
											// TmpWidget->AddToViewport();
											// ////UE_LOG(LogTemp, Log, TEXT("성공창 띠ㅡ웠음"));
										}
									}

									// "name" 필드 값을 가져옵니다.
									FString Name = DocumentObject->GetStringField(TEXT("name"));
									////UE_LOG(LogTemp, Log, TEXT("Document Name: %s"), *Name);

									// "fields" 객체 내의 "ID"와 "PW" 필드 값을 가져옵니다.
									TSharedPtr<FJsonObject> FieldsObject = DocumentObject->GetObjectField(TEXT("fields"));
									if (FieldsObject.IsValid())
									{
										FString ID = FieldsObject->GetObjectField(TEXT("ID"))->GetStringField(TEXT("stringValue"));
										FString PW = FieldsObject->GetObjectField(TEXT("PW"))->GetStringField(TEXT("stringValue"));
										////UE_LOG(LogTemp, Log, TEXT("ID: %s, PW: %s"), *ID, *PW);
									}
								}


								return;
							}
						}
						else
						{
							//////UE_LOG(LogTemp, Error, TEXT("이럼 없는 거임"));
						}

					}
				}
			}
			else
			{
				//////UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response."));
			}
			//TSharedPtr<FJsonObject> JsonObject;
			//TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

			////UE_LOG(LogTemp, Log, TEXT("HTTP Request was successful. Response: %s"), *Response->GetContentAsString());
			// Parse the JSON response if needed
			// ... rest of your code for JSON parsing
			// Assuming the query results are in an array named "results"
			 //TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());


		}
		else
		{
			////UE_LOG(LogTemp, Warning, TEXT("HTTP Request returned status code: %d"), Response->GetResponseCode());
		}
	}
	else
	{
		//////UE_LOG(LogTemp, Error, TEXT("HTTP Request was not successful."));
	}


	//여기에 실패 띄우기
	//////UE_LOG(LogTemp, Error, TEXT("실패창 "));
	CreateWidget_OX(false);

}

void UNetwork_Manager_R::OnResponseReceived_Join(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			// JSON 응답 파싱을 시도합니다.

			// JSON 응답을 배열로 파싱 시도합니다.
			TSharedPtr<FJsonValue> JsonValue;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			//////UE_LOG(LogTemp, Error, TEXT("response...  %s"), *Response->GetContentAsString());
			if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid() && JsonValue->Type == EJson::Array)
			{
				TArray<TSharedPtr<FJsonValue>> JsonArray = JsonValue->AsArray();
				// 배열 내의 객체를 처리합니다.
				for (auto& Item : JsonArray)
				{
					TSharedPtr<FJsonObject> JsonObject = Item->AsObject();
					if (JsonObject.IsValid())
					{
						if (JsonObject->HasField(TEXT("document")))
						{
							// "document" 객체에 접근합니다.
							TSharedPtr<FJsonObject> DocumentObject = JsonObject->GetObjectField(TEXT("document"));
							if (DocumentObject.IsValid())
							{
								//////UE_LOG(LogTemp, Error, TEXT("이미 존재하는 아이디라 실패"));
								CreateWidget_OX(false);

								return;
							}
						}
						else
						{

							InsertUser(Join_ID, Join_PW, Join_NickName);
							return;
						}

					}
				}
			}
			else
			{
				//////UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response."));
			}
			//TSharedPtr<FJsonObject> JsonObject;
			//TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

			////UE_LOG(LogTemp, Log, TEXT("HTTP Request was successful. Response: %s"), *Response->GetContentAsString());
			// Parse the JSON response if needed
			// ... rest of your code for JSON parsing
			// Assuming the query results are in an array named "results"
			 //TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());



		}
		else
		{
			////UE_LOG(LogTemp, Warning, TEXT("HTTP Request returned status code: %d"), Response->GetResponseCode());
		}
	}
	else
	{
		//////UE_LOG(LogTemp, Error, TEXT("HTTP Request was not successful."));
	}


	//여기에 실패 띄우기
	//////UE_LOG(LogTemp, Error, TEXT("실패창 "));
}

void UNetwork_Manager_R::SelectUser(const FString& Username, const FString& Password, UUserWidget* InputWidget)
{
	Current_Widget = InputWidget;
	FHttpModule* Http = &FHttpModule::Get();
	if (Http)
	{
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->SetVerb("POST");
		//Request->SetURL(TEXT("https://firestore.googleapis.com/v1/projects/unrealrpg-7a179/databases/(default)/documents/User/"));
		Request->SetURL(TEXT("https://firestore.googleapis.com/v1/projects/unrealrpg-7a179/databases/(default)/documents:runQuery"));
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		//FString QueryJson = TEXT("{\"structuredQuery\":{\"where\":{\"fieldFilter\":{\"field\":{\"fieldPath\":\"ID\"},\"op\":\"EQUAL\",\"value\":{\"stringValue\":\"fffl\"}}},\"from\":[{\"collectionId\":\"User\"}]}}");
		//Request->SetContentAsString(QueryJson);

		//FString QueryJson = TEXT("{\"structuredQuery\":{\"where\":{\"fieldFilter\":{\"field\":{\"fieldPath\":\"PW\"},\"op\":\"EQUAL\",\"value\":{\"stringValue\":\"fppp1\"}}},\"from\":[{\"collectionId\":\"User\"}]}}");
		FString QueryJson = FString::Printf(TEXT(
			"{\"structuredQuery\":{\"where\":{\"compositeFilter\":{\"op\":\"AND\",\"filters\":["
			"{\"fieldFilter\":{\"field\":{\"fieldPath\":\"ID\"},\"op\":\"EQUAL\",\"value\":{\"stringValue\":\"%s\"}}},"
			"{\"fieldFilter\":{\"field\":{\"fieldPath\":\"PW\"},\"op\":\"EQUAL\",\"value\":{\"stringValue\":\"%s\"}}}"
			"]}},\"from\":[{\"collectionId\":\"User\"}]}}"),
			*Username, *Password);
		Request->SetContentAsString(QueryJson);


		// Set any other necessary headers here, like Authorization if needed
		Login_ID = Username;
		Request->OnProcessRequestComplete().BindUObject(this, &UNetwork_Manager_R::OnResponseReceived);
		if (!Request->ProcessRequest())
		{
			////UE_LOG(LogTemp, Log, TEXT("Flag8"));
		}
	}
	else
	{
		////UE_LOG(LogTemp, Log, TEXT("Flag9"));
	}
}

void UNetwork_Manager_R::InsertUser(const FString& Username, const FString& Password, const FString& NickName)
{
	FHttpModule* Http = &FHttpModule::Get();
	if (Http)
	{
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->SetVerb("POST");
		//FString Url = FString::Printf(TEXT("https://firestore.googleapis.com/v1/projects/unrealrpg-7a179/databases/(default)/documents/User"));
		//Request->SetURL(Url);
		Request->SetURL(TEXT("https://firestore.googleapis.com/v1/projects/unrealrpg-7a179/databases/(default)/documents/User"));
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

		// Create JSON object for user data
		//FString JsonData = FString::Printf(TEXT("{\"fields\": {\"ID\": {\"stringValue\": \"%s\"}, \"PW\": {\"stringValue\": \"%s\"}}}"), *Username, *Password);

		// Create JSON object for user data
		FString JsonData = FString::Printf(TEXT("{\"fields\": {\"ID\": {\"stringValue\": \"%s\"}, \"PW\": {\"stringValue\": \"%s\"}, \"NICK\" : {\"stringValue\" : \"%s\"}}}"), *Username, *Password, *NickName);
		//FString JsonData = FString::Printf(TEXT("{\"name\": \"projects/unrealrpg-7a179/databases/(default)/documents/User/%s\",\"fields\": {\"ID\": {\"stringValue\": \"%s\"}, \"PW\": {\"stringValue\": \"%s\"}}}"), *Username, *Username, *Password);

		Request->SetContentAsString(JsonData);

		Request->OnProcessRequestComplete().BindUObject(this, &UNetwork_Manager_R::OnInsertUserResponseReceived);
		Request->ProcessRequest();
	}
	else
	{
		CreateWidget_OX(false);
		//////UE_LOG(LogTemp, Error, TEXT("Failed to get HTTP module!"));
	}
}

void UNetwork_Manager_R::ResistUser(const FString& Username, const FString& Password, const FString& NickName, UUserWidget* InputWidget)
{
	Current_Widget = InputWidget;

	Join_ID = Username;
	Join_PW = Password;
	Join_NickName = NickName;

	FHttpModule* Http = &FHttpModule::Get();
	if (Http)
	{
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->SetVerb("POST");
		//Request->SetURL(TEXT("https://firestore.googleapis.com/v1/projects/unrealrpg-7a179/databases/(default)/documents/User/"));
		Request->SetURL(TEXT("https://firestore.googleapis.com/v1/projects/unrealrpg-7a179/databases/(default)/documents:runQuery"));
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		//FString QueryJson = TEXT("{\"structuredQuery\":{\"where\":{\"fieldFilter\":{\"field\":{\"fieldPath\":\"ID\"},\"op\":\"EQUAL\",\"value\":{\"stringValue\":\"fffl\"}}},\"from\":[{\"collectionId\":\"User\"}]}}");
		//Request->SetContentAsString(QueryJson);

		//FString QueryJson = TEXT("{\"structuredQuery\":{\"where\":{\"fieldFilter\":{\"field\":{\"fieldPath\":\"PW\"},\"op\":\"EQUAL\",\"value\":{\"stringValue\":\"fppp1\"}}},\"from\":[{\"collectionId\":\"User\"}]}}");
		FString QueryJson = FString::Printf(TEXT(
			"{\"structuredQuery\":{\"where\":{\"fieldFilter\":{\"field\":{\"fieldPath\":\"ID\"},\"op\":\"EQUAL\",\"value\":{\"stringValue\":\"%s\"}}},"
			"\"from\":[{\"collectionId\":\"User\"}]}}"),
			*Username);
		Request->SetContentAsString(QueryJson);


		// Set any other necessary headers here, like Authorization if needed

		Request->OnProcessRequestComplete().BindUObject(this, &UNetwork_Manager_R::OnResponseReceived_Join);
		if (!Request->ProcessRequest())
		{
			////UE_LOG(LogTemp, Log, TEXT("Flag8"));
		}
	}
	else
	{
		////UE_LOG(LogTemp, Log, TEXT("Flag9"));
	}
}


void UNetwork_Manager_R::OnInsertUserResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			////UE_LOG(LogTemp, Log, TEXT("User inserted successfully!"));
			// Handle success
			CreateWidget_OX(true);
			
		}
		else
		{
			////UE_LOG(LogTemp, Warning, TEXT("HTTP Request returned status code: %d"), Response->GetResponseCode());
			CreateWidget_OX(false);
			// Handle error
		}
	}
	else
	{
		//////UE_LOG(LogTemp, Error, TEXT("HTTP Request was not successful."));
		CreateWidget_OX(false);
		// Handle error
	}
}

void UNetwork_Manager_R::CreateWidget_OX(bool bIsSuccess)
{

	if (bIsSuccess) Sound_Play(SOUND_TYPE::Btn_GameStart, 2, FVector(0, 0, 0), FRotator(0, 0, 0));
	else Sound_Play(SOUND_TYPE::Btn_Click_Fail, 2, FVector(0, 0, 0), FRotator(0, 0, 0));

	UUserWidget* TmpWidget;
	FString UniqueID = FGuid::NewGuid().ToString();
	FName UniqueName = FName(*(FString("SuccessWidget_") + UniqueID));
	if (bIsSuccess)
		TmpWidget = CreateWidget<UUserWidget>(this, Success_Widget, UniqueName);
	else
		TmpWidget = CreateWidget<UUserWidget>(this, Fail_Widget, UniqueName);

	if (TmpWidget)
		TmpWidget->AddToViewport();

}

void UNetwork_Manager_R::GetSpawnData()
{
	//CreateWidget_OX(true);

	FHttpModule* Http = &FHttpModule::Get();
	if (Http)
	{
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->SetVerb("POST");
		Request->SetURL(TEXT("https://firestore.googleapis.com/v1/projects/unrealrpg-7a179/databases/(default)/documents:runQuery"));
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

		FString QueryJson = FString::Printf(TEXT(
	"{\"structuredQuery\":{\"where\":{\"fieldFilter\":{\"field\":{\"fieldPath\":\"ID\"},\"op\":\"EQUAL\",\"value\":{\"stringValue\":\"%s\"}}},"
	"\"from\":[{\"collectionId\":\"Spawn\"}]}}"),
	*Login_ID); 

		Request->SetContentAsString(QueryJson);


		// Set any other necessary headers here, like Authorization if needed

		Request->OnProcessRequestComplete().BindUObject(this, &UNetwork_Manager_R::GetSpawnData_CallBack);
		if (!Request->ProcessRequest())
		{
			////UE_LOG(LogTemp, Log, TEXT("Flag8"));
		}
	}
	else
	{
		////UE_LOG(LogTemp, Log, TEXT("Flag9"));
	}

}

void UNetwork_Manager_R::GetSpawnData_CallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//////UE_LOG(LogTemp, Error, TEXT("       spawn data Callback      "));
	if (bWasSuccessful && Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			// JSON 응답 파싱을 시도합니다.

			// JSON 응답을 배열로 파싱 시도합니다.
			TSharedPtr<FJsonValue> JsonValue;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			//UE_LOG(LogTemp, Error, TEXT("response...  %s"), *Response->GetContentAsString());
			if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid() && JsonValue->Type == EJson::Array)
			{
				TArray<TSharedPtr<FJsonValue>> JsonArray = JsonValue->AsArray();
				// 배열 내의 객체를 처리합니다.
				for (auto& Item : JsonArray)
				{
					TSharedPtr<FJsonObject> JsonObject = Item->AsObject();

					if (JsonObject.IsValid())
					{
						if (JsonObject->HasField(TEXT("document")))
						{
							// "document" 객체에 접근합니다.
							TSharedPtr<FJsonObject> DocumentObject = JsonObject->GetObjectField(TEXT("document"));
							if (DocumentObject.IsValid())
							{
								TSharedPtr<FJsonObject> FieldsObject = DocumentObject->GetObjectField(TEXT("fields"));
								if (FieldsObject.IsValid())
								{
									//////UE_LOG(LogTemp, Error, TEXT("fields get!!!!!!!!!!!!"));
									TSharedPtr<FJsonObject> SpawnLocation_X_Object = FieldsObject->GetObjectField(TEXT("SpawnLocation_X"));
									TSharedPtr<FJsonObject> SpawnLocation_Y_Object = FieldsObject->GetObjectField(TEXT("SpawnLocation_Y"));
									TSharedPtr<FJsonObject> SpawnLocation_Z_Object = FieldsObject->GetObjectField(TEXT("SpawnLocation_Z"));
									TSharedPtr<FJsonObject> SpawnRotator_Pitch_Object = FieldsObject->GetObjectField(TEXT("SpawnRotator_Pitch"));
									TSharedPtr<FJsonObject> SpawnRotator_Roll_Object = FieldsObject->GetObjectField(TEXT("SpawnRotator_Roll"));
									TSharedPtr<FJsonObject> SpawnRotator_Yaw_Object = FieldsObject->GetObjectField(TEXT("SpawnRotator_Yaw"));
									TSharedPtr<FJsonObject> SwordNameObject = FieldsObject->GetObjectField(TEXT("SwordName"));
									TSharedPtr<FJsonObject> GunNameObject = FieldsObject->GetObjectField(TEXT("GunName"));
									TSharedPtr<FJsonObject> IDObject = FieldsObject->GetObjectField(TEXT("ID"));

									// SpawnLocation 값을 double로 가져옵니다.
									double SpawnLocation_X = SpawnLocation_X_Object->GetNumberField(TEXT("doubleValue"));
									double SpawnLocation_Y = SpawnLocation_Y_Object->GetNumberField(TEXT("doubleValue"));
									double SpawnLocation_Z = SpawnLocation_Z_Object->GetNumberField(TEXT("doubleValue"));

									SpawnLocation = FVector(SpawnLocation_X, SpawnLocation_Y, SpawnLocation_Z);

									// SpawnRotator 값을 double로 가져옵니다.
									double SpawnRotator_Pitch = SpawnRotator_Pitch_Object->GetNumberField(TEXT("doubleValue"));
									double SpawnRotator_Roll = SpawnRotator_Roll_Object->GetNumberField(TEXT("doubleValue"));
									double SpawnRotator_Yaw = SpawnRotator_Yaw_Object->GetNumberField(TEXT("doubleValue"));

									SpawnRotator = FRotator(SpawnRotator_Pitch, SpawnRotator_Yaw, SpawnRotator_Roll);

									// SwordName 값을 FString으로 가져옵니다.
									SwordName = SwordNameObject->GetStringField(TEXT("stringValue"));

									// GunName 값을 FString으로 가져옵니다.
									GunName = GunNameObject->GetStringField(TEXT("stringValue"));

									// ID 값을 FString으로 가져옵니다.
									FString ID = IDObject->GetStringField(TEXT("stringValue"));

									TSharedPtr<FJsonObject> ItemsObject = FieldsObject->GetObjectField(TEXT("Items"));
									TSharedPtr<FJsonObject> ArrayValueObject = ItemsObject->GetObjectField(TEXT("arrayValue"));
									ValuesArray = ArrayValueObject->GetArrayField(TEXT("values"));
									
									/// 동재추가코드
									TSharedPtr<FJsonObject> MoneyObject = FieldsObject->GetObjectField(TEXT("Money"));
									MoneyFromServer = MoneyObject->GetIntegerField(TEXT("integerValue"));
						
									/// 끝

									TSharedPtr<FJsonObject> Cur_Health_obj = FieldsObject->GetObjectField(TEXT("Health"));
									CurrentHealthFromServer = Cur_Health_obj->GetNumberField(TEXT("doubleValue"));

									TSharedPtr<FJsonObject> Max_Health_obj = FieldsObject->GetObjectField(TEXT("MaxHealth"));
									MaxHealthFromServer = Max_Health_obj->GetNumberField(TEXT("doubleValue"));

									TSharedPtr<FJsonObject> IsQuestAccept_obj = FieldsObject->GetObjectField(TEXT("IsQuestAccept"));
									IsQuestAccept = IsQuestAccept_obj->GetNumberField(TEXT("integerValue"));
									UE_LOG(LogTemp, Log, TEXT("FROM SERVER ::  %d"), IsQuestAccept);
								}
							}
						}
						else
						{
							//////UE_LOG(LogTemp, Error, TEXT("이럼 없는 거임"));

						}
					}
					Spawn_Init();

				}
			}
			else
			{
				//////UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response."));
			}
			//TSharedPtr<FJsonObject> JsonObject;
			//TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

			////UE_LOG(LogTemp, Log, TEXT("HTTP Request was successful. Response: %s"), *Response->GetContentAsString());
			// Parse the JSON response if needed
			// ... rest of your code for JSON parsing
			// Assuming the query results are in an array named "results"
			 //TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());


		}
		else
		{
			////UE_LOG(LogTemp, Warning, TEXT("HTTP Request returned status code: %d"), Response->GetResponseCode());
		}
	}
	else
	{
		//////UE_LOG(LogTemp, Error, TEXT("HTTP Request was not successful."));
	}


}

void UNetwork_Manager_R::Spawn_Init()
{
	if (SpawnLocation.IsNearlyZero() && SpawnRotator.IsNearlyZero())
	{
		// 변수가 '초기화되지 않았다'고 가정하고 처리
		//////UE_LOG(LogTemp, Error, TEXT("ITS NEWB"));
	}
	else
	{
		//////UE_LOG(LogTemp, Error, TEXT("ITS OLDB %s %s %s %s %s"), *SpawnLocation.ToCompactString(), *SpawnRotator.ToCompactString(), *GunName, *Login_ID, *SwordName);
	}

	// 현재 월드를 가져옵니다.
	UWorld* World = GetWorld();
	//////UE_LOG(LogTemp, Error, TEXT("Nope22"));
	if (World != nullptr)
	{
		// 레벨 시퀀스 플레이어를 생성합니다.
		Player_Sequence = ULevelSequencePlayer::CreateLevelSequencePlayer(World, Level_Sequence, MovieSceneSequencePlaybackSettings, SequenceActor);
		//////UE_LOG(LogTemp, Error, TEXT("Nope33"));
		
		AController* MyController = World->GetFirstPlayerController();
		if(Cast<APlayerController>(MyController))
		{
			////UE_LOG(LogTemp, Log, TEXT("Mouse 커서 없앴음 777"));
			//Cast<APlayerController>(MyController)->bShowMouseCursor = false;

			const FInputModeGameOnly InputMode;
			Cast<APlayerController>(MyController)->SetInputMode(InputMode);
			Cast<APlayerController>(MyController)->SetShowMouseCursor(false);
		}
	}
	//////UE_LOG(LogTemp, Error, TEXT("Nope44"));

	// 시퀀스 플레이어가 성공적으로 생성되었는지 확인하고 재생합니다.
	if (Player_Sequence != nullptr)
	{
		Player_Sequence->SetPlayRate(0.7f); // 재생 속도를 0.5로 설정
		Player_Sequence->OnFinished.AddDynamic(this, &UNetwork_Manager_R::OnSequenceFinished);
		Player_Sequence->Play();
		//////UE_LOG(LogTemp, Error, TEXT("Play!!!"));

	}
	else
	{
		//////UE_LOG(LogTemp, Error, TEXT("Nope"));
	}
}

void UNetwork_Manager_R::SetSpawnData(FVector _Location, FRotator _Rotation, FString _Login_ID)
{
	SpawnLocation = _Location;
	SpawnRotator = _Rotation;
	Login_ID = _Login_ID;
	////UE_LOG(LogTemp, Log, TEXT("Setspawndata init %s %s %s"), *_Location.ToCompactString(), *_Rotation.ToCompactString(), *_Login_ID);
	//우선 이미 있는 데이터인지 확인하고 그 안에서 있으면 update, 없으면 insert
	FHttpModule* Http = &FHttpModule::Get();
	
	if (Http)
	{
		////UE_LOG(LogTemp, Log, TEXT("Flag33"));
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->SetVerb("POST");
		Request->SetURL(TEXT("https://firestore.googleapis.com/v1/projects/unrealrpg-7a179/databases/(default)/documents:runQuery"));
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

		FString QueryJson = FString::Printf(TEXT(
	"{\"structuredQuery\":{\"where\":{\"fieldFilter\":{\"field\":{\"fieldPath\":\"ID\"},\"op\":\"EQUAL\",\"value\":{\"stringValue\":\"%s\"}}},"
	"\"from\":[{\"collectionId\":\"Spawn\"}]}}"),
	*Login_ID); 

		Request->SetContentAsString(QueryJson);

		////UE_LOG(LogTemp, Log, TEXT("Flag44"));
		// Set any other necessary headers here, like Authorization if needed
		Request->OnProcessRequestComplete().BindUObject(this, &UNetwork_Manager_R::SetSpawnData_Callback);
		////UE_LOG(LogTemp, Log, TEXT("Flag55"));
		if (!Request->ProcessRequest())
		{
			////UE_LOG(LogTemp, Log, TEXT("Flag8"));
		}
	}
	else
	{
		////UE_LOG(LogTemp, Log, TEXT("Flag9"));
	}
	
}

void UNetwork_Manager_R::SetSpawnData_Callback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	////UE_LOG(LogTemp, Log, TEXT("setspawn callback ..."));
	if (bWasSuccessful && Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			// JSON 응답 파싱을 시도합니다.

			// JSON 응답을 배열로 파싱 시도합니다.
			TSharedPtr<FJsonValue> JsonValue;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			////UE_LOG(LogTemp, Error, TEXT("SetSpawnData_Callback response...  %s"), *Response->GetContentAsString());
			if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid() && JsonValue->Type == EJson::Array)
			{
				TArray<TSharedPtr<FJsonValue>> JsonArray = JsonValue->AsArray();
				// 배열 내의 객체를 처리합니다.
				for (auto& Item : JsonArray)
				{
					TSharedPtr<FJsonObject> JsonObject = Item->AsObject();
					
					if (JsonObject.IsValid())
					{
						if (JsonObject->HasField(TEXT("document")))
						{
							// "document" 객체에 접근합니다.
							TSharedPtr<FJsonObject> DocumentObject = JsonObject->GetObjectField(TEXT("document"));
							if (DocumentObject.IsValid())
							{
								FString DocumentName = DocumentObject->GetStringField("name");
                
								// 문서 ID를 추출합니다. 일반적으로 마지막 '/' 이후의 문자열이 ID입니다.
								DocumentId = DocumentName;
								int32 LastSlashIndex;
								if (DocumentName.FindLastChar('/', LastSlashIndex))
								{
									DocumentId = DocumentName.Mid(LastSlashIndex + 1);
								}
								
								//이미 있는것이니 UPDATE
								////UE_LOG(LogTemp, Log, TEXT("flag1 ..."));
								UpdateSpawnData();

								return;
							}
						}else
						{
							////UE_LOG(LogTemp, Error, TEXT("이럼 없는 거임"));
							//없으니 INSERT
							InsertSpawnData();
						}
						
					}
				}
			}
			else
			{
				////UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response."));
			}
			//TSharedPtr<FJsonObject> JsonObject;
			//TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

			////UE_LOG(LogTemp, Log, TEXT("HTTP Request was successful. Response: %s"), *Response->GetContentAsString());
			// Parse the JSON response if needed
			// ... rest of your code for JSON parsing
			// Assuming the query results are in an array named "results"
			 //TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			
			
		}
		else
		{
			////UE_LOG(LogTemp, Warning, TEXT("HTTP Request returned status code: %d"), Response->GetResponseCode());
		}
	}
	else
	{
		////UE_LOG(LogTemp, Error, TEXT("HTTP Request was not successful."));
	}


}

void UNetwork_Manager_R::UpdateSpawnData()
{
	//UE_LOG(LogTemp, Log, TEXT("UpdateSpawnData ..."));
	FHttpModule* Http = &FHttpModule::Get();
	if (Http)
	{
		// Item 가진 스크립트

		
		const APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		const UInventoryComponent* PlayerInventory = CurrentCharacter->GetInventory();
		uint64 MyMoney = PlayerInventory->GetMoney();
		float MyHealth = CurrentCharacter->GetCurrentHealth();
		float MyMaxHealth = CurrentCharacter->GetMaxHealth();
		int8 _IsQuestAccept = CurrentCharacter->IsQuestAccept;
		FString ItemJson = TEXT("[");
		

		const TArray<UItemBase*>& InventoryItems = PlayerInventory->GetInventory();
		
		for (int32 i = 0; i < InventoryItems.Num(); ++i)
		{
			UItemBase* const& InventoryItem = InventoryItems[i];
			ItemJson += FString::Printf(TEXT(
				"{"
				"\"mapValue\": {"
				"\"fields\": {"
				"\"key\": {\"stringValue\": \"%s\"},"
				"\"quantity\": {\"integerValue\": %d}"
				"}"
				"}"
				"}"), 
				*InventoryItem->BaseItemID.ToString(), InventoryItem->BaseItemQuantity
			);
    
			if (i < InventoryItems.Num() - 1)
			{
				ItemJson += TEXT(",");
			}
		}

		
		ItemJson += TEXT("]");

		//UE_LOG(LogTemp,Log,TEXT(" ITEMJSON : %s "),*ItemJson);
		
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->SetVerb("PATCH");
		Request->SetURL(FString::Printf(TEXT("https://firestore.googleapis.com/v1/projects/unrealrpg-7a179/databases/(default)/documents/Spawn/%s"), *DocumentId));
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

		// JSON 구조체를 생성하여 필드 값을 설정합니다.
		FString FieldsJson = FString::Printf(TEXT(
		"{"
		"\"fields\": {"
		"\"GunName\": {\"stringValue\": \"red\"},"
		"\"ID\": {\"stringValue\": \"%s\"},"
		"\"SpawnLocation_X\": {\"doubleValue\": %f},"
		"\"SpawnLocation_Y\": {\"doubleValue\": %f},"
		"\"SpawnLocation_Z\": {\"doubleValue\": %f},"
		"\"SpawnRotator_Pitch\": {\"doubleValue\": %f},"
		"\"SpawnRotator_Roll\": {\"doubleValue\": %f},"
		"\"SpawnRotator_Yaw\": {\"doubleValue\": %f},"
		"\"SwordName\": {\"stringValue\": \"blue\"},"
		"\"Items\": {\"arrayValue\": {\"values\": [%s]}},"
		"\"Money\": {\"integerValue\": %d},"
		"\"Health\": {\"doubleValue\": %f},"
		"\"MaxHealth\": {\"doubleValue\": %f},"
		"\"IsQuestAccept\": {\"integerValue\": %d}"

		"}"
		"}"), *Login_ID,
		SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z,
		SpawnRotator.Pitch, SpawnRotator.Roll, SpawnRotator.Yaw
		,*ItemJson, MyMoney, MyHealth, MyMaxHealth, _IsQuestAccept
		);
//
		Request->SetContentAsString(FieldsJson);
		Request->OnProcessRequestComplete().BindUObject(this, &UNetwork_Manager_R::Callback_ForceExit);
		if (!Request->ProcessRequest())
		{
			////UE_LOG(LogTemp, Log, TEXT("Failed to process HTTP request for updating spawn data."));
		}
	}
	else
	{
		////UE_LOG(LogTemp, Log, TEXT("HTTP Module not found for update."));
	}
}

void UNetwork_Manager_R::InsertSpawnData()
{
	////UE_LOG(LogTemp, Log, TEXT("InsertSpawnData..."));
	FHttpModule* Http = &FHttpModule::Get();
	if (Http)
	{
		const APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		const UInventoryComponent* PlayerInventory = CurrentCharacter->GetInventory();
		uint64 MyMoney = PlayerInventory->GetMoney();
		float MyHealth = CurrentCharacter->GetCurrentHealth();
		float MyMaxHealth = CurrentCharacter->GetMaxHealth();
		int8 _IsQuestAccept = CurrentCharacter->IsQuestAccept;
		FString ItemJson = TEXT("[");
		

		const TArray<UItemBase*>& InventoryItems = PlayerInventory->GetInventory();
		
		for (int32 i = 0; i < InventoryItems.Num(); ++i)
		{
			UItemBase* const& InventoryItem = InventoryItems[i];
			ItemJson += FString::Printf(TEXT(
				"{"
				"\"mapValue\": {"
				"\"fields\": {"
				"\"key\": {\"stringValue\": \"%s\"},"
				"\"quantity\": {\"integerValue\": %d}"
				"}"
				"}"
				"}"), 
				*InventoryItem->BaseItemID.ToString(), InventoryItem->BaseItemQuantity
			);
    
			if (i < InventoryItems.Num() - 1)
			{
				ItemJson += TEXT(",");
			}
		}

		
		ItemJson += TEXT("]");

		//UE_LOG(LogTemp,Log,TEXT(" ITEMJSON : %s "),*ItemJson);
		
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
		Request->SetVerb("POST");
		Request->SetURL(TEXT("https://firestore.googleapis.com/v1/projects/unrealrpg-7a179/databases/(default)/documents/Spawn"));
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

		// JSON 문자열로 필드 값을 설정합니다.
		FString InsertJson = FString::Printf(TEXT(
			"{"
			"\"fields\": {"
			"\"GunName\": {\"stringValue\": \"red\"},"
			"\"ID\": {\"stringValue\": \"%s\"},"
			"\"SpawnLocation_X\": {\"doubleValue\": %f},"
			"\"SpawnLocation_Y\": {\"doubleValue\": %f},"
			"\"SpawnLocation_Z\": {\"doubleValue\": %f},"
			"\"SpawnRotator_Pitch\": {\"doubleValue\": %f},"
			"\"SpawnRotator_Roll\": {\"doubleValue\": %f},"
			"\"SpawnRotator_Yaw\": {\"doubleValue\": %f},"
			"\"SwordName\": {\"stringValue\": \"blue\"},"
			"\"Items\": {\"arrayValue\": {\"values\": [%s]}},"
			"\"Money\": {\"integerValue\": %d},"
			"\"Health\": {\"doubleValue\": %f},"
			"\"MaxHealth\": {\"doubleValue\": %f},"
			"\"IsQuestAccept\": {\"integerValue\": %d}"
			"}"
			"}"), *Login_ID,
			SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z,
			SpawnRotator.Pitch, SpawnRotator.Roll, SpawnRotator.Yaw
			,*ItemJson, MyMoney, MyHealth, MyMaxHealth, _IsQuestAccept
			);

		Request->SetContentAsString(InsertJson);
		Request->OnProcessRequestComplete().BindUObject(this, &UNetwork_Manager_R::Callback_ForceExit);
		if (!Request->ProcessRequest())
		{
			// 오류 처리
			////UE_LOG(LogTemp, Error, TEXT("flag1..."));
		}
	}
	else
	{
			////UE_LOG(LogTemp, Error, TEXT("flag2..."));
		// Http 모듈 로드 실패 처리
	}
}

void UNetwork_Manager_R::Callback_ForceExit(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	////UE_LOG(LogTemp, Error, TEXT("Callback_ForceExit..."));
	// 플레이어 캐릭터(폰)의 참조를 얻습니다.
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(PlayerController)
	{
		UKismetSystemLibrary::QuitGame(
			this, // WorldContextObject
			PlayerController, // Optional: A player controller to specify which player is quitting.
			EQuitPreference::Quit, // How the game should quit. EQuitPreference::Quit is typical.
			false // Ignore unsaved changes in the editor. Only relevant in the editor.
		);
	}
}


void UNetwork_Manager_R::OnSequenceFinished()
{
	//UE_LOG(LogTemp, Error, TEXT("시퀀스 콜백"));

	LoadStartAsset();

	// if(GetWorld()->GetFirstPlayerController())
	// {
	//     ALoginController* MyController = Cast<ALoginController>(GetWorld()->GetFirstPlayerController());
	//     ////UE_LOG(LogTemp, Error, TEXT("얘 컨트롤러꺼를 바꿔야되는게 맞아 : %s" ), GetWorld()->GetFirstPlayerController()->GetName());
	//     MyController->ChangePawn(MyController->INDEX_OF_PLAYER_CONTROLLER);
	//     //CallSpawn(MyController->INDEX_OF_PLAYER_CONTROLLER);
	// }

	if (GetFirstLocalPlayerController())
	{
		ALoginController* MyController = Cast<ALoginController>(GetFirstLocalPlayerController());
		////UE_LOG(LogTemp, Error, TEXT("얘 컨트롤러꺼를 바꿔야되는게 맞아 : %s"), *GetFirstLocalPlayerController()->GetName());
		////UE_LOG(LogTemp, Error, TEXT("ITS OLDB %s %s %s %s %s"), *SpawnLocation.ToCompactString(), *SpawnRotator.ToCompactString(), *GunName, *Login_ID, *SwordName);
		MyController->ChangePawn(MyController->INDEX_OF_PLAYER_CONTROLLER, SpawnLocation, SpawnRotator, MaxHealthFromServer, CurrentHealthFromServer);
		//MyController->Login_ID = Login_ID;
		MyController->SetLoginID(Login_ID);
		//CallSpawn(MyController->INDEX_OF_PLAYER_CONTROLLER);

		Player_Widget = CreateWidget(this, Player_Widget_Class);
		Player_Widget->AddToViewport();
		AInventoryHUD* HUD = Cast<AInventoryHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		HUD->SetSkillWidget(Player_Widget);
		Respawn_Widget = CreateWidget(this, Respawn_Widget_Class);
		Respawn_Widget->AddToViewport();
		Respawn_Widget->SetVisibility(ESlateVisibility::Collapsed);
		Cast<APlayerCharacter>(MyController->GetPawn())->IsQuestAccept = IsQuestAccept;

		
		////서버 전용 함수 기능
		//if (MyController->GetPawn()->GetLocalRole() == ROLE_Authority)
		//{
		//	CreateSession(MyController->GetLocalPlayer(), 100, false);
		//}
		////클라이언트 전용 함수 기능
		//if (MyController->GetPawn()->IsLocallyControlled())
		//{
		//	////UE_LOG(LogTemp, Display, TEXT("아직이야"));
		//}
		

	}

}

bool UNetwork_Manager_R::CreateSession(ULocalPlayer* Player, int32 NumPublicConnections, bool bIsLAN)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
		if (Sessions.IsValid() && Player)
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsLANMatch = bIsLAN;
			SessionSettings.NumPublicConnections = NumPublicConnections;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.bUsesPresence = true;
			SessionSettings.bUseLobbiesIfAvailable = true;

			SessionSettings.Set(TEXT("MapName"), FString("L_greek_island"), EOnlineDataAdvertisementType::ViaOnlineService);

			FOnCreateSessionCompleteDelegate SessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UNetwork_Manager_R::OnCreateSessionComplete);
			Sessions->AddOnCreateSessionCompleteDelegate_Handle(SessionCompleteDelegate);

			return Sessions->CreateSession(*Player->GetPreferredUniqueNetId(), NAME_GameSession, SessionSettings);
		}
	}
	return false;
}

void UNetwork_Manager_R::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	// 여기에서 세션 생성 완료 후 처리할 로직을 구현합니다.
	if (bWasSuccessful)
	{
		// 세션 생성 성공 처리
		////UE_LOG(LogTemp, Log, TEXT("Create Success!!"));
	}
	else
	{
		// 세션 생성 실패 처리
		////UE_LOG(LogTemp, Log, TEXT("Create Fail"));
	}
}


//void UNetwork_Manager_R::FindSessions(APlayerController* PlayerController, bool bIsLAN, int32 MaxSearchResults)
//{
//	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
//
//	if (OnlineSubsystem)
//	{
//		IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
//		if (Sessions.IsValid() && PlayerController)
//		{
//			SessionSearch = MakeShareable(new FOnlineSessionSearch());
//			SessionSearch->bIsLanQuery = bIsLAN;
//			SessionSearch->MaxSearchResults = MaxSearchResults;
//			SessionSearch->PingBucketSize = 50;
//			SessionSearch->QuerySettings.Set(TEXT("MapName"), true, EOnlineComparisonOp::Equals);
//
//			Sessions->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UNetwork_Manager_R::OnFindSessionsComplete));
//			Sessions->FindSessions(*PlayerController->GetPlayerState()->GetUniqueId(), SessionSearch.ToSharedRef());
//		}
//	}
//}
//
//void UNetwork_Manager_R::OnFindSessionsComplete(bool bWasSuccessful)
//{
//	if (bWasSuccessful && SessionSearch.IsValid())
//	{
//		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
//		{
//			// 여기에서 각 세션 결과를 처리합니다. 예를 들면, 결과를 UI에 표시할 수 있습니다.
//		}
//	}
//
//	// 세션 검색이 완료된 후 필요한 정리 작업을 수행합니다.
//}

//void JoinSession(APlayerController* PlayerController, const FOnlineSessionSearchResult& SearchResult)
//{
//	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
//	if (OnlineSub)
//	{
//		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
//		if (Sessions.IsValid() && PlayerController)
//		{
//			Sessions->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMyGameSession::OnJoinSessionComplete));
//			Sessions->JoinSession(*PlayerController->GetPlayerId(), NAME_GameSession, SearchResult);
//		}
//	}
//}
//
//void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
//{
//	// Handle session join result
//}

void UNetwork_Manager_R::CallSpawn_Implementation(int Player_Idx)
{
	//////UE_LOG(LogTemp, Error, TEXT("콜 스폰 -->  %d 번 %d"), Player_Idx, UGameplayStatics::GetNumPlayerControllers(this));
	AGameModeBase* MyModeBase = GetWorld()->GetAuthGameMode();

	UWorld* World = GetWorld(); // 현재 GameInstance가 참조하는 월드를 얻습니다.
	if (!World) return;

	// 현재 실행 환경이 서버인지 클라이언트인지 확인
	FString Role = World->GetNetMode() == NM_DedicatedServer || World->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	////UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *Role);

	// 게임 내의 모든 플레이어 컨트롤러의 수를 로깅
	int32 NumPlayerControllers = World->GetNumPlayerControllers();
	////UE_LOG(LogTemp, Log, TEXT("게임 내의 플레이어 컨트롤러 수: %d"), NumPlayerControllers);

	// 모든 플레이어 컨트롤러를 순회하며 로깅
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			////UE_LOG(LogTemp, Log, TEXT("플레이어 컨트롤러: %s"), *PC->GetName());
		}
	}


	return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 1);


	// 여기서 폰 생성 로직을 구현합니다.
	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(SpawnPawn, SpawnLocation, FQuat::Identity.Rotator());
	if (NewPawn)
	{
		//APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		//////UE_LOG(LogTemp, Error, TEXT("빙의도 진해됐긴했어 -->  %d 번"), Player_Idx);
		PC->Possess(NewPawn);
	}

	//////UE_LOG(LogTemp, Error, TEXT("Call Spawn 서버에서 했겠지."));

	// if (PlayerController != nullptr)
	// {
	// 	////UE_LOG(LogTemp, Error, TEXT("CallSpawn init"));
	// 	// 새 Pawn 클래스의 인스턴스를 생성합니다.
	// 	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(SpawnPawn, SpawnLocation, FQuat::Identity.Rotator());
 //        
	// 	// 새로 생성된 Pawn을 PlayerController에 빙의시킵니다.
	// 	PlayerController->Possess(NewPawn);
	// }
}

