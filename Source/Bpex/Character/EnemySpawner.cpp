// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

void AEnemySpawner::BeginPlay()
{
	SpawnEnemy();
}

void AEnemySpawner::SpawnEnemy()
{
	if (EnemyClass != nullptr)
	{
		SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBase>(EnemyClass, GetActorTransform());
		SpawnedEnemy->RespawnDel.BindUObject(this, &AEnemySpawner::RespawnEnemy);
	}
}

void AEnemySpawner::RespawnEnemy()
{
	if (EnemyClass != nullptr)
	{
		GetWorldTimerManager().SetTimer(RespawnTimer, this, &AEnemySpawner::SpawnEnemy, RespawnTimeSpan);
	}
}