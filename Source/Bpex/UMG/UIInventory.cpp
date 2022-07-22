// Fill out your copyright notice in the Description page of Project Settings.


#include "UIInventory.h"
#include "Kismet/GameplayStatics.h"
#include "Bpex/Character/PlayerCharacter.h"
#include "Bpex/UMG/UIBagPanel.h"
#include "Bpex/UMG/UIWeaponPanel.h"

void UUIInventory::SetupWidget()
{
	auto player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (player == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Can't get Player!"));
		return;
	}
	player->HUDWeaponInfoDel.AddUObject(this, &UUIInventory::UpdateWeaponPanel);

	auto playerBag = player->GetBagComp();
	if (playerBag != nullptr)
	{
		playerBag->UIInventoryDel.AddUObject(this, &UUIInventory::UpdateBagPanel);
	}

	auto inputComp = UGameplayStatics::GetPlayerController(GetWorld(), 0)->InputComponent;
	check(inputComp);
	inputComp->BindAction("Inventory", IE_Pressed, this, &UUIInventory::InputShowOrHide);
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("setup"));
}

void UUIInventory::UpdateBagPanel(TArray<AItemBase*>& BagList, int32 BagSize)
{
	BagPanel->UpdateBagUI(BagList, BagSize);
}
void UUIInventory::UpdateWeaponPanel(FWeaponInfo& WeaponInfo, int32 UseIdx)
{
	if (UseIdx == 1)
	{
		WeaponPanel_1->UpdateWeaponUI(WeaponInfo);
	}
	else
	{
		WeaponPanel_2->UpdateWeaponUI(WeaponInfo);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("update weapon"));
}

void UUIInventory::InputShowOrHide()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("show"));
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!bIsShowed)
	{
		AddToViewport();
		
		bIsShowed = true;

		pc->bShowMouseCursor = true;
		FInputModeUIOnly InputModeUIAndGame;
		InputModeUIAndGame.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		SetFocus();
		pc->SetInputMode(InputModeUIAndGame);
	}
	else
	{
		RemoveFromViewport();
		bIsShowed = false;

		pc->bShowMouseCursor = false;
		FInputModeGameOnly InputModeGameOnly;
		pc->SetInputMode(InputModeGameOnly);
	}
}