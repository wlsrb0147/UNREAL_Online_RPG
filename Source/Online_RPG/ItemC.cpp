// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemC.h"

#include "InventoryComponent.h"
#include "InventoryHUD.h"
#include "ItemInteractionInterface.h"
#include "PickUpItem.h"

// Sets default values
AItemC::AItemC(): HUD(nullptr)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("인벤토리"));
	PlayerInventory->SetInventoryCapacity(24);
}

void AItemC::FoundNoInteract()
{
	if (InteractionData.CurrentInteracting)
	{
		if (IsValid(InteractionTarget.GetObject()))
		{
			InteractionTarget->EndFocus();
		}

		HUD->CloseInteractionWidget();

		InteractionData.CurrentInteracting = nullptr;
		InteractionTarget = nullptr;
	}
}

void AItemC::FoundInteract(AActor* NewInteract)
{
	
	if (InteractionData.CurrentInteracting)
	{
		InteractionTarget = InteractionData.CurrentInteracting;
		InteractionTarget->EndFocus();
	}
	
	InteractionData.CurrentInteracting = NewInteract;
	InteractionTarget = NewInteract;
	
	HUD->UpdateInteractionWidget(&InteractionTarget->InteractionData);
	InteractionTarget->BeginFocus();
}

void AItemC::BeginInteract()
{
	UE_LOG(LogTemp,Display,TEXT("%d"),PlayerInventory->GetInventoryCapacity())
	CheckInteraction();

	if (!InteractionData.CurrentInteracting) return;

	if (!IsValid(InteractionTarget.GetObject())) return;

	InteractionTarget->BeginInteract();
	Interact();
}

void AItemC::Interact()
{
	if (!IsValid(InteractionTarget.GetObject())) return;

	InteractionTarget->Interact(this);
	FoundNoInteract();
}

void AItemC::EndInteract()
{
	if (!IsValid(InteractionTarget.GetObject())) return;

	InteractionTarget->EndInteract();
}

void AItemC::OpenInventory()
{
	HUD->ToggleInventoryWidget();
}

void AItemC::UpdateInteractionWidget() const
{
	if (IsValid(InteractionTarget.GetObject()))
	{
		HUD->UpdateInteractionWidget(&InteractionTarget->InteractionData);
	}
}

void AItemC::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (!PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const FVector SpawnLocation{GetActorLocation() + GetActorForwardVector()*50.0f};
	const FTransform SpawnTransform(GetActorRotation(),SpawnLocation);
	const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop,QuantityToDrop);

	APickUpItem* Item = GetWorld()->SpawnActor<APickUpItem>(APickUpItem::StaticClass(),SpawnTransform,SpawnParameters);

	Item->InitializeDropItem(ItemToDrop,RemovedQuantity);
}

// Called when the game starts or when spawned
void AItemC::BeginPlay()
{
	Super::BeginPlay();
	HUD = Cast<AInventoryHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

// Called every frame
void AItemC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckInteraction();
}

void AItemC::CheckInteraction()
{
	FVector TraceStart{GetPawnViewLocation()};
	FVector TraceEnd{TraceStart + GetViewRotation().Vector() * InteractionDistance};
	
	DrawDebugLine(GetWorld(),TraceStart,TraceEnd,FColor::Red,false,-1,0,2);

	//FCollisionQueryParams
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	if (!GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd,
		ECC_Visibility,
		QueryParams)
		)
	{
		FoundNoInteract();
		return;
	}

	if (!HitResult.GetActor()->GetClass()->ImplementsInterface(UItemInteractionInterface::StaticClass())) return;

	if (HitResult.GetActor() != InteractionData.CurrentInteracting)
	{
		FoundInteract(HitResult.GetActor());
		return;
	}

	if (HitResult.GetActor() == InteractionData.CurrentInteracting)
	{
		return;
	}
}


// Called to bind functionality to input
void AItemC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact",IE_Pressed,this,&AItemC::BeginInteract);
	PlayerInputComponent->BindAction("Interact",IE_Released,this,&AItemC::EndInteract);

	PlayerInputComponent->BindAction("OpenInventory",IE_Pressed,this,&AItemC::OpenInventory);
}

