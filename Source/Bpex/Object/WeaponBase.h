// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Components/SphereComponent.h"
#include "Bpex/Interface/InteractableINT.h"
#include "Bpex/Interface/PickupINT.h"
#include "Sound/SoundCue.h"
#include "WeaponBase.generated.h"

class AProjectile;
class ATureFPSCharacter;

USTRUCT(BlueprintType)
struct BPEX_API FWeaponAttachData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		FVector IKGunLocation;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		FRotator IKGunRotation;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		FVector LeftShoulderLocation;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		FVector RightShoulderLocation;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		FRotator LeftHandRotation;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		FRotator RightHandRotaion;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		FVector LeftEffectorLocation;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		FVector RightEffectorLocation;
};


USTRUCT(BlueprintType)
struct BPEX_API FWeaponInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "BasicProperties")
		FString WeaponName;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "BasicProperties")
		int ClipSize;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "BasicProperties")
		int Ammo;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Fire")
		float FireRate;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Fire")
		float Damage;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Fire")
		float HeadShotDamage;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Fire")
		float SpeadRate;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Fire")
		float GunRollValue;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Fire")
		float RecoilMax;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UI")
		class UTexture2D* WeaponIcon;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Attach")
		FWeaponAttachData WeaponAttachData;
};


UCLASS()
class BPEX_API AWeaponBase : public AActor, public IInteractableINT, public IPickupINT
{
	GENERATED_BODY()

	UPROPERTY()
		FTimeline RecoilTimeline;
	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> RecoilTimelineDirection;

	bool setOriginRot_Flag = false;
	bool setPostRot_Flag = false;

	FRotator OriginRot;
	FRotator PostRot;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Trace")
		class USphereComponent* DetectSphere;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Trace")
		bool bCanPickup = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Audio")
		class USoundCue* FireSounds;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Audio")
		class USoundCue* LowAmmoSound;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnyWhere, Category = "FireEffect")
		class UParticleSystem* Muzzle;
	UPROPERTY(EditAnyWhere, Category = "FireEffect")
		float MuzzleScale3D = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Info")
		FWeaponInfo WeaponInfo;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "HUD")
		class UObjInfoWidget* DisplayInfo;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation")
		class UAnimMontage* FireAnim;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FireEffect")
		TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "RecoilCurve")
		class UCurveFloat* PitchRecoilCurve;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "RecoilCurve")
		class UCurveFloat* YawRecoilCurve;

public:
	// Sets default values for this actor's properties
	AWeaponBase();

	FORCEINLINE USkeletalMeshComponent* GetMesh() { return WeaponMesh; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	//CustomEvent
	UFUNCTION(BlueprintCallable)
		void Fire();

	UFUNCTION(BlueprintCallable)
		void CalcFireSpead(FRotator& Rot);

	//Recoil Func
	UFUNCTION()
		void BeginFireRecoil();
	UFUNCTION()
		void StopFireRecoil();

private:
	UFUNCTION()
		void ResetFireRecoil();
	UFUNCTION()
		void RecoilFinishedCallback();
	UFUNCTION()
		void PitchRotUpdate(float PitchInput);
	UFUNCTION()
		void YawRotUpdate(float YawInput);
	float GetRecoilDeltaPitch();

public:
	UFUNCTION()
		void SetHideStatus(bool Toggle);

	UFUNCTION(BlueprintCallable)
		void OnOverlapsBegin(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		void OnOverlapsEnd(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// InteractableINT接口方法
	virtual void InteractINTFunc_Implementation(APlayerCharacter* player) override;
	// PickupINT接口方法
	virtual void OnVisibleLineTraceINTFunc_Implementation() override;
	virtual void HideInfoINTFunc_Implementation() override;
};
