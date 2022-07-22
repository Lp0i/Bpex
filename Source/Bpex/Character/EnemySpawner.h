// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "EnemyBase.h"
#include "EnemySpawner.generated.h"

/**
 * 
 */
UCLASS()
class BPEX_API AEnemySpawner : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Spawning")
		TSubclassOf<AEnemyBase> EnemyClass;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Spawning")
		AEnemyBase* SpawnedEnemy;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Spawning")
		float RespawnTimeSpan = 4.0f;

	FTimerHandle RespawnTimer;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void SpawnEnemy();

	UFUNCTION(BlueprintCallable)
		void RespawnEnemy();
};
