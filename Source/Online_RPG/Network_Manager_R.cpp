// Fill out your copyright notice in the Description page of Project Settings.

#include "Network_Manager_R.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"


const char *host = "localhost";
const char *user = "red";
const char *pw = "red";
const char *db = "mydb";

UNetwork_Manager_R::UNetwork_Manager_R()
{
	
	//SelectUser(TEXT("fffl"),TEXT("fppp1"));

	InsertUser(TEXT("RED77"), TEXT("RED77"), TEXT("REDRED77"));
	
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

void UNetwork_Manager_R::SelectUser(const FString& Username, const FString& Password)
{
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
		UE_LOG(LogTemp, Error, TEXT("Failed to get HTTP module!"));
	}
}

void UNetwork_Manager_R::ResistUser(const FString& Username, const FString& Password, const FString& NickName)
{
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
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HTTP Request returned status code: %d"), Response->GetResponseCode());
			// Handle error
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Request was not successful."));
		// Handle error
	}
}
