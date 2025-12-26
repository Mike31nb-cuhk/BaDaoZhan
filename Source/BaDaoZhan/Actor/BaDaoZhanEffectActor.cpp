// Fill out your copyright notice in the Description page of Project Settings.
#include "BaDaoZhanEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


ABaDaoZhanEffectActor::ABaDaoZhanEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void ABaDaoZhanEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABaDaoZhanEffectActor::OnOverlap(AActor* TargetActor)
{
	// 应用被标记为 ApplyOnOverlap 的 effect
	for (const FEffectConfig IEffectConfig:EffectConfigs)
	{
		if (IEffectConfig.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyGameEffectToTarget(TargetActor,IEffectConfig);
		}
	}
}

void ABaDaoZhanEffectActor::EndOverlap(AActor* TargetActor)
{
	// 应用被标记为 ApplyOnEndOverlap 的 effect
	for (const FEffectConfig IEffectConfig:EffectConfigs)
	{
		if (IEffectConfig.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyGameEffectToTarget(TargetActor,IEffectConfig);
		}
	}

	// 局部数组，用来记录需要移除的 ActiveEffectSpecHandleInfo 的索引
	TArray<int32> IndicesToRemove;
	int32 index = 0;

	// 取消被标记为 RemoveOnEndOverlap 的运行时 EffectHandle
	for (const FActiveEffectSpecHandleInfo IActiveEffectSpecHandleInfos:ActiveEffectSpecHandleInfos)
	{
		if (IActiveEffectSpecHandleInfos.RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			IActiveEffectSpecHandleInfos.TargetASC->RemoveActiveGameplayEffect(IActiveEffectSpecHandleInfos.ActiveGameplayEffectHandle,1);
		}
		IndicesToRemove.Add(index);
		index++;
	}

	// 从后向前移除被标记的 IActiveEffectSpecHandleInfos，避免索引错乱
	for (int32 i = IndicesToRemove.Num() - 1; i >= 0; --i)
	{
		ActiveEffectSpecHandleInfos.RemoveAt(IndicesToRemove[i]);
	}
}

void ABaDaoZhanEffectActor::ApplyGameEffectToTarget(AActor* Target, const FEffectConfig EffectConfig)
{
	// 获取并制造参数
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!TargetASC) return;
	check(EffectConfig.GameplayEffectClass)
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectConfig.GameplayEffectClass,EffectConfig.Level,EffectContextHandle);

	// 应用 GameEffectSpec，并存储运行时 GameEffectHandle
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle= TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	// 存储 IActiveEffectSpecHandleInfos
	if (EffectSpecHandle.Data.Get()->Def->DurationPolicy != EGameplayEffectDurationType::Instant && EffectConfig.RemovalPolicy != EEffectRemovalPolicy::DoNotRemove)
	{
		ActiveEffectSpecHandleInfos.Add(FActiveEffectSpecHandleInfo(ActiveGameplayEffectHandle,EffectConfig.RemovalPolicy,TargetASC));
	}
}