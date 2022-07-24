// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "HUDSupplyProgress.generated.h"

class UTexture2D;

UCLASS()
class BPEX_API UHUDSupplyProgress : public UUserWidget
{
	GENERATED_BODY()
	

public:
	FTimerHandle SupplyingTimer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* SupplementImg;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* SupplementName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* SupplyProgressBar;
	
	UPROPERTY()
		bool bIsTicking;
	UPROPERTY()
		float SupplyTimeLength;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void SetupSupplyData(float supplyTime, UTexture2D* img, FString name);

	UFUNCTION(BlueprintCallable)
		void SetSupplementImg(UTexture2D* img);

	UFUNCTION(BlueprintCallable)
		void SetSupplementName(const FString& name);

	UFUNCTION(BlueprintCallable)
		void SetProgress(float percent);

	UFUNCTION(BlueprintCallable)
		void SupplyFinished();
};
