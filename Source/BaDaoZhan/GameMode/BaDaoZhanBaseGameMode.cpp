// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanBaseGameMode.h"

void ABaDaoZhanBaseGameMode::AddEnemy(TScriptInterface<IEnemyInterface> Enemy)
{
	EnemyList.Add(Enemy);
	OnEnemyListChanged.Broadcast();
}

void ABaDaoZhanBaseGameMode::RemoveEnemy(TScriptInterface<IEnemyInterface> Enemy)
{
	EnemyList.Remove(Enemy);
	OnEnemyListChanged.Broadcast();
}
