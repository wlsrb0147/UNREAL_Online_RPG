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
	
	SelectUser(TEXT("fffl"),TEXT("fppp1"));

	InsertUser(TEXT("fffl"), TEXT("fppp1"));
	
}





void UNetwork_Manager_R::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			UE_LOG(LogTemp, Log, TEXT("HTTP Request was successful. Response: %s"), *Response->GetContentAsString());
			// Parse the JSON response if needed
			// ... rest of your code for JSON parsing
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

void UNetwork_Manager_R::InsertUser(const FString& Username, const FString& Password)
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
		FString JsonData = FString::Printf(TEXT("{\"fields\": {\"ID\": {\"stringValue\": \"%s\"}, \"PW\": {\"stringValue\": \"%s\"}}}"),  *Username, *Password);
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