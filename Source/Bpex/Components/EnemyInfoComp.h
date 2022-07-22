// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Bpex/Interface/HitableINT.h"
#include "EnemyInfoComp.generated.h"

UENUM(BlueprintType)
enum class EHitType :uint8
{
	ARMOUR_HIT,
	ARMOUR_BREAK,
	FLESH_HIT,
	HIT_DOWN,
	DEAD,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BPEX_API UEnemyInfoComp : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FPlaySoundsDel, EHitType)
	DECLARE_DELEGATE_OneParam(FPlayAnimDel, EHitType)
	DECLARE_DELEGATE_TwoParams(FEmitParticleDel, EHitType, const FVector&)


public:
	FPlaySoundsDel PlaySoundsDel;
	FPlayAnimDel PlayAnimDel;
	FEmitParticleDel EmitParticleDel;

	// Sets default values for this component's properties
	UEnemyInfoComp();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		int32 ArmourLevel = 4;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		float MaxArmour = 1.25f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		float CurArmour = 1.25f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		float MaxHealth = 1.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		float CurHealth = 1.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		bool bIsDead = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Properties")
		bool bIsArmourBreak = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void GetHurt(float Damage, EDamageType DamageType, const FVector& HurtLoc);

	UFUNCTION(BlueprintCallable)
		void GetSupply(float Supply, EDamageType DamageType);
};
