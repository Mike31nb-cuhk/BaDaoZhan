// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "BaDaoZhanEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM()
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM()
enum class EEffectRemovalPolicy
{
	RemoveOnEndOverlap,
	DoNotRemove,
};

USTRUCT(BlueprintType)
struct FEffectConfig
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Effect Config")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Effect Config")
	EEffectApplicationPolicy ApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Effect Config")
	EEffectRemovalPolicy RemovalPolicy = EEffectRemovalPolicy::DoNotRemove;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Effect Config")
	float Level = 1.f;
};

USTRUCT()
struct FActiveEffectSpecHandleInfo
{
	GENERATED_BODY()
	UPROPERTY()
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle;
	UPROPERTY()
	EEffectRemovalPolicy RemovalPolicy;
	UPROPERTY()
	UAbilitySystemComponent* TargetASC;
};

UCLASS()
class BADAOZHAN_API ABaDaoZhanEffectActor : public AActor
{
	GENERATED_BODY()
	
public:
	ABaDaoZhanEffectActor();
	
protected:
	virtual void BeginPlay() override;
	
	
	UFUNCTION(BlueprintCallable)
	void ApplyGameEffectToTarget(AActor* TargetActor, FEffectConfig EffectConfig);
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void EndOverlap(AActor* TargetActor);
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "Effect Setting")
	TArray<FEffectConfig> EffectConfigs;

	
	
	TArray<FActiveEffectSpecHandleInfo> ActiveEffectSpecHandleInfos;
};