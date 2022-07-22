// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoComp.h"
#include "Bpex/Object/OutFitBase.h"
#include "Bpex/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPlayerInfoComp::UPlayerInfoComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;



	// ...
}


// Called when the game starts
void UPlayerInfoComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	HUDInfoDel.ExecuteIfBound(MaxEvoValue - CurEvoValue, ArmourLevel, (CurArmour / MaxArmour), (CurHealth / MaxHealth));
}


// Called every frame
void UPlayerInfoComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UPlayerInfoComp::GetDamage(float damage)
{
	CurArmour -= damage;

	if (CurArmour < 0.f)
	{
		CurHealth += CurArmour;
		CurArmour = 0.f;

		if (CurHealth <= 0.f)
			bIsDead = true;
	}

	HUDInfoDel.ExecuteIfBound(MaxEvoValue - CurEvoValue, ArmourLevel, (CurArmour / MaxArmour), (CurHealth / MaxHealth));
}


void UPlayerInfoComp::AttainArmour(AOutfitBase* attainArmour)
{
	FOutfitInfo& attainInfo = attainArmour->OutfitInfo;

	if (EquippedArmour != nullptr)
	{
		FOutfitInfo& curInfo = EquippedArmour->OutfitInfo;
		curInfo.AdditiveValue = MaxArmour;
		curInfo.CurrentValue = CurArmour;
		curInfo.EvoValue = CurEvoValue;
		curInfo.MaxEvoValue = MaxEvoValue;
		curInfo.Level = ArmourLevel;
	}

	CurArmour = attainInfo.CurrentValue;
	MaxArmour = attainInfo.AdditiveValue;
	CurEvoValue = attainInfo.EvoValue;
	MaxEvoValue = attainInfo.MaxEvoValue;
	ArmourLevel = attainInfo.Level;
	bArmourSpecialBuff = attainInfo.bSpecialBuff;

	EquippedArmour = attainArmour;
	attainArmour->SetHideStatus(true);

	HUDInfoDel.ExecuteIfBound(MaxEvoValue - CurEvoValue, ArmourLevel, (CurArmour / MaxArmour), (CurHealth / MaxHealth));
}

void UPlayerInfoComp::AttainHelmet(AOutfitBase* attainHelmet)
{
	FOutfitInfo& attainInfo = attainHelmet->OutfitInfo;

	bHelmetSpecialBuff = attainInfo.bSpecialBuff;
	HelmetLevel = attainInfo.Level;
	HeadShotDefence = attainInfo.AdditiveValue;

	EquippedHelmet = attainHelmet;
	attainHelmet->SetHideStatus(true);
}

void UPlayerInfoComp::ArmourEvolution(float damage)
{
	if (ArmourLevel == 0 || ArmourLevel == 4)
		return;

	CurEvoValue += damage;

	if (CurEvoValue >= MaxEvoValue)
	{
		++ArmourLevel;
		CurEvoValue -= MaxEvoValue;
		if (ArmourUpgradeSoundEffect != nullptr)
			UGameplayStatics::PlaySound2D(GetWorld(), ArmourUpgradeSoundEffect);

		switch (ArmourLevel)
		{
		case 2:
			MaxArmour += 0.25f;
			MaxEvoValue = 300.f;
			break;

		case 3:
			MaxArmour += 0.25f;
			MaxEvoValue = 750.f;
			break;

		case 4:
			MaxArmour += 0.25f;
			MaxEvoValue = INT32_MAX;
			break;
		}
	}

	HUDInfoDel.ExecuteIfBound(MaxEvoValue - CurEvoValue, ArmourLevel, (CurArmour / MaxArmour), (CurHealth / MaxHealth));
}