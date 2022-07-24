// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Bpex/Components/PlayerWeaponComp.h"
#include "Bpex/Components/PlayerInfoComp.h"
#include "Bpex/Components/PlayerBagComp.h"
#include "Bpex/Object/WeaponBase.h"
#include "Bpex/Object/Projectile.h"
#include "Bpex/Object/ItemBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FPSController.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, OriginalHeight);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = StandJumpVelocity;
	GetCharacterMovement()->AirControl = 0.75f;
	//设置初始移动数值
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	GetCharacterMovement()->MaxAcceleration = 1600.f;
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;	//可蹲伏
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanFly = true;  //可飞行、用于攀爬


	//创建摄像机组件
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->bUsePawnControlRotation = true;
	FollowCamera->FieldOfView = 106.f;

	//手臂
	ArmsAttachComp = CreateDefaultSubobject<USceneComponent>(TEXT("ArmScaleComp"));
	ArmsAttachComp->AttachToComponent(FollowCamera, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ArmsAttachComp->SetMobility(EComponentMobility::Movable);
	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	ArmMesh->AttachToComponent(ArmsAttachComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
	ArmMesh->SetMobility(EComponentMobility::Movable);
	ADSTransComp = CreateDefaultSubobject<USceneComponent>(TEXT("ADSTransComp"));
	ADSTransComp->AttachToComponent(ArmMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ADSTransComp->SetMobility(EComponentMobility::Movable);

	//武器管理组件
	EquippedWeapon = CreateDefaultSubobject<UPlayerWeaponComp>(TEXT("EquippedWeapon"));

	//背包管理组件
	Bag = CreateDefaultSubobject<UPlayerBagComp>(TEXT("PlayerBag"));

	//属性管理组件
	Info = CreateDefaultSubobject<UPlayerInfoComp>(TEXT("PlayerInfo"));

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultArmsTrans = ArmMesh->GetRelativeTransform();

	if (ADSCurve != nullptr)
	{
		FOnTimelineFloat ADSUpdateFunc;
		ADSUpdateFunc.BindUFunction(this, TEXT("SetADSAlpha"));
		ADSTimeline.AddInterpFloat(ADSCurve, ADSUpdateFunc);
	}
	

	if (CrouchCurve != nullptr)
	{
		FOnTimelineFloat CrouchUpdateFunc;
		CrouchUpdateFunc.BindUFunction(this, TEXT("SetCrouchAlpha"));
		CrouchTimeline.AddInterpFloat(CrouchCurve, CrouchUpdateFunc);
	}

	//GetCharacterMovement()->MaxWalkSpeed = WalkSpeed + 100.f;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/// Weapon Status
	bUsingWeapon = EquippedWeapon->bUsingWeapon;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::FromInt(EquippedWeapon->bUsingWeapon));

	/// ADSTimeline Tick
	ADSTimeline.TickTimeline(DeltaTime);
	/// CrouchTimeline Tick 
	CrouchTimeline.TickTimeline(DeltaTime);

	if (GetVelocity().Size() <= CrouchSpeed + 50.f)
		ToggleSlide(false);

	FHitResult VisibleTracedResult;
	FVector StartPoint = FollowCamera->GetComponentLocation();
	FVector EndPoint = StartPoint + FollowCamera->GetForwardVector() * 450.f;

	GetWorld()->LineTraceSingleByChannel(VisibleTracedResult, StartPoint, EndPoint, ECC_GameTraceChannel2);
	if (VisibleTracedResult.bBlockingHit)
	{
		TracedObj = VisibleTracedResult.GetActor();

		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, TracedObj->GetName());
		auto tracedObjClass = TracedObj->GetClass();
		if (tracedObjClass->ImplementsInterface(UPickupINT::StaticClass()))
		{
			auto item = Cast<IPickupINT>(TracedObj);
			if (item != nullptr)
				item->OnVisibleLineTraceINTFunc_Implementation();

			if (PreSpottedPickup == nullptr)
				PreSpottedPickup = TracedObj;
			if (PreSpottedPickup != TracedObj)
			{
				auto preItem = Cast<IPickupINT>(PreSpottedPickup);
				if (preItem != nullptr)
					preItem->HideInfoINTFunc_Implementation();
				PreSpottedPickup = TracedObj;
			}
		}
	}
	else
	{
		TracedObj = nullptr;
		if (PreSpottedPickup != nullptr)
		{
			auto preItem = Cast<IPickupINT>(PreSpottedPickup);
			if (preItem != nullptr)
				preItem->HideInfoINTFunc_Implementation();
			PreSpottedPickup = nullptr;
		}
	}
}

void APlayerCharacter::CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	auto Weapon = GetUsingWeapon();
	if (!bUsingWeapon || (Weapon == nullptr))
		return;

	float NewFOV = FMath::Lerp(FollowCamera->FieldOfView, 90.f, ADSAlpha);
	OutResult.FOV = NewFOV;
	
	AFPSController* pc = Cast<AFPSController>(GetController());
	FVector OffsetVec = FVector(0, FMath::Clamp(pc->TurnRightVal, -1.3f, 1.3f), FMath::Clamp(pc->LookUpVal, -1.f, 1.f));
	FVector OffsetLoc = UKismetMathLibrary::VInterpTo(WeaponSwayOffset, WeaponSwayOffset + OffsetVec, DeltaTime, 1.f);
	OffsetLoc = UKismetMathLibrary::Quat_RotateVector(FQuat(OutResult.Rotation), OffsetLoc);
	OutResult.Location += OffsetLoc;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::Jump()
{
	// 增加跳跃缓冲，缓冲完毕后恢复最大跳跃高度
	if (bJumpCharged)
	{
		GetCharacterMovement()->JumpZVelocity = StandJumpVelocity;
		bJumpCharged = false;
		GetWorldTimerManager().SetTimer(JumpChargeTimer, this, &APlayerCharacter::SetJumpCharged, JumpChargeDelay, true);
	}
	else
	{
		GetCharacterMovement()->JumpZVelocity = CrouchJumpVelocity;
		if (GetWorldTimerManager().IsTimerActive(JumpChargeTimer))
		{
			GetWorldTimerManager().ClearTimer(JumpChargeTimer);
			GetWorldTimerManager().SetTimer(JumpChargeTimer, this, &APlayerCharacter::SetJumpCharged, JumpChargeDelay, true);
		}
	}

	Super::Jump();

	if (bIsCrouching && !bIsSliding)
	{
		ToggleCrouch(false);
	}
}

void APlayerCharacter::Move(FVector Direction, float Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, Direction.ToString());
	if (!bIsSliding)
	{
		AddMovementInput(Direction, Value);
		//GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Cyan, FString::SanitizeFloat(Value));
	}
}


void APlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (PrevMovementMode == EMovementMode::MOVE_Falling)
	{
		if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
		{
			if (bAttemptToCrouch)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Purple, TEXT("Mode change trigger slide"));
				ToggleCrouch(true);
				ToggleSlide(true);
			}
		}
	}
}

bool APlayerCharacter::CanJumpInternal_Implementation() const
{
	// 使得蹲伏时可以跳跃
	// 移除：bool bCanJump = !bIsCrouched; 
	bool bCanJump = true;//GetCharacterMovement()->CanAttemptJump();

	///////////////////////////////////////////////////////
	//  Origin Source Code...
	// Ensure that the CharacterMovement state is valid
	if (bCanJump)
	{
		// Ensure JumpHoldTime and JumpCount are valid.
		if (!bWasJumping || GetJumpMaxHoldTime() <= 0.0f)
		{
			if (JumpCurrentCount == 0 && GetCharacterMovement()->IsFalling())
			{
				bCanJump = JumpCurrentCount + 1 < JumpMaxCount;
			}
			else
			{
				bCanJump = JumpCurrentCount < JumpMaxCount;
			}
		}
		else
		{
			const bool bJumpKeyHeld = (bPressedJump && JumpKeyHoldTime < GetJumpMaxHoldTime());
			bCanJump = bJumpKeyHeld &&
				((JumpCurrentCount < JumpMaxCount) || (bWasJumping && JumpCurrentCount == JumpMaxCount));
		}
	}
	return bCanJump;
}

void APlayerCharacter::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, TEXT("Crouch"));

	CrouchTimeline.Play();
}

void APlayerCharacter::UnCrouch(bool bClientSimulation)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, TEXT("UnCrouch !!!"));
	Super::UnCrouch(bClientSimulation);
}
void APlayerCharacter::UnCrouch_MyVersion()
{
	CrouchTimeline.Reverse();
}


///////////////////////////////////////////////////////////
// CustomEvent
void APlayerCharacter::ToggleCrouch(bool bToggle)
{
	if (bToggle)
	{
		if (GetCharacterMovement()->IsFalling())
		{
			bAttemptToCrouch = true;
			return;
		}
		bIsCrouching = true;
		if (bIsSprinting && !(GetCharacterMovement()->IsFalling()) && !bAttemptToCrouch)
			ToggleSlide(true);

		Crouch();
	}
	else
	{
		ToggleSlide(false);
		bIsCrouching = false;
		bAttemptToCrouch = false;
		UnCrouch_MyVersion();
	}
}
void APlayerCharacter::SetCrouchAlpha(float value)
{
	CrouchAlpha = value;
	float NewHeight = FMath::Lerp(OriginalHeight, CrouchHeight, value);
	GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeight);
	GetCharacterMovement()->CrouchedHalfHeight = NewHeight;
	if (NewHeight == OriginalHeight)
		UnCrouch();
}

void APlayerCharacter::ToggleSprint(bool bToggle)
{
	if (bToggle)
	{
		if (!(bIsAimming || bIsReloading || GetCharacterMovement()->IsFalling()))
		{
			if (bIsCrouching)
				ToggleCrouch(false);
			bIsSprinting = true;
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
	}
	else
	{
		bIsSprinting = false;
		if (!(bIsAimming || bIsSupplying))
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacter::ToggleSlide(bool bToggle)
{
	if (bToggle)
	{
		bIsSliding = true;
		//GetCharacterMovement()->MaxWalkSpeedCrouched = SlideSpeed;
		GetCharacterMovement()->bUseSeparateBrakingFriction = true;
		FVector velocity = GetVelocity();
		float impluseScale = 1.5f;
		if (velocity.Size2D() <= WalkSpeed && bAttemptToCrouch)
			impluseScale = 2.f;
		GetCharacterMovement()->AddImpulse(FVector(velocity.X * impluseScale, velocity.Y * impluseScale, 0.f), true);
	}
	else
	{
		bIsSliding = false;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
		GetCharacterMovement()->bUseSeparateBrakingFriction = false;
	}
}

void APlayerCharacter::ToggleSupply(bool bToggle)
{	
	if (bToggle)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		bIsSupplying = true;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		bIsSupplying = false;
	}
}

void APlayerCharacter::PrimaryFire()
{
	if (bIsReloading)
		return;

	auto Weapon = GetUsingWeapon();
	if (!bUsingWeapon || !bTakeOutWeapon || Weapon == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("Not using weapon!"));
		return;
	}
	else if (Weapon->WeaponInfo.Ammo <= 0)
	{
		StopFire();
		StartReload();
		return;
	}

	ToggleSprint(false);
	bIsFiring = true;

	float fireRate = Weapon->WeaponInfo.FireRate;
	Weapon->Fire();
	Weapon->BeginFireRecoil();
	FireAmmoTrace();

	if (!bIsAimming)
	{
		UAnimMontage* fireAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/ArmMesh/Animations/anim_Arm_FIre_Montage.anim_Arm_FIre_Montage'"));
		ArmMesh->GetAnimInstance()->Montage_Play(fireAnim);
	}
	else
	{
		UAnimMontage* fireAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/ArmMesh/Animations/anim_Arm_ADS_Fire_Montage.anim_Arm_ADS_Fire_Montage'"));
		ArmMesh->GetAnimInstance()->Montage_Play(fireAnim);
	}

	HUDAmmoDel.ExecuteIfBound(Weapon->WeaponInfo.Ammo, -1);
	GetWorldTimerManager().SetTimer(FireDelayTimer, this, &APlayerCharacter::FireLoop, fireRate, true);
}
void APlayerCharacter::FireLoop()
{
	if (bIsFiring)
	{
		auto Weapon = GetUsingWeapon();
		if (Weapon->WeaponInfo.Ammo <= 0)
		{
			StopFire();
			StartReload();
			return;
		}
		Weapon->Fire();
		Weapon->BeginFireRecoil();
		FireAmmoTrace();

		if (!bIsAimming)
		{
			UAnimMontage* fireAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/ArmMesh/Animations/anim_Arm_FIre_Montage.anim_Arm_FIre_Montage'"));
			ArmMesh->GetAnimInstance()->Montage_Play(fireAnim);
		}
		else
		{
			UAnimMontage* fireAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/ArmMesh/Animations/anim_Arm_ADS_Fire_Montage.anim_Arm_ADS_Fire_Montage'"));
			ArmMesh->GetAnimInstance()->Montage_Play(fireAnim);
		}


		HUDAmmoDel.ExecuteIfBound(Weapon->WeaponInfo.Ammo, -1);
	}
}
void APlayerCharacter::StopFire()
{
	bIsFiring = false;

	auto Weapon = GetUsingWeapon();
	if (Weapon != nullptr)
	{
		Weapon->StopFireRecoil();
	}
}
void APlayerCharacter::FireAmmoTrace()
{
	auto Weapon = GetUsingWeapon();
	auto MuzzleLoc = Weapon->GetMesh()->GetSocketLocation(TEXT("Muzzle"));

	auto camera = GetFollowCamera();
	FHitResult ShotResult;
	FRotator fwdRot = camera->GetComponentRotation();
	if (!bIsAimming)
		Weapon->CalcFireSpead(fwdRot);

	FVector StartPoint = camera->GetComponentLocation();
	FVector EndPoint = StartPoint + fwdRot.Vector() * 3000.f;
	GetWorld()->LineTraceSingleByChannel(ShotResult, StartPoint, EndPoint, ECC_GameTraceChannel3);
	//DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Green, false, 20.f);

	if (Weapon->ProjectileClass)
		GetWorld()->SpawnActor<AProjectile>(Weapon->ProjectileClass, MuzzleLoc, fwdRot);

	if (ShotResult.bBlockingHit)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, ShotResult.GetActor()->GetName());//.ToString());

		auto* hitObj = Cast<IHitableINT>(ShotResult.GetActor());
		if (hitObj)
		{
			float damage = (ShotResult.BoneName.ToString() == TEXT("head")) ? Weapon->WeaponInfo.HeadShotDamage : Weapon->WeaponInfo.Damage;
			FVector2D screenLoc;
			UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0), ShotResult.Location, screenLoc);
			//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, screenLoc.ToString());
			bool isDead = hitObj->HitINTFunc_Implementation(damage, ShotResult.Location, ShotResult.BoneName.ToString());
			Info->ArmourEvolution(damage * 100.f);
			HUDHitMarkDel.ExecuteIfBound(isDead);
		}
	}
}

void APlayerCharacter::StartReload()
{
	ToggleADS(false);
	ToggleSprint(false);
	StopFire();

	bIsReloading = true;

	UAnimMontage* TPPAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/Mannequin/UE4_Mannequin/Animation/Reload_Rifle_Ironsights_Montage.Reload_Rifle_Ironsights_Montage'"));
	PlayAnimMontage(TPPAnim);
	UAnimMontage* ReloadAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/ArmMesh/Animations/anim_Arm_Reload_Montage.anim_Arm_Reload_Montage'"));
	ArmMesh->GetAnimInstance()->Montage_Play(ReloadAnim);

	GetWorldTimerManager().SetTimer(ReloadDelayTimer, this, &APlayerCharacter::EndReload, ReloadAnim->GetPlayLength());
}

void APlayerCharacter::EndReload()
{
	auto Weapon = GetUsingWeapon();
	
	if (Weapon == nullptr)
		return;
	if (Weapon->WeaponInfo.Ammo == Weapon->WeaponInfo.ClipSize)
		return;

	AItemBase* ammoItem = Bag->SearchInBag(EItemType::AMMO);
	if (ammoItem == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Out of Ammo!"));
		UGameplayStatics::PlaySound2D(GetWorld(), Weapon->LowAmmoSound);
		return;
	}

	int32 loadAmmo = Weapon->WeaponInfo.ClipSize - Weapon->WeaponInfo.Ammo;
	int32 expectedLoad = loadAmmo;
	if (!Bag->UseItems(ammoItem, loadAmmo))
		Weapon->WeaponInfo.Ammo += expectedLoad - loadAmmo;
	else
		Weapon->WeaponInfo.Ammo = Weapon->WeaponInfo.ClipSize;

	HUDAmmoDel.ExecuteIfBound(Weapon->WeaponInfo.Ammo, Bag->CountItemInBag(EItemType::AMMO));
	bIsReloading = false;
}

/////////////////////////////////////////////////
// ADS-Mode
void APlayerCharacter::SetADSAlpha(float value)
{
	ADSAlpha = value;

	FTransform OriginTrans = bIsAimming ? ArmMesh->GetRelativeTransform() : DefaultArmsTrans;
	FTransform TargetTrans = ADSTransComp->GetComponentTransform().GetRelativeTransform(ArmMesh->GetComponentTransform()).Inverse();
	ArmMesh->SetRelativeTransform(UKismetMathLibrary::TLerp(OriginTrans, TargetTrans, value));
}
void APlayerCharacter::ToggleADS(bool bToggle)
{
	if (bToggle)
	{
		if ((!bIsReloading) && bTakeOutWeapon)
		{
			bIsAimming = true;
			GetCharacterMovement()->MaxWalkSpeed = AimSpeed;
			ArmMesh->GetAnimInstance()->StopAllMontages(0.f);
			ADSTimeline.Play();
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		ADSTimeline.Reverse();
		bIsAimming = false;
	}
}
void APlayerCharacter::SetADSTransComp()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Set trans"));
	ADSTransComp->SetWorldTransform(GetUsingWeapon()->GetMesh()->GetSocketTransform("IronSight"));
	bTakeOutWeapon = true;
}

/////////////////////////////////////////////
// Interact-Mode
void APlayerCharacter::InteractTracedObj()
{
	if (TracedObj == nullptr)
		return;

	UClass* objClass = TracedObj->GetClass();
	if (objClass->ImplementsInterface(UInteractableINT::StaticClass()))
	{
		auto ImpObj = Cast<IInteractableINT>(TracedObj);
		ImpObj->InteractINTFunc_Implementation(this);
	}
}

void APlayerCharacter::UsingSupplement(AItemBase* Item, int32 Num)
{
	auto supplementType = Item->ItemInfo.ItemType;
	if (supplementType == EItemType::CELL || supplementType == EItemType::BATTERY)
	{
		if (Info->CurArmour == Info->MaxArmour)
			return;
	}
	else if (supplementType == EItemType::AIDS || supplementType == EItemType::MEDKIT)
	{
		if (Info->CurHealth == Info->MaxHealth)
			return;
	}

	SwitchWeapon(0.f);
	ToggleSupply(true);

	float usingTime = Item->ItemInfo.NeedTime;
	FTimerDelegate CompleteSupplyDel;
	CompleteSupplyDel.BindUFunction(this, "SupplyFinished", Item, Num);
	GetWorldTimerManager().SetTimer(UseSupplementTimer, CompleteSupplyDel, usingTime, false);
	
	HUDSupplyProgressDel.ExecuteIfBound(usingTime, Item->ItemInfo.Icon, Item->ItemInfo.ItemName);
}

void APlayerCharacter::SupplyFinished(AItemBase* Item, int32 Num)
{
	ToggleSupply(false);
	Bag->UseItems(Bag->SearchInBag(Item->ItemInfo.ItemType), Num);
}

void APlayerCharacter::StopUsingSupplement()
{
	ToggleSupply(false);
	GetWorldTimerManager().ClearTimer(UseSupplementTimer);
}

//////////////////////////////////////////
// Weapon
void APlayerCharacter::AttainWeapon(AWeaponBase* weapon)
{
	if (bUsingWeapon == false)
	{
		UAnimMontage* TakeoutAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/ArmMesh/Animations/anim_Arm_Takeout_Montage.anim_Arm_Takeout_Montage'"));
		ArmMesh->GetAnimInstance()->Montage_Play(TakeoutAnim);
		EquippedWeapon->AttainWeapon(weapon);

		GetWorldTimerManager().SetTimer(SwitchDelayTimer, this, &APlayerCharacter::SetupWeaponWhenTakeout, TakeoutAnim->GetPlayLength() * 0.4f);
		GetWorldTimerManager().SetTimer(SetADSTransTimer, this, &APlayerCharacter::SetADSTransComp, TakeoutAnim->GetPlayLength() * 0.98f);
	}
	else
	{
		//bUsingWeapon = false;
		bTakeOutWeapon = false;
		UAnimMontage* SwitchAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/ArmMesh/Animations/anim_Arm_Switch_Montage.anim_Arm_Switch_Montage'"));
		ArmMesh->GetAnimInstance()->Montage_Play(SwitchAnim);
		EquippedWeapon->AttainWeapon(weapon);

		GetWorldTimerManager().SetTimer(SwitchDelayTimer, this, &APlayerCharacter::SetupWeaponWhenTakeout, SwitchAnim->GetPlayLength() * 0.4f);
		GetWorldTimerManager().SetTimer(SetADSTransTimer, this, &APlayerCharacter::SetADSTransComp, SwitchAnim->GetPlayLength() * 0.98f);
	}

	HUDWeaponInfoDel.Broadcast(weapon->WeaponInfo, EquippedWeapon->UseWeaponIdx);
}

void APlayerCharacter::SwitchWeapon(int32 weaponIdx)
{
	if (weaponIdx != 0)
	{
		if (EquippedWeapon->GetWeaponByIdx(weaponIdx) == nullptr)
			return;
		else
		{
			if (bUsingWeapon && (EquippedWeapon->UseWeaponIdx != weaponIdx))
			{
				//bUsingWeapon = false;
				bTakeOutWeapon = false;
				UAnimMontage* SwitchAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/ArmMesh/Animations/anim_Arm_Switch_Montage.anim_Arm_Switch_Montage'"));
				ArmMesh->GetAnimInstance()->Montage_Play(SwitchAnim);
				EquippedWeapon->SwitchWeapon(weaponIdx);
				GetWorldTimerManager().SetTimer(SwitchDelayTimer, this, &APlayerCharacter::SetupWeaponWhenTakeout, SwitchAnim->GetPlayLength() * 0.4f);
				GetWorldTimerManager().SetTimer(SetADSTransTimer, this, &APlayerCharacter::SetADSTransComp, SwitchAnim->GetPlayLength() * 0.98f);

				HUDWeaponInfoDel.Broadcast(GetUsingWeapon()->WeaponInfo, EquippedWeapon->UseWeaponIdx);
				return;
			}

			if (!bUsingWeapon)
			{
				bTakeOutWeapon = false;
				UAnimMontage* TakeoutAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/ArmMesh/Animations/anim_Arm_Takeout_Montage.anim_Arm_Takeout_Montage'"));
				ArmMesh->GetAnimInstance()->Montage_Play(TakeoutAnim);
				EquippedWeapon->SwitchWeapon(weaponIdx);
				GetWorldTimerManager().SetTimer(SwitchDelayTimer, this, &APlayerCharacter::SetupWeaponWhenTakeout, TakeoutAnim->GetPlayLength() * 0.4f);
				GetWorldTimerManager().SetTimer(SetADSTransTimer, this, &APlayerCharacter::SetADSTransComp, TakeoutAnim->GetPlayLength() * 0.98f);

				HUDWeaponInfoDel.Broadcast(GetUsingWeapon()->WeaponInfo, EquippedWeapon->UseWeaponIdx);
				return;
			}
		}
	}
	else
	{
		if (bUsingWeapon)
		{
			UAnimMontage* PackupAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/ArmMesh/Animations/anim_Arm_Packup_Montage.anim_Arm_Packup_Montage'"));
			ArmMesh->GetAnimInstance()->Montage_Play(PackupAnim);
			GetWorldTimerManager().SetTimer(SwitchDelayTimer, this, &APlayerCharacter::SetupWeaponWhenPackup, PackupAnim->GetPlayLength() * 0.4f);

			HUDWeaponInfoDel.Broadcast(GetUsingWeapon()->WeaponInfo, EquippedWeapon->UseWeaponIdx);
			return;
		}
	}

}

void APlayerCharacter::SetupWeaponWhenTakeout()
{
	AWeaponBase* usingWeapon = GetUsingWeapon();
	if (usingWeapon == nullptr)
		return;

	usingWeapon->AttachToComponent(GetArmMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponAttach"));
	usingWeapon->SetActorHiddenInGame(false);
	ADSTransComp->SetWorldTransform(usingWeapon->GetMesh()->GetSocketTransform(TEXT("IronSight")));

	AWeaponBase* unUse = EquippedWeapon->WeaponList[2 - EquippedWeapon->UseWeaponIdx];
	if (unUse != nullptr)
		unUse->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Back"));

	EquippedWeapon->TakeoutWeapon();
	bUsingWeapon = true;
}
void APlayerCharacter::SetupWeaponWhenPackup()
{
	AWeaponBase* usingWeapon = GetUsingWeapon();
	if (usingWeapon == nullptr)
		return;

	EquippedWeapon->PackupWeapon();
	bUsingWeapon = false;
	bTakeOutWeapon = false;
	usingWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Back"));
}
AWeaponBase* APlayerCharacter::GetUsingWeapon()
{
	return EquippedWeapon->GetUsingWeapon();
}