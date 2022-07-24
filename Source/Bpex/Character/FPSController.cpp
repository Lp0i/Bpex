// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSController.h"
#include "PlayerCharacter.h"

AFPSController::AFPSController()
{
	InputComponent = CreateDefaultSubobject<UInputComponent>(TEXT("Input"));
}

void AFPSController::SetupInputComponent()
{
	InputComponent->BindAxis("LookUp", this, &AFPSController::LookUp);
	InputComponent->BindAxis("Turn", this, &AFPSController::TurnRight);
	InputComponent->BindAxis("MoveForward", this, &AFPSController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPSController::MoveRight);

	InputComponent->BindAction<FToggleEventDel>("Crouch_Hold", IE_Pressed, this, &AFPSController::ToggleCrouch, true);
	InputComponent->BindAction<FToggleEventDel>("Crouch_Hold", IE_Released, this, &AFPSController::ToggleCrouch, false);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSController::Jump);
	InputComponent->BindAction<FToggleEventDel>("Sprint", IE_Pressed, this, &AFPSController::ToggleSprint, true);
	InputComponent->BindAction<FToggleEventDel>("Sprint", IE_Released, this, &AFPSController::ToggleSprint, false);
	InputComponent->BindAction<FToggleEventDel>("ADS", IE_Pressed, this, &AFPSController::ToggleADS, true);
	InputComponent->BindAction<FToggleEventDel>("ADS", IE_Released, this, &AFPSController::ToggleADS, false);
	InputComponent->BindAction("Interact", IE_Pressed, this, &AFPSController::Interact);
	InputComponent->BindAction<FSwitchWeaponDel>("SwitchWeapon_1", IE_Pressed, this, &AFPSController::SwitchWeapon, int32(1));
	InputComponent->BindAction<FSwitchWeaponDel>("SwitchWeapon_2", IE_Pressed, this, &AFPSController::SwitchWeapon, int32(2));
	InputComponent->BindAction<FSwitchWeaponDel>("PackupWeapon", IE_Pressed, this, &AFPSController::SwitchWeapon, int32(0));
	InputComponent->BindAction<FToggleEventDel>("PrimaryFire", IE_Pressed, this, &AFPSController::ToggleFire, true);
	InputComponent->BindAction<FToggleEventDel>("PrimaryFire", IE_Released, this, &AFPSController::ToggleFire, false);
	InputComponent->BindAction("Reload", IE_Pressed, this, &AFPSController::Reload);

}

void AFPSController::BeginPlay()
{
	ControlPlayer = Cast<APlayerCharacter>(GetPawn());		
}


void AFPSController::LookUp(float val)
{
	LookUpVal = val;
	AddPitchInput(val);
	ControlPlayer->LookVal = val;
}

void AFPSController::TurnRight(float val)
{
	TurnRightVal = val;
	AddYawInput(val);
	ControlPlayer->TurnVal = val;
}

void AFPSController::MoveForward(float val)
{
	MoveFwdVal = val;

	if (val != 0.0f)
	{
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		ControlPlayer->Move(Direction, val);
	}
	if (val < 0)
		ToggleSprint(false);
}

void AFPSController::MoveRight(float val)
{
	MoveRightVal = val;

	if (val != 0.0f)
	{
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		ControlPlayer->Move(Direction, val);
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, Direction.ToString());
	}
}

void AFPSController::Jump()
{
	ControlPlayer->Jump();
}

void AFPSController::ToggleSprint(bool bToggle)
{ 
	if (bToggle)
	{
		if (MoveFwdVal > 0)
			ControlPlayer->ToggleSprint(bToggle);
	}
	else
		ControlPlayer->ToggleSprint(bToggle);
}

void AFPSController::ToggleCrouch(bool bToggle)
{
	ControlPlayer->ToggleCrouch(bToggle);
}

void AFPSController::ToggleADS(bool bToggle)
{
	ControlPlayer->ToggleADS(bToggle);
}

void AFPSController::Interact()
{
	ControlPlayer->InteractTracedObj();
}

void AFPSController::SwitchWeapon(int32 idx)
{
	ControlPlayer->SwitchWeapon(idx);
}

void AFPSController::ToggleFire(bool bToggle)
{
	if (bToggle)
		ControlPlayer->PrimaryFire();
	else
		ControlPlayer->StopFire();
}

void AFPSController::Reload()
{
	ControlPlayer->StartReload();
}