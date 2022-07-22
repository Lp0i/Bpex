// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Bpex/Object/ItemBase.h"
#include "PlayerBagComp.generated.h"

class AOutfitBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BPEX_API UPlayerBagComp : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DELEGATE_TwoParams(FHUDItemDel, EItemType, int32)
	DECLARE_MULTICAST_DELEGATE_TwoParams(FUIInventoryDel, TArray<AItemBase*>&, int32)

public:
	// Sets default values for this component's properties
	UPlayerBagComp();

	FHUDItemDel HUDItemDel;
	FUIInventoryDel UIInventoryDel;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Outfit")
		class AOutfitBase* EquippedBag;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Outfit")
		int32 BagLevel = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item")
		TArray<AItemBase*> Bag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		int32 OriginSize = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		int32 MaxSize;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE TArray<AItemBase*>& GetBag() { return Bag; }

	UFUNCTION(BlueprintCallable)
		void GetItem(AItemBase* Item);

	UFUNCTION(BlueprintCallable)
		void DropItem(AItemBase* Item);

	UFUNCTION(BlueprintCallable)
		bool UseItems(AItemBase* Item, UPARAM(ref) int32& UseNum);

	UFUNCTION(BlueprintCallable)
		void AttainBag(AOutfitBase* attainBag);

	UFUNCTION()
		AItemBase* SearchInBag(EItemType ItemType);

	UFUNCTION()
		int32 CountItemInBag(EItemType ItemType);
};
