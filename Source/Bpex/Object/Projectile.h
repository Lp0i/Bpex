// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class BPEX_API AProjectile : public AActor
{
	GENERATED_BODY()

		FTimerHandle DestroyTimer;

	UFUNCTION()
		void OnTimeDestroy() { ConditionalBeginDestroy(); };

public:
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USceneComponent* DefaultRoot;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mesh")
		class UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
		class UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
