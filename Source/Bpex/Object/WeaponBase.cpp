// Fill out your copyright notice in the Description page of Project Settings.
#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "Bpex/Object/Projectile.h"
#include "Bpex/Character/PlayerCharacter.h"
#include "Bpex/Interface/HitableINT.h"
#include "Bpex/UMG/ObjInfoWidget.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/AudioComponent.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = WeaponMesh;

	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	DetectSphere->SetSphereRadius(168.f);
	DetectSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DetectSphere->SetMobility(EComponentMobility::Movable);

}

void AWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnOverlapsBegin);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnOverlapsEnd);
	//AudioPlayer->SetSound(FireSounds);

	//初始化DisplayWidget 不能写在Constructor中
	auto widgetClass = LoadClass<UObjInfoWidget>(NULL, TEXT("WidgetBlueprint'/Game/Blueprint/UMG/BP_ObjWidget.BP_ObjWidget_C'"));
	if (widgetClass)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("find BPclass!"));
		DisplayInfo = CreateWidget<UObjInfoWidget>(GetWorld(), widgetClass);
		if (DisplayInfo != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Create Successful"));
			DisplayInfo->bindNum(1);
			DisplayInfo->bindType(FText::FromString(WeaponInfo.WeaponName));
			DisplayInfo->bindImg(WeaponInfo.WeaponIcon);
		}
	}
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();


	// RecoilTimeline 初始化
	FOnTimelineFloatStatic PitchUpdateEvent;
	FOnTimelineFloatStatic YawUpdateEvent;
	FOnTimelineEventStatic FinishedEvent;

	PitchUpdateEvent.BindUFunction(this, TEXT("PitchRotUpdate"));
	YawUpdateEvent.BindUFunction(this, TEXT("YawRotUpdate"));
	FinishedEvent.BindUFunction(this, TEXT("RecoilFinishedCallback"));

	RecoilTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	RecoilTimeline.SetTimelineLength(3.5f);
	RecoilTimeline.AddInterpFloat(PitchRecoilCurve, PitchUpdateEvent);
	RecoilTimeline.AddInterpFloat(YawRecoilCurve, YawUpdateEvent);
	RecoilTimeline.SetTimelineFinishedFunc(FinishedEvent);
	RecoilTimeline.SetPropertySetObject(this);
	RecoilTimeline.SetDirectionPropertyName(TEXT("RecoilTimelineDirection"));
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RecoilTimeline.TickTimeline(DeltaTime);

}

///////////////////////////////////////////////
// Fire 模块
void AWeaponBase::Fire()
{
	--WeaponInfo.Ammo;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::FromInt(WeaponInfo.Ammo));


	if (WeaponMesh != nullptr)
	{
		auto MuzzleLoc = WeaponMesh->GetSocketLocation(TEXT("Muzzle"));
		auto MuzzleRot = WeaponMesh->GetSocketRotation(TEXT("Muzzle"));

		WeaponMesh->PlayAnimation(FireAnim, false);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSounds, GetActorLocation(), GetActorRotation());
		auto muzzleparticle = UGameplayStatics::SpawnEmitterAttached(Muzzle, WeaponMesh, TEXT("Muzzle"), FVector(0.f), FRotator(0.f), EAttachLocation::SnapToTarget);
		muzzleparticle->SetWorldScale3D(FVector(MuzzleScale3D));
	}
}
void AWeaponBase::CalcFireSpead(FRotator& Rot)
{
	float sp = WeaponInfo.SpeadRate;
	float YawRand = FMath::FRandRange(-sp, sp);
	float PitchRand = FMath::FRandRange(-sp, sp);

	Rot.Yaw += YawRand;
	Rot.Pitch += PitchRand;
}


////////////////////////////////////////////////////////
// Fire-Recoil 模块
void AWeaponBase::BeginFireRecoil()
{
	if (PitchRecoilCurve == nullptr || YawRecoilCurve == nullptr)
		return;

	if (!setOriginRot_Flag)
	{
		OriginRot = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
		setOriginRot_Flag = true;
	}

	RecoilTimeline.SetPlayRate(1.f);
	RecoilTimeline.Play();
}
void AWeaponBase::StopFireRecoil()
{
	RecoilTimeline.SetPlayRate(8.f);
	RecoilTimeline.Reverse();
}
void AWeaponBase::ResetFireRecoil()
{
	//恢复TimelineDirection
	RecoilTimeline.Play();
	RecoilTimeline.SetPlaybackPosition(0.f, true, false);
	RecoilTimeline.Stop();

	//恢复Flag
	setOriginRot_Flag = false;
	setPostRot_Flag = false;
	//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Blue, TEXT("Reset Recoil!"));
}
void AWeaponBase::RecoilFinishedCallback()
{
	if (RecoilTimelineDirection == ETimelineDirection::Backward)
	{
		ResetFireRecoil();
	}
	RecoilTimeline.SetPropertySetObject(this);
	RecoilTimeline.SetDirectionPropertyName(TEXT("RecoilTimelineDirection"));
}
void AWeaponBase::PitchRotUpdate(float PitchInput)
{
	if (RecoilTimelineDirection == ETimelineDirection::Forward)//!RecoilTimeline.IsReversing())//
	{
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->AddControllerPitchInput(PitchInput);
	}
	else
	{
		if (!setPostRot_Flag)
		{
			PostRot = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
			setPostRot_Flag = true;
			//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Yellow, FString::SanitizeFloat(PostRot.Pitch));
		}

		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->AddControllerPitchInput(PitchInput * GetRecoilDeltaPitch() * -6.f);
	}
}
void AWeaponBase::YawRotUpdate(float YawInput)
{
	if (RecoilTimelineDirection == ETimelineDirection::Forward)//!RecoilTimeline.IsReversing())//
	{
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->AddControllerYawInput(YawInput);
	}
	else
	{
		if (!setPostRot_Flag)
		{
			PostRot = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
			setPostRot_Flag = true;
			//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Yellow, FString::SanitizeFloat(PostRot.Pitch));
		}
		//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Red, FString::SanitizeFloat(GetRecoilDeltaPitch()));
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->AddControllerYawInput(YawInput * GetRecoilDeltaPitch() * -3.f);
	}
}
float AWeaponBase::GetRecoilDeltaPitch()
{

	FRotator deltaRot = UKismetMathLibrary::NormalizedDeltaRotator(OriginRot, PostRot); //(OriginRot - PostRot).GetNormalized();
	float deltaPitch = FMath::Abs(deltaRot.Pitch);
	float valInMaxRange = FMath::GetMappedRangeValueClamped(FVector2D(0.f, WeaponInfo.RecoilMax), FVector2D(0.f, 1.f), deltaPitch);
	float valOutMaxRange = FMath::GetMappedRangeValueClamped(FVector2D(WeaponInfo.RecoilMax, 12.f), FVector2D(1.f, 0.f), deltaPitch);

	//return deltaPitch;
	return (deltaPitch < WeaponInfo.RecoilMax) ? valInMaxRange : valOutMaxRange;
}


void AWeaponBase::InteractINTFunc_Implementation(APlayerCharacter* player)
{
	if (bCanPickup)
	{
		player->AttainWeapon(this);
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("add weapon"));
		WeaponMesh->SetSimulatePhysics(false);
		SetHideStatus(true);
	}

}

void AWeaponBase::OnVisibleLineTraceINTFunc_Implementation()
{
	if (DisplayInfo && !DisplayInfo->IsInViewport())
	{
		DisplayInfo->AddToViewport();
		DisplayInfo->playInfoAnim();
	}
}

void AWeaponBase::HideInfoINTFunc_Implementation()
{
	if (DisplayInfo->IsInViewport())
	{
		DisplayInfo->RemoveFromViewport();
	}
}

void AWeaponBase::OnOverlapsBegin(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	bCanPickup = true;
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("overlap"));
}

void AWeaponBase::OnOverlapsEnd(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bCanPickup = false;
}

void AWeaponBase::SetHideStatus(bool Toggle)
{
	SetActorEnableCollision(!Toggle);

	SetActorHiddenInGame(Toggle);

	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("set hidden"));
}