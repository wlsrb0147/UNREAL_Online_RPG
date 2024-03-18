// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItemBase;
DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

UENUM(BlueprintType)
enum class EItemAddOperationResult
{
	IAR_NoItemAdded UMETA (DisplayName = "아이템 확득 실패"),
	IAR_PartialAmountItemAdded UMETA (DisplayName = "아이템 일부 획득"),
	IAR_AllItemAdded UMETA (DisplayName = "모든 아이템 획득")
};

USTRUCT(BlueprintType)
struct FItemAddResultData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 AddAmount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddOperationResult AddResult = EItemAddOperationResult::IAR_NoItemAdded;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText AddResultMessage;

	static FItemAddResultData AddNone(const FText& ErrorText)
	{
		FItemAddResultData AddNoneResult;
		AddNoneResult.AddAmount = 0;
		AddNoneResult.AddResult = EItemAddOperationResult::IAR_NoItemAdded;
		AddNoneResult.AddResultMessage = ErrorText;
		return AddNoneResult;
	}

	static FItemAddResultData AddPartial(const int32 PartialAdded, const FText& Message)
	{
		FItemAddResultData AddPartialResult;
		AddPartialResult.AddAmount = PartialAdded;
		AddPartialResult.AddResult = EItemAddOperationResult::IAR_PartialAmountItemAdded;
		AddPartialResult.AddResultMessage = Message;
		return AddPartialResult;
	}

	static FItemAddResultData AddAll(const int32 AllAdded, const FText& Message)
	{
		FItemAddResultData AddAllResult;
		AddAllResult.AddAmount = AllAdded;
		AddAllResult.AddResult = EItemAddOperationResult::IAR_AllItemAdded;
		AddAllResult.AddResultMessage = Message;
		return AddAllResult;
	}
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONLINE_RPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	FOnInventoryUpdated OnInventoryUpdated;

	// Sets default values for this component's properties
	UInventoryComponent();
	FORCEINLINE TArray<UItemBase*> GetInventory() const {return InventoryContents;}
	FORCEINLINE int32 GetInventoryCapacity() const {return InventorySlotCapacity;}
	FORCEINLINE void SetInventoryCapacity(const int32 Capacity) {InventorySlotCapacity = Capacity;}
	void AddMoney(const uint64 AmountToAddMoney){CurrentMoney += AmountToAddMoney;}
	uint64 GetMoney() const {return CurrentMoney;}
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32 InventorySlotCapacity;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	uint64 CurrentMoney = 0;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FItemAddResultData HandleAddItem(UItemBase* InputItem);
	int32 AddStackableItem(UItemBase* InputItem, int32 AddAmount);
	FItemAddResultData AddNonStackableItem(UItemBase* InputItem);
	void AddNewItem(UItemBase* Item,const int32 AmountToAdd);
	void RemoveItemFromList(UItemBase* ItemToRemove);
	int32 RemoveAmountOfItem(UItemBase* RemoveItem,int32 AmountToRemove) const;
	UItemBase* FindNextPartial(UItemBase* ItemIn) const;
	int32 CalculateNumberForFullStack(const UItemBase* StackableItem,int32 AddAmount);

};
