// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSController.generated.h"

/**
 * 
 */
UCLASS()
class BPEX_API AFPSController : public APlayerController
{
	GENERATED_BODY()

	// BindAction���ô��� bool���Ͳ���
	DECLARE_DELEGATE_OneParam(FToggleEventDel, bool)
	// �л��������ô��� int32���Ͳ���
	DECLARE_DELEGATE_OneParam(FSwitchWeaponDel, int32)


	UPROPERTY()
		class APlayerCharacter* ControlPlayer;

public:
	AFPSController();

	virtual void SetupInputComponent() override;

	UPROPERTY(BlueprintReadWrite, Category = "Moving Data")
		float MoveRightVal;
	UPROPERTY(BlueprintReadWrite, Category = "Moving Data")
		float TurnRightVal;
	UPROPERTY(BlueprintReadWrite, Category = "Moving Data")
		float LookUpVal;
	UPROPERTY(BlueprintReadWrite, Category = "Moving Data")
		float MoveFwdVal;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void LookUp(float val);
	UFUNCTION()
		void TurnRight(float val);
	UFUNCTION()
		void MoveForward(float val);
	UFUNCTION()
		void MoveRight(float val);
	UFUNCTION()
		void Jump();
	UFUNCTION()
		void ToggleSprint(bool bToggle);
	UFUNCTION()
		void ToggleCrouch(bool bToggle);
	UFUNCTION()
		void ToggleADS(bool bToggle);
	UFUNCTION()
		void Interact();
	UFUNCTION()
		void SwitchWeapon(int32 idx);
	UFUNCTION()
		void ToggleFire(bool bToggle);
	UFUNCTION()
		void Reload();

	UFUNCTION()
		void SupplyInputChange(bool bToggle);
};
