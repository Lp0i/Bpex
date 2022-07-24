// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Bpex/UMG/ObjInfoWidget.h"
#include "Bpex/Interface/InteractableINT.h"
#include "Bpex/Interface/PickupINT.h"
#include "ItemBase.generated.h"

class APlayerCharacter;

UENUM(BlueprintType)
enum class EItemType :uint8
{
	ATTACHMENT,
	AMMO,
	AIDS,
	MEDKIT,
	CELL,
	BATTERY,
	THROWN,
};

USTRUCT(BlueprintType)
struct BPEX_API FItemInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Type")
		EItemType ItemType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Count")
		int32 Num = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Count")
		int32 MaxNum = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float Supply = 0.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		float NeedTime = 0.f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		class UTexture2D* Icon;
};


UCLASS()
class BPEX_API AItemBase : public AActor, public IInteractableINT, public IPickupINT
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemBase();

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Info")
		FItemInfo ItemInfo;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Info")
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "HUD")
		UObjInfoWidget* DisplayInfo;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Trace")
		USphereComponent* DetectSphere;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Trace")
		bool bCanPickup = false;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Trace")
		class APlayerCharacter* OwnerPlayer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool isMax() { return ItemInfo.Num == ItemInfo.MaxNum; }
	FORCEINLINE void EnablePickup() { bCanPickup = true; }
	FORCEINLINE void DisablePickup() { bCanPickup = false; }

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�ڵ��ù��캯������ʼ������Component����ø���д�ĺ���
	virtual void PostInitializeComponents() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent) override;

	//��дInteract_Interface�ӿڷ���
	virtual void InteractINTFunc_Implementation(APlayerCharacter* player) override;
	//
	virtual void OnVisibleLineTraceINTFunc_Implementation() override;
	virtual void HideInfoINTFunc_Implementation() override;

	//��Ҽ�����ʱ���踴��
	UFUNCTION(BlueprintCallable)
		AItemBase* DuplicateItem(FItemInfo& NewInfo, bool StatusToggle);

	//��Ʒ����Ϸ�е�״̬
	UFUNCTION(BlueprintCallable)
		void SetHideStatus(bool Toggle);

	//������Ʒ��ϢHUD
	UFUNCTION(BlueprintCallable)
		void RefreshDisplayInfo();

	//ʹ����Ʒ
	UFUNCTION(BlueprintCallable)
		void UseItem();
	
	//�ص������߼���¼�
	UFUNCTION(BlueprintCallable)
		void OnOverlapsBegin(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		void OnOverlapsEnd(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable)
		void OnHit(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Impluse, const FHitResult& SweepResult);
};
