// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Bpex/Object/WeaponBase.h"
#include "Bpex/Components/PlayerBagComp.h"
#include "Bpex/Components/PlayerInfoComp.h"
#include "Bpex/Components/PlayerWeaponComp.h"
#include "Bpex/Interface/HitableINT.h"
#include "Components/TimelineComponent.h"
#include "Components/SceneComponent.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class BPEX_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/////////////////////////////////
	// 声明代理
	// HUD中Weaponz数据变化所用代理
	DECLARE_MULTICAST_DELEGATE_TwoParams(FHUDWeaponInfoDel, FWeaponInfo&, int32)
	// HUD中Ammo数据变化所用代理
	DECLARE_DELEGATE_TwoParams(FHUDAmmoNumDel, int32, int32)
	// HUD击中反馈
	DECLARE_DELEGATE_OneParam(FHUDHitMarkDel, bool)
	// 补给时UI信息
	DECLARE_DELEGATE_ThreeParams(FHUDSupplyProgressDel, float, UTexture2D*, FString)

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// 实例化代理
	FHUDAmmoNumDel HUDAmmoDel;
	FHUDWeaponInfoDel HUDWeaponInfoDel;
	FHUDHitMarkDel HUDHitMarkDel;
	FHUDSupplyProgressDel HUDSupplyProgressDel;

	// JumpCharge Timer 控制连续跳跃间隔
	FTimerHandle JumpChargeTimer;
	// SlideCharge Timer 控制连续滑铲间隔
	FTimerHandle SlideChargeTimer;
	// FireDelay Timer 控制开火速度
	FTimerHandle FireDelayTimer;
	// ReloadDelay Timer 换弹时间
	FTimerHandle ReloadDelayTimer;
	// SwitchAnimDelay Timer 切换武器时间
	FTimerHandle SwitchDelayTimer;
	FTimerHandle SetADSTransTimer;
	// CustomCrouch Timer 蹲伏所需时间
	FTimerHandle CrouchTimer;
	// 使用物品时间
	FTimerHandle UseSupplementTimer;

public:	//Properties
	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere, Category = "Status")
		bool bIsCrouching = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere, Category = "Status")
		bool bAttemptToCrouch = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere, Category = "Status")
		bool bIsSprinting = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere, Category = "Status")
		bool bIsAimming = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere, Category = "Status")
		bool bIsReloading = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere, Category = "Status")
		bool bIsFiring = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere, Category = "Status")
		bool bIsSliding = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere, Category = "Status")
		bool bIsClimbing = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere, Category = "Status")
		bool bIsSupplying = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool bUsingWeapon = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool bTakeOutWeapon = false;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		float WalkSpeed = 380.f;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		float CrouchSpeed = 230.f;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		float AimSpeed = 160.f;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		float SprintSpeed = 760.f;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		float StandJumpVelocity = 800.f;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		float CrouchJumpVelocity = 520.f;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		float JumpChargeDelay = 1.2f;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		bool bJumpCharged = true;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		float SlideChargeDelay = 2.f;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		bool bSlideCharged = true;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		float CrouchHeight = 44.f;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		float OriginalHeight = 96.f;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		FVector WeaponSwayOffset;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		float TurnVal;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Moving Data")
		float LookVal;

public: //Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* ArmsAttachComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* ArmMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* ADSTransComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		class UPlayerWeaponComp* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bag", meta = (AllowPrivateAccess = "true"))
		class UPlayerBagComp* Bag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Outfit", meta = (AllowPrivateAccess = "true"))
		class UPlayerInfoComp* Info;

	UPROPERTY()
		AActor* PreSpottedPickup;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Trace", meta = (AllowPrivateAccess = "true"))
		AActor* TracedObj;

	UPROPERTY()
	FTimeline ADSTimeline;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ADSMode")
		UCurveFloat* ADSCurve;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "ADSMode")
		float ADSAlpha = 0.f;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "ADSMode")
		FTransform DefaultArmsTrans;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "ADSMode")
		FVector DefaultArmsLoc;

	UPROPERTY()
	FTimeline CrouchTimeline;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "CrouchMode")
		UCurveFloat* CrouchCurve;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "CrouchMode")
		float CrouchAlpha = 0.f;

	UFUNCTION()
		void SetADSAlpha(float value);
	UFUNCTION()
		void SetCrouchAlpha(float value);

public: //Inline
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class UPlayerBagComp* GetBagComp() const { return Bag; }
	FORCEINLINE class UPlayerWeaponComp* GetWeaponComp() const { return EquippedWeapon; }
	FORCEINLINE class UPlayerInfoComp* GetInfoComp() const { return Info; }
	FORCEINLINE class USkeletalMeshComponent* GetArmMesh() const { return ArmMesh; }
	
	UFUNCTION(BlueprintCallable)
		void SetJumpCharged() { bJumpCharged = true; }
	UFUNCTION(BlueprintCallable)
		void SetSlideCharged() { bSlideCharged = true; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;

	/** Called for Movement input */
	UFUNCTION(BlueprintCallable)
	void Move(FVector Direction, float Value);

	// 扩展：运动状态改变
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	// 重写：修改ACharacter中的跳跃条件：以实现蹲伏时可跳跃
	virtual bool CanJumpInternal_Implementation() const override;

	// 扩展：跳跃
	virtual void Jump() override;
	// 扩展：蹲伏
	virtual void Crouch(bool bClientSimulation = false) override;
	void UnCrouch_MyVersion();


	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void ToggleCrouch(bool bToggle);
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void ToggleSprint(bool bToggle);
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void ToggleSlide(bool bToggle);
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void ToggleADS(bool bToggle);
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void ToggleSupply(bool bToggle);
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void StartReload();
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void EndReload();

	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void PrimaryFire();
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void FireLoop();
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void StopFire();
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void FireAmmoTrace();

	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void AttainWeapon(AWeaponBase* weapon);
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void SwitchWeapon(int32 weaponIdx);
	UFUNCTION()
		void SetupWeaponWhenTakeout();
	UFUNCTION()
		void SetupWeaponWhenPackup();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		AWeaponBase* GetUsingWeapon();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void InteractTracedObj();
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void UsingSupplement(AItemBase* Item, int32 Num);
	UFUNCTION()
		void SupplyFinished(AItemBase* Item, int32 Num);
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
		void StopUsingSupplement();

	UFUNCTION(BlueprintCallable)
		void SetADSTransComp();
};
