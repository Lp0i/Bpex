// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDDamageNum.generated.h"

class UTextBlock;
class UImage;
class UWidgetAnimation;

UCLASS()
class BPEX_API UHUDDamageNum : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* DamageNum;
	UPROPERTY(meta = (BindWidget))
		UImage* ArmourImg;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* FadeAnim;

	FTimerHandle CountTimer;
	FTimerHandle ResetTimer;
	float DamageCount = 0;

public:
	UFUNCTION()
		void AddDamage(float damage, int32 level, bool bIsArmourBreak);

	UFUNCTION()
		virtual bool Initialize() override;

	UFUNCTION()
		void PlayFadeAnim();

	UFUNCTION()
		void ResetCount() { DamageCount = 0; RemoveFromViewport(); }

	UFUNCTION()
		FLinearColor GetColorByLevel(int32 level);
};
