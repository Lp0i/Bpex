// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "ObjInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class BPEX_API UObjInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* HorizonPanel;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ObjName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Num;
	UPROPERTY(meta = (BindWidget))
	class UImage* ObjImg;
	UPROPERTY(meta = (BindWidget))
	class UOverlay* PickupTips;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* FadeAnim;

public:
	FORCEINLINE void playInfoAnim() { PlayAnimation(FadeAnim); }
	FORCEINLINE void showPickupTips(bool Toggle) {
		if (Toggle) PickupTips->Visibility = ESlateVisibility::Visible;
		else PickupTips->Visibility = ESlateVisibility::Hidden;
	}

	void bindType(FText text);
	void bindNum(int32 num);
	void bindImg(UTexture2D* img);
	void setBoxLoc(FVector2D NewLocation);
};
