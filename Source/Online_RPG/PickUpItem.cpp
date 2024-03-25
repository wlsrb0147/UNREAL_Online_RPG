// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItem.h"

#include "AItemManager.h"
#include "InventoryComponent.h"
#include "ItemBase.h"
#include "ItemC.h"
#include "Network_Manager_R.h"
#include "Net/UnrealNetwork.h"

// Sets default values
//APickUpItem::APickUpItem(int32 InitialQuantity)
//{
//	SetReplicateMovement(true);
//	SetReplicates(true);
//	
//	bReplicates = true;
//	// 드랍템
// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = false;
//
//	InstanceMesh = CreateDefaultSubobject<UStaticMeshComponent>("CurrentMesh");
//	InstanceMesh->SetSimulatePhysics(true);
//	SetRootComponent(InstanceMesh);
//
//    if (InitialQuantity <=0) InstanceItemQuantity = 1;
//    else InstanceItemQuantity = InitialQuantity;
//	
//	InteractionData.InteractionType = EInteractionType::PickUp;
//	
//	if (bReplicates) {
//		UE_LOG(LogTemp, Log, TEXT(" item init by repli"));
//	}
//	else {
//		UE_LOG(LogTemp, Log, TEXT(" item init by XXXX repli"));
//	}
//	
//}

APickUpItem::APickUpItem()
{
	SetReplicateMovement(true);
	//SetReplicates(true);
	bReplicates = true;
	// 엔피씨
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InstanceMesh = CreateDefaultSubobject<UStaticMeshComponent>("CurrentMesh");
	InstanceMesh->SetSimulatePhysics(true);
	SetRootComponent(InstanceMesh);
	if (bReplicates) {
		UE_LOG(LogTemp, Log, TEXT(" item init by repli"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT(" item init by XXXX repli"));
	}
	
}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	//InitializeItem(UItemBase::StaticClass(),InstanceItemQuantity);
	if (bReplicates) {
		UE_LOG(LogTemp, Log, TEXT(" item begin by repli"));
		FString _Role = GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
		UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT(" item begin by XXXX repli"));
		FString _Role = GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
		UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);
	}

	float CurrentZRotation = InstanceMesh->GetComponentRotation().Yaw;

	// X와 Y 회전값을 0으로, Z 회전값은 현재 값으로 설정
	InstanceMesh->SetSimulatePhysics(true);
	FRotator DesiredRotation = FRotator(0.0f, CurrentZRotation,0.0f );
	InstanceMesh->SetWorldRotation(DesiredRotation);

}

void APickUpItem::InitializeItem(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	UE_LOG(LogTemp, Warning, TEXT("비긴 이니셜1"));
	if (InstanceItemDataTable) {
		UE_LOG(LogTemp, Warning, TEXT("flag1"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("flag3"));
	}
	if (InstanceItemID.IsNone()) {

		UE_LOG(LogTemp, Warning, TEXT("flag2"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("flag4"));
	}

	if (InstanceItemDataTable && !InstanceItemID.IsNone())
	{
		const FItemData* ItemData = InstanceItemDataTable->FindRow<FItemData>(InstanceItemID,TEXT("Error"));
		
		InstanceItemData = NewObject<UItemBase>(this,BaseClass);
		
		InstanceItemData->BaseItemID = ItemData->ItemID;
		InstanceItemData->BaseItemType = ItemData->ItemType;
		InstanceItemData->BaseItemNumericData = ItemData->ItemNumericData;
		InstanceItemData->BaseItemTextData = ItemData->ItemTextData;
		InstanceItemData->BaseItemAssetData = ItemData->ItemAssetData;
		InstanceItemData->BaseItemStatistics = ItemData->ItemStatistics;
		
		if (InQuantity <=0) InstanceItemData->SetQuantity(1);
		else InstanceItemData->BaseItemQuantity = InQuantity;

		if (InstanceItemData->BaseItemNumericData.bIsStackable && InstanceItemData->BaseItemNumericData.MaxStackSize < 2 )
		{
			InstanceItemData->BaseItemNumericData.MaxStackSize = 2;
		}

		InstanceMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);
		UpdateItemInteractionData();

		UE_LOG(LogTemp, Log, TEXT(" mesh .. %s"), *ItemData->ItemAssetData.Mesh->GetName());
		
	}

}

void APickUpItem::UpdateItemInteractionData()
{
	// InteractionData는 Interface에 있는 정보
	// InstanceItemInteractData는 PickUpItem에 있는 정보
	// 정보 탐색은 interface에서 하고있음
	InstanceItemInteractData.Name = InstanceItemData->BaseItemTextData.NameText;
	InstanceItemInteractData.Quantity = InstanceItemData->BaseItemQuantity;
	this->InstanceItemQuantity = InstanceItemData->BaseItemQuantity;
	InteractionData = InstanceItemInteractData;
}

void APickUpItem::ServerDestroyActor_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("실행됨!!!!!!777"));
	Destroy();
}

void APickUpItem::ServerRequestSetOwner_Implementation(APlayerController* NewOwner)
{
	UE_LOG(LogTemp, Log, TEXT("서버에서 setowner_0"));
	if (NewOwner)
	{
		UE_LOG(LogTemp, Log, TEXT("서버에서 setowner"));
		SetOwner(NewOwner);
	}
}

bool APickUpItem::ServerRequestSetOwner_Validate(APlayerController* NewOwner)
{
	return true; // 간단한 유효성 검사 예제
}

void APickUpItem::OnRep_ReplicatedOwner()
{
	UE_LOG(LogTemp, Log, TEXT("Owner 바뀐거 감지!!!"));
	SetOwner(ReplicatedOwner);
	if (ReplicatedOwner) {
		UE_LOG(LogTemp, Log, TEXT("Owner 바뀐거 감지2!!! %s"), *ReplicatedOwner->GetName());
	}
	//SetOwner(GetWorld()->GetFirstPlayerController());
}

void APickUpItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickUpItem, a);
	DOREPLIFETIME(APickUpItem, ReplicatedOwner);
}

void APickUpItem::BeginFocus()
{
	IItemInteractionInterface::BeginFocus();
}

void APickUpItem::EndFocus()
{
	IItemInteractionInterface::EndFocus();
}

void APickUpItem::Interact(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter) PickUpItem(PlayerCharacter);
}

void APickUpItem::PickUpItem(const APlayerCharacter* Taker)
{
	
	
	if (IsPendingKillPending()) return;

	if (!InstanceItemData) return;
	// 아이템 액터의 소유자를 설정
	// 아이템을 픽업하는 로직을 처리하는 함수 내에서
	//SetOwner(Taker->GetController());
	//SetOwner(GetWorld()->GetFirstPlayerController());
	if (GetOwner())
	{
		// 객체에 Owner가 설정되어 있는 경우, Owner의 이름을 로그에 출력
		UE_LOG(LogTemp, Log, TEXT("The owner of this object is: %s"), *GetOwner()->GetName());

		// Owner의 타입이 PlayerController인지 확인
		APlayerController* PC = Cast<APlayerController>(GetOwner());
		if (PC)
		{
			// Owner가 PlayerController인 경우, 추가 정보 로그에 출력
			UE_LOG(LogTemp, Log, TEXT("The owner is a PlayerController. Player Name: %s"), *PC->GetPawn()->GetName());
		}
	}
	else
	{
		// Owner가 설정되지 않은 경우
		UE_LOG(LogTemp, Warning, TEXT("This object does not have an owner."));
	}
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Display, TEXT(" ppp "));
		// 서버에서 실행 중인 경우, 직접 소유자를 설정할 수 있습니다.
		SetOwner(Taker->GetController());
		//SetOwner(GetWorld()->GetFirstPlayerController());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT(" kkk "));
		// 클라이언트에서 실행 중인 경우, 서버에 소유자 변경을 요청합니다.
		ServerRequestSetOwner(Cast<APlayerController>(Taker->GetController()));
		//ServerRequestSetOwner(GetWorld()->GetFirstPlayerController());
	}
	if (HasAuthority()) {
		// 서버에서 실행 중인 경우, 플레이어 컨트롤러를 소유자로 설정
		SetOwner(Taker->GetController());
	}
	UE_LOG(LogTemp, Display, TEXT(" flag777 "));
	
	FString _Role = GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);

	if (UInventoryComponent* PlayerInventory = Taker->GetInventory())
	{
		const FItemAddResultData AddResult = PlayerInventory->HandleAddItem(InstanceItemData);

		switch (AddResult.AddResult) {
		case EItemAddOperationResult::IAR_NoItemAdded:
			break;
		case EItemAddOperationResult::IAR_PartialAmountItemAdded:
			UpdateItemInteractionData();
			
			Taker->UpdateInteractionWidget();
			break;
		case EItemAddOperationResult::IAR_AllItemAdded:
			//Destroy();
			UE_LOG(LogTemp, Display, TEXT(" flag999 "));
			
			ServerDestroyActor();
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Default실행"));
			
		}
	}
	UE_LOG(LogTemp, Display, TEXT(" flag888 "));
}

void APickUpItem::InitializeDropItem_Implementation(int32 ItemToDrop, const int32 Quantity)
{
	const AItemManager* ItemManager = Cast<UNetwork_Manager_R>(GetGameInstance())->GetItemManager();
	
	UItemBase* Base = ItemManager->MakeItemBaseByKey(this,ItemToDrop,Quantity);
	FString _Role = GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("서버") : TEXT("클라이언트");
	UE_LOG(LogTemp, Log, TEXT("현재 실행 환경: %s"), *_Role);
	UE_LOG(LogTemp, Log, TEXT("InitializeDropItem... %d"), ItemToDrop);
	InstanceItemData = Base;
	InstanceItemData->SetQuantity(Quantity);
	InstanceItemData->OwningInventory = nullptr;
	InstanceMesh->SetStaticMesh(Base->BaseItemAssetData.Mesh);
	UE_LOG(LogTemp, Log, TEXT(" mesh .. %s"), *Base->BaseItemAssetData.Mesh->GetName());
	
	
	UpdateItemInteractionData();
}

void APickUpItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property?
		PropertyChangedEvent.Property->GetFName() : NAME_None;
	
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickUpItem,InstanceItemID))
	{
		if (InstanceItemDataTable)
		{
			const FItemData* ChangeItemData =
				InstanceItemDataTable->FindRow<FItemData>(InstanceItemID,"Error");
			if (ChangeItemData)
			{
				InstanceMesh->SetStaticMesh(ChangeItemData->ItemAssetData.Mesh);
			}
		}
	}
}

