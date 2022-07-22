// Fill out your copyright notice in the Description page of Project Settings.


#include "OutFitBase.h"
#include "Bpex/Character/PlayerCharacter.h"
#include "Bpex/Components/PlayerInfoComp.h"
#include "Bpex/Components/PlayerBagComp.h"
#include "Bpex/UMG/ObjInfoWidget.h"

///////////////////////////////////////////////////
// Initialize
// Sets default values
AOutfitBase::AOutfitBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	DetectSphere->SetSphereRadius(168.f);
	DetectSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DetectSphere->SetMobility(EComponentMobility::Movable);
}
void AOutfitBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AOutfitBase::OnOverlapsBegin);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AOutfitBase::OnOverlapsEnd);

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
			const UEnum* outfittype = FindObject<UEnum>(ANY_PACKAGE, TEXT("EOutfitInfo"));
			DisplayInfo->bindType(outfittype->GetEnumText((int32)OutfitInfo.OutfitType));
			DisplayInfo->bindImg(OutfitInfo.OutfitImg);
		}
	}
}
void AOutfitBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	switch (OutfitInfo.OutfitType)
	{
	case EOutfitType::HELMET:
		OutfitInfo.EvoValue = -1;
		OutfitInfo.MaxEvoValue = -1;
		break;

	case EOutfitType::ARMOUR:
		OutfitInfo.AdditiveValue = 0.25 + OutfitInfo.Level * 0.25;
		OutfitInfo.CurrentValue = OutfitInfo.AdditiveValue;
		OutfitInfo.EvoValue = 0.f;
		if (OutfitInfo.Level == 1)
			OutfitInfo.MaxEvoValue = 150.f;
		else
		{
			if (OutfitInfo.Level == 2)
				OutfitInfo.MaxEvoValue = 300.f;
			else
			{
				if (OutfitInfo.Level == 3)
					OutfitInfo.MaxEvoValue = 750.f;
				else
					OutfitInfo.MaxEvoValue = INT32_MAX;
			}
		}
		break;

	case EOutfitType::BAG:
		OutfitInfo.AdditiveValue = 2 * OutfitInfo.Level;
		OutfitInfo.EvoValue = -1;
		OutfitInfo.MaxEvoValue = -1;
		break;
	}
}


// Called when the game starts or when spawned
void AOutfitBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AOutfitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AOutfitBase::SetHideStatus(bool bToggle)
{
	SetActorHiddenInGame(bToggle);
	SetActorEnableCollision(!bToggle);
}

/////////////////////////////////////////////////
// 交互 //Interact_Interface接口函数
void AOutfitBase::InteractINTFunc_Implementation(APlayerCharacter* player)
{
	if (bCanPickup)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("pickup Outfit"));
		auto playerBag = player->GetBagComp();
		auto playerInfo = player->GetInfoComp();

		switch (OutfitInfo.OutfitType)
		{
		case EOutfitType::HELMET:
			playerInfo->AttainHelmet(this);
			break;

		case EOutfitType::ARMOUR:
			playerInfo->AttainArmour(this);
			break;

		case EOutfitType::BAG:
			playerBag->AttainBag(this);
			break;
		}
	}
}


void AOutfitBase::OnVisibleLineTraceINTFunc_Implementation()
{
	if (DisplayInfo && !DisplayInfo->IsInViewport())
	{
		DisplayInfo->AddToViewport();
		DisplayInfo->playInfoAnim();
	}
}
void AOutfitBase::HideInfoINTFunc_Implementation()
{
	if (DisplayInfo->IsInViewport())
	{
		DisplayInfo->RemoveFromViewport();
	}
}