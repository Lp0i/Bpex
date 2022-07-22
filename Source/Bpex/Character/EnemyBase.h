// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Bpex/Interface/HitableINT.h"
#include "Bpex/Components/EnemyInfoComp.h"
#include "EnemyBase.generated.h"

UCLASS()
class BPEX_API AEnemyBase : public ACharacter, public IHitableINT
{
	GENERATED_BODY()

	DECLARE_DELEGATE(FRespawnDel)

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Audio")
		class UAudioComponent* AudioPlayer;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Audio")
		class USoundBase* ArmourHitSound;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Audio")
		class USoundBase* FleshHitSound;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Audio")
		class USoundBase* HitDownSound;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Audio")
		class USoundBase* ArmourBreakSound;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Audio")
		class USoundBase* HeadShotSound;


	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Particle")
		class UNiagaraSystem* FleshHitParticle;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Particle")
		class UNiagaraSystem* ArmourHitParticle;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Particle")
		class UNiagaraSystem* ArmourBreakParticle;


	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "HUD")
		class UHUDDamageNum* HUDDamageNum;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Info")
		class UEnemyInfoComp* Info;

	FRespawnDel RespawnDel;
	FTimerHandle DeadDestroyTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool HitINTFunc_Implementation(float damage, const FVector& hitLocation, const FString& hitBoneName = TEXT("None"), EDamageType DamageType = EDamageType::COMMON) override;

	UFUNCTION(BlueprintCallable)
		void PlaySoundsEffect(EHitType Type);

	UFUNCTION(BlueprintCallable)
		void PlayAnimEffect(EHitType Type);

	UFUNCTION(BlueprintCallable)
		void EmitParticleEffect(EHitType Type, const FVector& hitLoc);

	UFUNCTION(BlueprintCallable)
		void EnemyDead();
};
