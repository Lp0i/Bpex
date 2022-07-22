// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyInfoComp.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UEnemyInfoComp::UEnemyInfoComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyInfoComp::BeginPlay()
{
	Super::BeginPlay();

	CurArmour = MaxArmour;
	CurHealth = MaxHealth;
	// ...

}


// Called every frame
void UEnemyInfoComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UEnemyInfoComp::GetHurt(float Damage, EDamageType DamageType, const FVector& HurtLoc)
{
	switch (DamageType)
	{
	case EDamageType::COMMON:
		CurArmour -= Damage;
		if (CurArmour > 0)
		{
			PlaySoundsDel.ExecuteIfBound(EHitType::ARMOUR_HIT);
			EmitParticleDel.ExecuteIfBound(EHitType::ARMOUR_HIT, HurtLoc);
		}
		else
		{
			CurHealth += CurArmour;
			CurArmour = 0;
			if (!bIsArmourBreak)
			{
				bIsArmourBreak = true;
				PlaySoundsDel.ExecuteIfBound(EHitType::ARMOUR_BREAK);
				EmitParticleDel.ExecuteIfBound(EHitType::ARMOUR_BREAK, HurtLoc);
			}
			PlaySoundsDel.ExecuteIfBound(EHitType::FLESH_HIT);
			PlayAnimDel.ExecuteIfBound(EHitType::FLESH_HIT);
			EmitParticleDel.ExecuteIfBound(EHitType::FLESH_HIT, HurtLoc);
		}
		break;

	case EDamageType::ARMOUR_ONLY:
		CurArmour -= Damage;
		if (CurArmour < 0)
		{
			CurArmour = 0;
			if (!bIsArmourBreak)
			{
				bIsArmourBreak = true;
				PlaySoundsDel.ExecuteIfBound(EHitType::ARMOUR_BREAK);
			}
		}
		break;

	case EDamageType::HEALTH_ONLY:
		CurHealth -= Damage;

		break;
	}

	if (CurHealth <= 0 && !bIsDead)
	{
		PlaySoundsDel.ExecuteIfBound(EHitType::HIT_DOWN);
		PlayAnimDel.ExecuteIfBound(EHitType::DEAD);
		bIsDead = true;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, FString::SanitizeFloat(CurHealth));
}

void UEnemyInfoComp::GetSupply(float Supply, EDamageType DamageType)
{

}