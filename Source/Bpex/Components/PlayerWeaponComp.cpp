// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeaponComp.h"
#include "Bpex/Character/PlayerCharacter.h"

// Sets default values for this component's properties
UPlayerWeaponComp::UPlayerWeaponComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bAutoActivate = true;

	UseWeaponIdx = 1;

	WeaponList.Add(nullptr);
	WeaponList.Add(nullptr);

	// ...
}


// Called when the game starts
void UPlayerWeaponComp::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UPlayerWeaponComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


//CustomEvent
void UPlayerWeaponComp::AttainWeapon(AWeaponBase* Weapon)
{
	int32 replaceIdx;

	if (WeaponList.Find(nullptr, replaceIdx))
	{
		WeaponList[replaceIdx] = Weapon;
		UseWeaponIdx = replaceIdx + 1;
	}
	else
	{
		auto removeWeapon = WeaponList[UseWeaponIdx - 1];
		removeWeapon->SetActorScale3D(FVector(1.1f));
		removeWeapon->SetActorLocation(GetOwner()->GetActorLocation());
		removeWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		removeWeapon->SetHideStatus(false);
		removeWeapon->GetMesh()->SetSimulatePhysics(true);
		removeWeapon->WeaponMesh->AddImpulse(GetOwner()->GetActorRotation().Vector() * 500.f);
		WeaponList[UseWeaponIdx - 1] = Weapon;
	}
}

void UPlayerWeaponComp::SwitchWeapon(int32 WeaponIdx)
{
	if (WeaponIdx != 0)
	{
		if (WeaponList[WeaponIdx - 1] != nullptr)
			UseWeaponIdx = WeaponIdx;
	}
}