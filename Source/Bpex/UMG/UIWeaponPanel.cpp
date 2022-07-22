// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWeaponPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UUIWeaponPanel::UpdateWeaponUI(FWeaponInfo& weaponInfo)
{
	UTexture2D* img = weaponInfo.WeaponIcon;
	FString name = weaponInfo.WeaponName;
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, name);
	WeaponImg->SetBrushFromTexture(img);
	WeaponName->SetText(FText::FromString(name));
}