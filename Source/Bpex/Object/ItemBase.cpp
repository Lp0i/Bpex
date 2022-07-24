// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Bpex/Character/PlayerCharacter.h"
#include "Bpex/Components/PlayerBagComp.h"

//////////////////////////////////////////////////
// Constructor
AItemBase::AItemBase()
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


/////////////////////////////////////////////////
// InitializeComponent
void AItemBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverlapsBegin);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnOverlapsEnd);

	//初始化DisplayWidget 不能写在Constructor中
	auto widgetClass = LoadClass<UObjInfoWidget>(NULL, TEXT("WidgetBlueprint'/Game/Blueprint/UMG/BP_ObjWidget.BP_ObjWidget_C'"));
	if (widgetClass)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("find BPclass!"));
		DisplayInfo = CreateWidget<UObjInfoWidget>(GetWorld(), widgetClass);
		if (DisplayInfo != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Create Successful"));
			DisplayInfo->bindNum(ItemInfo.Num);
			const UEnum* itemtype = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemType"));
			DisplayInfo->bindType(itemtype->GetEnumText((int32)ItemInfo.ItemType));
			DisplayInfo->bindImg(ItemInfo.Icon);
		}
	}
}

void AItemBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent)
{
	Super::PostEditChangeProperty(PropertyChangeEvent);

	switch (ItemInfo.ItemType)
	{
	case EItemType::AMMO:
		ItemInfo.MaxNum = 60;
		ItemInfo.NeedTime = 0.f;
		ItemInfo.Supply = 0.f;
		break;

	case EItemType::AIDS:
		ItemInfo.MaxNum = 4;
		ItemInfo.NeedTime = 5.f;
		ItemInfo.Supply = 0.25f;
		break;

	case EItemType::MEDKIT:
		ItemInfo.MaxNum = 2;
		ItemInfo.NeedTime = 8.f;
		ItemInfo.Supply = 1.f;
		break;

	case EItemType::CELL:
		ItemInfo.MaxNum = 4;
		ItemInfo.NeedTime = 3.f;
		ItemInfo.Supply = 0.25f;
		break;

	case EItemType::BATTERY:
		ItemInfo.MaxNum = 2;
		ItemInfo.NeedTime = 5.f;
		ItemInfo.Supply = 1.25f;
		break;

	case EItemType::THROWN:
		ItemInfo.MaxNum = 1;
		ItemInfo.Num = 1;
		ItemInfo.NeedTime = 0.f;
		ItemInfo.Supply = 0.f;
		break;

	case EItemType::ATTACHMENT:
		ItemInfo.MaxNum = 1;
		ItemInfo.Num = 1;
		ItemInfo.NeedTime = 0.f;
		ItemInfo.Supply = 0.f;
		break;
	}
}

/////////////////////////////////////////////////
// BeginPlay
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

}
////////////////////////////////////////////////
// Tick
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

////////////////////////////////////////////////
// 处理重叠碰撞\射线检测
void AItemBase::OnOverlapsBegin(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//EnablePickup();
	bCanPickup = true;

	if (DisplayInfo)
		DisplayInfo->showPickupTips(true);
}
void AItemBase::OnOverlapsEnd(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//DisablePickup();
	bCanPickup = false;

	if (DisplayInfo)
		DisplayInfo->showPickupTips(false);
}
void AItemBase::OnHit(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Impluse, const FHitResult& SweepResult)
{
	unimplemented();
}
void AItemBase::OnVisibleLineTraceINTFunc_Implementation()
{
	if (DisplayInfo && !DisplayInfo->IsInViewport())
	{
		DisplayInfo->AddToViewport();
		DisplayInfo->playInfoAnim();
	}
}


/////////////////////////////////////////////////
// 交互 //Interact_Interface接口函数
void AItemBase::InteractINTFunc_Implementation(APlayerCharacter* player)
{
	if (bCanPickup)
	{
		auto bag = player->GetBagComp();
		bag->GetItem(this);
		OwnerPlayer = player;
		RefreshDisplayInfo();
	}
}
void AItemBase::HideInfoINTFunc_Implementation()
{
	if (DisplayInfo->IsInViewport())
	{
		DisplayInfo->RemoveFromViewport();
	}
}
void AItemBase::RefreshDisplayInfo()
{
	DisplayInfo->bindNum(ItemInfo.Num);
}
void AItemBase::SetHideStatus(bool Toggle)
{
	SetActorEnableCollision(!Toggle);
	Mesh->SetSimulatePhysics(!Toggle);
	SetActorHiddenInGame(Toggle);
	RefreshDisplayInfo();
}
AItemBase* AItemBase::DuplicateItem(FItemInfo& NewInfo, bool StatusToggle)
{
	UClass* genClass = GetClass();

	auto dupItem = GetWorld()->SpawnActor<AItemBase>(genClass, GetActorTransform());
	dupItem->ItemInfo.ItemType = NewInfo.ItemType;
	dupItem->ItemInfo.Num = NewInfo.Num;
	dupItem->SetHideStatus(StatusToggle);
	return dupItem;
}
void AItemBase::UseItem()
{
	switch (ItemInfo.ItemType)
	{
	case EItemType::AIDS:
	case EItemType::MEDKIT:
		if (OwnerPlayer)
			OwnerPlayer->GetInfoComp()->GetHealthSupply(ItemInfo.Supply);
		break;

	case EItemType::CELL:
	case EItemType::BATTERY:
		if (OwnerPlayer)
			OwnerPlayer->GetInfoComp()->GetArmourSupply(ItemInfo.Supply);
		break;
	}

	if (ItemInfo.Num == 0)
		ConditionalBeginDestroy();
}
