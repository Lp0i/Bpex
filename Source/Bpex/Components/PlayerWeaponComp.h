// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Bpex/Object/WeaponBase.h"
#include "PlayerWeaponComp.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BPEX_API UPlayerWeaponComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerWeaponComp();

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Weapon")
		TArray<AWeaponBase*> WeaponList;
	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere, Category = "Weapon")
		int32 UseWeaponIdx = 1;  // Only 1 or 2;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
		bool bUsingWeapon = false;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		AWeaponBase* GetUsingWeapon()
	{
		return WeaponList[UseWeaponIdx - 1];
	}

	UFUNCTION()
		AWeaponBase* GetWeaponByIdx(int32 idx)
	{
		return WeaponList[idx - 1];
	}

	void AttainWeapon(AWeaponBase* Weapon);
	void SwitchWeapon(int32 WeaponIdx);

	UFUNCTION()
		void PackupWeapon() { bUsingWeapon = false; }
	UFUNCTION()
		void TakeoutWeapon() { bUsingWeapon = true; }
};
