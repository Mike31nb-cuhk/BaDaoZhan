// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanBaseGameMode.h"

void ABaDaoZhanBaseGameMode::AddEnemy(TScriptInterface<IEnemyInterface> Enemy)
{
	EnemyList.Add(Enemy);
	OnEnemyListChanged.ExecuteIfBound();
}

void ABaDaoZhanBaseGameMode::RemoveEnemy(TScriptInterface<IEnemyInterface> Enemy)
{
	EnemyList.Remove(Enemy);
	OnEnemyListChanged.ExecuteIfBound();
}
