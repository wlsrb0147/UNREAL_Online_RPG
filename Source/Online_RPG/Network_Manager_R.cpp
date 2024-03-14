// Fill out your copyright notice in the Description page of Project Settings.

#include "Network_Manager_R.h"

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


const char *host = "localhost";
const char *user = "red";
const char *pw = "red";
const char *db = "mydb";

UNetwork_Manager_R::UNetwork_Manager_R()
{
	
	//SelectUser(TEXT("fffl"),TEXT("fppp1"));

	//InsertUser(TEXT("RED77"), TEXT("RED77"), TEXT("REDRED77"));
	//ULevelSequence* LevelSequence = LoadObject<ULevelSequence>(nullptr, TEXT("/Game/Path/To/YourSequence.YourSequence"));

	Level_Sequence = LoadObject<ULevelSequence>(nullptr, TEXT("/Game/MAIN/Sequence/SQ_GameStart.SQ_GameStart"));
	if(Level_Sequence == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Nope11"));
		// 에셋을 찾지 못한 경우 에러 처리
		return;
	}
	
	
	
	
}


ASound_Manager_R* UNetwork_Manager_R::Get_Sound_Instance()
{
	FActorSpawnParameters SpawnParams;
	if(!Sound_Instance)
	Sound_Instance = GetWorld()->SpawnActor<ASound_Manager_R>(Sound_Class, FVector(0,0,0), FRotator(0,0,0), SpawnParams);

	return Sound_Instance;
}

void UNetwork_Manager_R::LoadStartAsset()
{
	UE_LOG(LogTemp, Error, TEXT("LoadStartAsset"));
	// 사운드 에셋 비동기 로드
	FPrimaryAssetId SoundAssetId("UPostLoadAsset", TEXT("BP_PostLoadAsset"));
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	// FPrimaryAssetId에서 FSoftObjectPath 배열을 얻어내는 과정
	TArray<FSoftObjectPath> AssetPaths;
	FSoftObjectPath AssetPath = UAssetManager::Get().GetPrimaryAssetPath(SoundAssetId);
	AssetPaths.Add(AssetPath);
	if (!AssetPath.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("AssetPath is not valid."));
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("AssetPath is  valid. 777"));
	}
	// 비동기 로드 콜백
	auto OnLoadComplete = [this]()
	{
		//UPostLoadAsset* SoundAsset = Cast<UPostLoadAsset>(LoadedAsset);
		UE_LOG(LogTemp, Warning, TEXT("UPostLoadAsset Load 성공"));
		// if (SoundAsset && SoundAsset->Sound1) // Sound1을 예로 듭니다. 필요에 따라 Sound2 등 다른 사운드 참조 가능
		// {
		// 	UGameplayStatics::PlaySoundAtLocation(this, SoundAsset->Sound1, GetActorLocation());
		// }
	};

	// 비동기적으로 사운드 에셋 로드 요청
	Streamable.RequestAsyncLoad(AssetPath, FStreamableDelegate::CreateLambda(OnLoadComplete));
	//Streamable.RequestAsyncLoad(AssetPaths,FStreamableDelegate::CreateLambda(OnLoadComplete));
	//Streamable.RequestAsyncLoad(SoundAssetId, FStreamableDelegate::CreateLambda(OnLoadComplete));
	

	/////////////////
	
	// UAssetManager& __Manager = UAssetManager::Get();
	//
	// TArray<FPrimaryAssetId> Assets;
	// __Manager.GetPrimaryAssetIdList(TEXT("PostLoadAsset"), Assets);
	//
	//
	// if(Assets.Num() > 0)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Asset Num > 0"));
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
	// 		UE_LOG(LogTemp, Warning, TEXT("Asset loaded successfully!"));
	// 	}
	// 	else UE_LOG(LogTemp, Warning, TEXT("Asset loaded fail..."));
	// }
	// else UE_LOG(LogTemp, Warning, TEXT("Asset Num = 0"));
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
	// 	UE_LOG(LogTemp, Warning, TEXT("Asset loaded successfully!"));
	// });
	//
	// // 비동기적으로 에셋 로드
	// StreamableManager.RequestAsyncLoad(AssetRef, Callback);
}

void UNetwork_Manager_R::Sound_Play(SOUND_TYPE Sound_Type, int32 Audio_idx, FVector Location, FRotator Rotator, APawn* MyPawn)
{
	if(!Sound_Instance) Get_Sound_Instance();
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
			UE_LOG(LogTemp, Error, TEXT("response...  %s"), *Response->GetContentAsString());
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
								if(1==2)
								{
									//Join 시도
									
								}else
								{
									//이미 있는것이니 성공 띄우고 몇초 뒤에 로그인 시켜버리면됨
									UE_LOG(LogTemp, Error, TEXT("성공창 "));
									if(Current_Widget)
									{
										UE_LOG(LogTemp, Error, TEXT("성공해서 위젯 없애버려볼까 "));
										Current_Widget->RemoveFromParent();
										if(Success_Widget)
										{
											GetSpawnData();
											Sound_Play(SOUND_TYPE::Btn_GameStart, 1, FVector(0,0,0), FRotator(0,0,0));
											
											// UUserWidget* TmpWidget = CreateWidget<UUserWidget>(this, Success_Widget, TEXT("SuccessWidget"));
											// if(TmpWidget)
											// TmpWidget->AddToViewport();
											// UE_LOG(LogTemp, Log, TEXT("성공창 띠ㅡ웠음"));
										}
									}
								
									// "name" 필드 값을 가져옵니다.
									FString Name = DocumentObject->GetStringField(TEXT("name"));
									UE_LOG(LogTemp, Log, TEXT("Document Name: %s"), *Name);

									// "fields" 객체 내의 "ID"와 "PW" 필드 값을 가져옵니다.
									TSharedPtr<FJsonObject> FieldsObject = DocumentObject->GetObjectField(TEXT("fields"));
									if (FieldsObject.IsValid())
									{
										FString ID = FieldsObject->GetObjectField(TEXT("ID"))->GetStringField(TEXT("stringValue"));
										FString PW = FieldsObject->GetObjectField(TEXT("PW"))->GetStringField(TEXT("stringValue"));
										UE_LOG(LogTemp, Log, TEXT("ID: %s, PW: %s"), *ID, *PW);
									}
								}
								

								return;
							}
						}else
						{
							UE_LOG(LogTemp, Error, TEXT("이럼 없는 거임"));
						}
						
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response."));
			}
			//TSharedPtr<FJsonObject> JsonObject;
			//TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

			UE_LOG(LogTemp, Log, TEXT("HTTP Request was successful. Response: %s"), *Response->GetContentAsString());
			// Parse the JSON response if needed
			// ... rest of your code for JSON parsing
			// Assuming the query results are in an array named "results"
			 //TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HTTP Request returned status code: %d"), Response->GetResponseCode());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Request was not successful."));
	}


	//여기에 실패 띄우기
	UE_LOG(LogTemp, Error, TEXT("실패창 "));
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
			UE_LOG(LogTemp, Error, TEXT("response...  %s"), *Response->GetContentAsString());
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
								UE_LOG(LogTemp, Error, TEXT("이미 존재하는 아이디라 실패"));
								CreateWidget_OX(false);

								return;
							}
						}else
						{
							
							InsertUser(Join_ID, Join_PW, Join_NickName);
							return;
						}
						
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response."));
			}
			//TSharedPtr<FJsonObject> JsonObject;
			//TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

			UE_LOG(LogTemp, Log, TEXT("HTTP Request was successful. Response: %s"), *Response->GetContentAsString());
			// Parse the JSON response if needed
			// ... rest of your code for JSON parsing
			// Assuming the query results are in an array named "results"
			 //TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			
			
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HTTP Request returned status code: %d"), Response->GetResponseCode());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Request was not successful."));
	}


	//여기에 실패 띄우기
	UE_LOG(LogTemp, Error, TEXT("실패창 "));
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
			UE_LOG(LogTemp, Log, TEXT("Flag8"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Flag9"));
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
		Request->SetURL(TEXT("https://firestore.googleapis.com/v1/projects/unrealrpg-7a179/databases/(default)/documents/User")) ;
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

		// Create JSON object for user data
		//FString JsonData = FString::Printf(TEXT("{\"fields\": {\"ID\": {\"stringValue\": \"%s\"}, \"PW\": {\"stringValue\": \"%s\"}}}"), *Username, *Password);

		// Create JSON object for user data
		FString JsonData = FString::Printf(TEXT("{\"fields\": {\"ID\": {\"stringValue\": \"%s\"}, \"PW\": {\"stringValue\": \"%s\"}, \"NICK\" : {\"stringValue\" : \"%s\"}}}"),  *Username, *Password, *NickName);
		//FString JsonData = FString::Printf(TEXT("{\"name\": \"projects/unrealrpg-7a179/databases/(default)/documents/User/%s\",\"fields\": {\"ID\": {\"stringValue\": \"%s\"}, \"PW\": {\"stringValue\": \"%s\"}}}"), *Username, *Username, *Password);

		Request->SetContentAsString(JsonData);

		Request->OnProcessRequestComplete().BindUObject(this, &UNetwork_Manager_R::OnInsertUserResponseReceived);
		Request->ProcessRequest();
	}
	else
	{
		CreateWidget_OX(false);
		UE_LOG(LogTemp, Error, TEXT("Failed to get HTTP module!"));
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
			UE_LOG(LogTemp, Log, TEXT("Flag8"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Flag9"));
	}
}


void UNetwork_Manager_R::OnInsertUserResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			UE_LOG(LogTemp, Log, TEXT("User inserted successfully!"));
			// Handle success
			CreateWidget_OX(true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HTTP Request returned status code: %d"), Response->GetResponseCode());
			CreateWidget_OX(false);
			// Handle error
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Request was not successful."));
		CreateWidget_OX(false);
		// Handle error
	}
}

void UNetwork_Manager_R::CreateWidget_OX(bool bIsSuccess)
{

	if(bIsSuccess) Sound_Play(SOUND_TYPE::Btn_GameStart, 2, FVector(0,0,0), FRotator(0,0,0));
	else Sound_Play(SOUND_TYPE::Btn_Click_Fail, 2, FVector(0,0,0), FRotator(0,0,0));

	UUserWidget* TmpWidget;
	FString UniqueID = FGuid::NewGuid().ToString();
	FName UniqueName = FName(*(FString("SuccessWidget_") + UniqueID));
	if(bIsSuccess)
		TmpWidget = CreateWidget<UUserWidget>(this, Success_Widget, UniqueName);
	else
		TmpWidget = CreateWidget<UUserWidget>(this, Fail_Widget, UniqueName);

	if(TmpWidget)
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
	*Login_ID); // 'IdToSearch'는 검색하고자 하는 ID 값으로, 적절한 변수로 대체해야 합니다.

		Request->SetContentAsString(QueryJson);

		
		// Set any other necessary headers here, like Authorization if needed

		Request->OnProcessRequestComplete().BindUObject(this, &UNetwork_Manager_R::GetSpawnData_CallBack);
		if (!Request->ProcessRequest())
		{
			UE_LOG(LogTemp, Log, TEXT("Flag8"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Flag9"));
	}
	
}

void UNetwork_Manager_R::GetSpawnData_CallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Error, TEXT("       spawn data Callback      "));
	if (bWasSuccessful && Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			// JSON 응답 파싱을 시도합니다.

			// JSON 응답을 배열로 파싱 시도합니다.
			TSharedPtr<FJsonValue> JsonValue;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			UE_LOG(LogTemp, Error, TEXT("response...  %s"), *Response->GetContentAsString());
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
									UE_LOG(LogTemp, Error, TEXT("fields get!!!!!!!!!!!!"));
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

									SpawnLocation = FVector(SpawnLocation_X,SpawnLocation_Y,SpawnLocation_Z);

									// SpawnRotator 값을 double로 가져옵니다.
									double SpawnRotator_Pitch = SpawnRotator_Pitch_Object->GetNumberField(TEXT("doubleValue"));
									double SpawnRotator_Roll = SpawnRotator_Roll_Object->GetNumberField(TEXT("doubleValue"));
									double SpawnRotator_Yaw = SpawnRotator_Yaw_Object->GetNumberField(TEXT("doubleValue"));

									SpawnRotator = FRotator(SpawnRotator_Pitch,SpawnRotator_Yaw,SpawnRotator_Roll);

									// SwordName 값을 FString으로 가져옵니다.
									SwordName = SwordNameObject->GetStringField(TEXT("stringValue"));

									// GunName 값을 FString으로 가져옵니다.
									GunName = GunNameObject->GetStringField(TEXT("stringValue"));

									// ID 값을 FString으로 가져옵니다.
									FString ID = IDObject->GetStringField(TEXT("stringValue"));
									

								}
							}
						}else
						{
							UE_LOG(LogTemp, Error, TEXT("이럼 없는 거임"));
							
						}
					}
					Spawn_Init();
					
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response."));
			}
			//TSharedPtr<FJsonObject> JsonObject;
			//TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

			UE_LOG(LogTemp, Log, TEXT("HTTP Request was successful. Response: %s"), *Response->GetContentAsString());
			// Parse the JSON response if needed
			// ... rest of your code for JSON parsing
			// Assuming the query results are in an array named "results"
			 //TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
			
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HTTP Request returned status code: %d"), Response->GetResponseCode());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Request was not successful."));
	}


}

void UNetwork_Manager_R::Spawn_Init()
{
	if (SpawnLocation.IsNearlyZero() && SpawnRotator.IsNearlyZero())
	{
		// 변수가 '초기화되지 않았다'고 가정하고 처리
		UE_LOG(LogTemp, Error, TEXT("ITS NEWB"));
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("ITS OLDB %s %s %s %s %s"),*SpawnLocation.ToCompactString(), *SpawnRotator.ToCompactString(), *GunName, *Login_ID, *SwordName);
	}

	// 현재 월드를 가져옵니다.
	UWorld* World = GetWorld();
	UE_LOG(LogTemp, Error, TEXT("Nope22"));
	if(World != nullptr)
	{
		// 레벨 시퀀스 플레이어를 생성합니다.
		Player_Sequence = ULevelSequencePlayer::CreateLevelSequencePlayer(World, Level_Sequence, MovieSceneSequencePlaybackSettings, SequenceActor);
		UE_LOG(LogTemp, Error, TEXT("Nope33"));
	}
	UE_LOG(LogTemp, Error, TEXT("Nope44"));

	// 시퀀스 플레이어가 성공적으로 생성되었는지 확인하고 재생합니다.
	if(Player_Sequence != nullptr)
	{
		Player_Sequence->SetPlayRate(0.7f); // 재생 속도를 0.5로 설정
		Player_Sequence->OnFinished.AddDynamic(this, &UNetwork_Manager_R::OnSequenceFinished);
		Player_Sequence->Play();
		UE_LOG(LogTemp, Error, TEXT("Play!!!"));
		
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("Nope"));
	}
}

void UNetwork_Manager_R::OnSequenceFinished()
{
	UE_LOG(LogTemp, Error, TEXT("시퀀스 콜백"));

	LoadStartAsset();
	
	// if(GetWorld()->GetFirstPlayerController())
	// {
	// 	ALoginController* MyController = Cast<ALoginController>(GetWorld()->GetFirstPlayerController());
	// 	UE_LOG(LogTemp, Error, TEXT("얘 컨트롤러꺼를 바꿔야되는게 맞아 : %s" ), *GetWorld()->GetFirstPlayerController()->GetName());
	// 	MyController->ChangePawn(MyController->INDEX_OF_PLAYER_CONTROLLER);
	// 	//CallSpawn(MyController->INDEX_OF_PLAYER_CONTROLLER);
	// }
		
	if(GetFirstLocalPlayerController())
	{
		ALoginController* MyController = Cast<ALoginController>(GetFirstLocalPlayerController());
		UE_LOG(LogTemp, Error, TEXT("얘 컨트롤러꺼를 바꿔야되는게 맞아 : %s" ), *GetFirstLocalPlayerController()->GetName());
		MyController->ChangePawn(MyController->INDEX_OF_PLAYER_CONTROLLER);
		//CallSpawn(MyController->INDEX_OF_PLAYER_CONTROLLER);

		//Ingame BGM ON
		Sound_Play(SOUND_TYPE::BGM_Ingame, 1, FVector(0,0,0), FRotator(0,0,0));
		
	}
	
}

void UNetwork_Manager_R::CallSpawn_Implementation(int Player_Idx)
{
	UE_LOG(LogTemp, Error, TEXT("콜 스폰 -->  %d 번 %d"), Player_Idx, UGameplayStatics::GetNumPlayerControllers(this));
	AGameModeBase* MyModeBase = GetWorld()->GetAuthGameMode();

	UWorld* World = GetWorld(); // 현재 GameInstance가 참조하는 월드를 얻습니다.
	if (!World) return;

	// 현재 실행 환경이 서버인지 클라이언트인지 확인
	FString Role = World->GetNetMode() == NM_DedicatedServer || World->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *Role);

	// 게임 내의 모든 플레이어 컨트롤러의 수를 로깅
	int32 NumPlayerControllers = World->GetNumPlayerControllers();
	UE_LOG(LogTemp, Log, TEXT("게임 내의 플레이어 컨트롤러 수: %d"), NumPlayerControllers);

	// 모든 플레이어 컨트롤러를 순회하며 로깅
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			UE_LOG(LogTemp, Log, TEXT("플레이어 컨트롤러: %s"), *PC->GetName());
		}
	}

	
	return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 1);
	
	
	// 여기서 폰 생성 로직을 구현합니다.
	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(SpawnPawn, SpawnLocation, FQuat::Identity.Rotator());
	if (NewPawn)
	{
		//APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		UE_LOG(LogTemp, Error, TEXT("빙의도 진해됐긴했어 -->  %d 번"), Player_Idx);
		PC->Possess(NewPawn);
	}

	UE_LOG(LogTemp, Error, TEXT("Call Spawn 서버에서 했겠지."));
	
	// if (PlayerController != nullptr)
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("CallSpawn init"));
	// 	// 새 Pawn 클래스의 인스턴스를 생성합니다.
	// 	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(SpawnPawn, SpawnLocation, FQuat::Identity.Rotator());
 //        
	// 	// 새로 생성된 Pawn을 PlayerController에 빙의시킵니다.
	// 	PlayerController->Possess(NewPawn);
	// }
}

