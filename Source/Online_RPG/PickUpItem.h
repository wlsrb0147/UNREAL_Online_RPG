// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "PickUpItem.generated.h"

class UItemBase;

UCLASS()
class ONLINE_RPG_API APickUpItem : public AActor,public IItemInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpItem();

	void InitializeItem(const TSubclassOf<UItemBase> BaseClass,const int32 InQuantity);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere,Category = "ItemData")
	UStaticMeshComponent* InstanceMesh;
	
	UPROPERTY(EditDefaultsOnly,Category = "ItemData")
	UDataTable* InstanceItemDataTable;
	
	UPROPERTY(EditInstanceOnly,Category = "ItemData")
	FName InstanceItemID = TEXT("001");
	
	UPROPERTY(VisibleAnywhere,Category = "ItemData")
	UItemBase* InstanceItemData;
	
	UPROPERTY(EditInstanceOnly,Category = "ItemData")
	int32 InstanceItemQuantity;
	
	UPROPERTY(EditInstanceOnly,Category = "ItemData")
	FInteractionData InstanceItemInteractData;

protected:
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	virtual void EndInteract() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
