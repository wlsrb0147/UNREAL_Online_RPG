// Fill out your copyright notice in the Description page of Project Settings.


#include "Network_Manager_R.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"



UNetwork_Manager_R::UNetwork_Manager_R(): Socket(nullptr)
{
	UE_LOG(LogTemp, Log, TEXT("Network instance created."));
	const FString ServerIP = TEXT("192.168.0.102");
	int32 port = 8080;
	ConnectToServer(ServerIP, port);

	//
}

bool UNetwork_Manager_R::ConnectToServer(const FString& IP, int32 Port)
{
	return false;
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
    
	FIPv4Address Addr;
	FIPv4Address::Parse(IP, Addr);
	TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Address->SetIp(Addr.Value);
	Address->SetPort(Port);
    
	bool Connected = Socket->Connect(*Address);
	if (Connected)
	{
		UE_LOG(LogTemp, Log, TEXT("Connected to server."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to connect to server."));
	}
    
	return Connected;
}

void UNetwork_Manager_R::SendMessageToServer(const FString& Message)
{
	if (!Socket) return;

	const TCHAR *SerializedChar = Message.GetCharArray().GetData();
	int32 Size = FCString::Strlen(SerializedChar) + 1;
	int32 Sent = 0;

	bool Successful = Socket->Send((uint8*)TCHAR_TO_UTF8(SerializedChar), Size, Sent);
}

void UNetwork_Manager_R::ReceiveMessageFromServer()
{
	if (!Socket) return;

	// 이 부분은 네트워킹 스레드에서 비동기적으로 수행되어야 합니다.
	// 여기서는 간단한 동기적 예제만 제공합니다.

	uint32 Size;
	while (Socket->HasPendingData(Size))
	{
		TArray<uint8> ReceivedData;
		ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));

		int32 Read = 0;
		Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
        
		// 데이터를 문자열로 변환하여 처리합니다.
		// ...
	}
}