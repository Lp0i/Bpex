// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Bpex/Interface/InteractableINT.h"
#include "Bpex/Interface/PickupINT.h"
#include "OutFitBase.generated.h"

class APlayerCharacter;

UENUM(BlueprintType)
enum class EOutfitType:uint8
{
	HELMET,
	ARMOUR,
	BAG,
};

USTRUCT(BlueprintType)
struct BPEX_API FOutfitInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		EOutfitType OutfitType;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		FString OutfitName;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		int32 Level = 1;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		float AdditiveValue = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		float CurrentValue = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		float MaxEvoValue = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		float EvoValue = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		bool bSpecialBuff = false;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		UTexture2D* OutfitImg;
};

UCLASS()
class BPEX_API AOutfitBase : public AActor, public IInteractableINT, public IPickupINT
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Info")
		FOutfitInfo OutfitInfo;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "HUD")
		class UObjInfoWidget* DisplayInfo;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Info")
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Trace")
		USphereComponent* DetectSphere;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Trace")
		bool bCanPickup = false;


	// Sets default values for this actor's properties
	AOutfitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// InteractableINT接口
	virtual void InteractINTFunc_Implementation(APlayerCharacter* player) override;
	// PickupINT接口
	virtual void OnVisibleLineTraceINTFunc_Implementation() override;
	virtual void HideInfoINTFunc_Implementation() override;

	//重叠、射线检测事件
	UFUNCTION(BlueprintCallable)
		void OnOverlapsBegin(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult) {
		bCanPickup = true;
	}
	UFUNCTION(BlueprintCallable)
		void OnOverlapsEnd(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
		bCanPickup = false;
	}
	//UFUNCTION(BlueprintCallable)
	//	void OnOutFitVisibleLineTrace();

	UFUNCTION(BlueprintCallable)
		void SetHideStatus(bool bToggle);

};
