// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInfoComp.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BPEX_API UPlayerInfoComp : public UActorComponent
{
	GENERATED_BODY()

		DECLARE_DELEGATE_FourParams(FHUDInfoDel, float, int32, float, float)


public:
	// Sets default values for this component's properties
	UPlayerInfoComp();

public:

	FHUDInfoDel HUDInfoDel;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Properties")
		bool bIsDead;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		int32 HelmetLevel = 1;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		int32 ArmourLevel = 1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		bool bHelmetSpecialBuff = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		bool bArmourSpecialBuff = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		float MaxArmour = 0.5f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		float MaxHealth = 1.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		float MaxEvoValue = 150.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		float CurArmour = 0.5f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		float CurHealth = 1.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		float CurEvoValue = 0.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		float HeadShotDefence = 0.1f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "OutFit")
		class AOutfitBase* EquippedHelmet;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "OutFit")
		class AOutfitBase* EquippedArmour;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Audio")
		class USoundBase* ArmourUpgradeSoundEffect;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void GetDamage(float damage);

	UFUNCTION(BlueprintCallable)
		void GetArmourSupply(float supply) {
		CurArmour += supply;
		if (CurArmour > MaxArmour)
			CurArmour = MaxArmour;

		HUDInfoDel.ExecuteIfBound(MaxEvoValue - CurEvoValue, ArmourLevel, (CurArmour / MaxArmour), (CurHealth / MaxHealth));
	}

	UFUNCTION(BlueprintCallable)
		void GetHealthSupply(float supply) {
		CurHealth += supply;
		if (CurHealth > MaxHealth)
			CurHealth = MaxHealth;

		HUDInfoDel.ExecuteIfBound(MaxEvoValue - CurEvoValue, ArmourLevel, (CurArmour / MaxArmour), (CurHealth / MaxHealth));
	}

	UFUNCTION(BlueprintCallable)
		void AttainArmour(AOutfitBase* attainArmour);
	UFUNCTION(BlueprintCallable)
		void AttainHelmet(AOutfitBase* attainHelmet);
	UFUNCTION(BlueprintCallable)
		void ArmourEvolution(float damage);
};
