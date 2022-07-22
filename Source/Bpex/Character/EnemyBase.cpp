// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyBase.h"
#include "Components/AudioComponent.h"
#include "Bpex/UMG/HUDWidget.h"
#include "Bpex/UMG/HUDDamageNum.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//设置默认骨骼网格体组件
	auto skeletonmesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Asset/Mannequin/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (skeletonmesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(skeletonmesh.Object);
	}

	AudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioPlayer"));

	Info = CreateDefaultSubobject<UEnemyInfoComp>(TEXT("Info"));
	Info->PlaySoundsDel.BindUObject(this, &AEnemyBase::PlaySoundsEffect);
	Info->PlayAnimDel.BindUObject(this, &AEnemyBase::PlayAnimEffect);
	Info->EmitParticleDel.BindUObject(this, &AEnemyBase::EmitParticleEffect);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto hudClass = LoadClass<UHUDDamageNum>(NULL, TEXT("WidgetBlueprint'/Game/Blueprint/UMG/BP_HUDDamgeNum.BP_HUDDamgeNum_C'"));
	if (hudClass)
	{
		HUDDamageNum = CreateWidget<UHUDDamageNum>(GetWorld(), hudClass);
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("find num"));
	}
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//////////////////////////////////////////
// Hit_Interface
bool AEnemyBase::HitINTFunc_Implementation(float damage, const FVector& hitLocation, const FString& hitBoneName, EDamageType DamageType)
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Yellow, TEXT("GetHit!"));

	if (HUDDamageNum != nullptr)
	{
		HUDDamageNum->AddDamage(damage * 100.f, Info->ArmourLevel, Info->bIsArmourBreak);
		if (!HUDDamageNum->IsInViewport())
		{
			FVector2D hudLoc;
			UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0), hitLocation, hudLoc);
			HUDDamageNum->AddToViewport();
			HUDDamageNum->SetPositionInViewport(hudLoc + FVector2D(50.f, -100.f));
		}
	}
	Info->GetHurt(damage, DamageType, hitLocation);
	if(hitBoneName == "head")
		UGameplayStatics::PlaySound2D(GetWorld(), HeadShotSound);

	if (Info->bIsDead)
	{
		SetActorEnableCollision(false);
		GetWorldTimerManager().SetTimer(DeadDestroyTimer, this, &AEnemyBase::EnemyDead, 1.0f);
		return true;
	}
	else
		return false;
}

void AEnemyBase::EnemyDead()
{
	RespawnDel.ExecuteIfBound();
	ConditionalBeginDestroy();
}


void AEnemyBase::PlaySoundsEffect(EHitType Type)
{
	switch (Type)
	{
	case EHitType::ARMOUR_HIT:
		UGameplayStatics::PlaySound2D(GetWorld(), ArmourHitSound);
		break;

	case EHitType::ARMOUR_BREAK:
		UGameplayStatics::PlaySound2D(GetWorld(), ArmourBreakSound);
		break;

	case EHitType::FLESH_HIT:
		UGameplayStatics::PlaySound2D(GetWorld(), FleshHitSound);
		break;

	case EHitType::HIT_DOWN:
		UGameplayStatics::PlaySound2D(GetWorld(), HitDownSound);
		break;
	}
}

void AEnemyBase::PlayAnimEffect(EHitType Type)
{
	UAnimMontage* hitAnim;

	switch (Type)
	{
	case EHitType::ARMOUR_HIT:
		//
		break;

	case EHitType::ARMOUR_BREAK:
		//
		break;

	case EHitType::FLESH_HIT:
		hitAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/Mannequin/UE4_Mannequin/Animation/Hit_React_1_Montage.Hit_React_1_Montage'"));
		PlayAnimMontage(hitAnim);
		break;

	case EHitType::HIT_DOWN:
		//
		break;

	case EHitType::DEAD:
		hitAnim = LoadObject<UAnimMontage>(NULL, TEXT("AnimMontage'/Game/Asset/Mannequin/UE4_Mannequin/Animation/Death_Montage.Death_Montage'"));
		PlayAnimMontage(hitAnim);
		break;
	}
}

void AEnemyBase::EmitParticleEffect(EHitType Type, const FVector& hitLoc)
{
	switch (Type)
	{
	case EHitType::ARMOUR_HIT:
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ArmourHitParticle, hitLoc);
		break;

	case EHitType::ARMOUR_BREAK:
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ArmourBreakParticle, GetActorLocation());
		break;

	case EHitType::FLESH_HIT:
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FleshHitParticle, hitLoc);
		break;
	}
}