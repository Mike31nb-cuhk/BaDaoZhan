// Fill out your copyright notice in the Description page of Project Settings.	

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaDaoZhanAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()
	
	FEffectProperties(): SourceASC(nullptr)
	, TargetASC(nullptr)
	, SourceActor(nullptr)
	, TargetActor(nullptr)
	, SourceController(nullptr)
	, TargetController(nullptr)
	, EffectContextHandle(){};
	
	FEffectProperties(const UAbilitySystemComponent* SourceASCReceived,const UAbilitySystemComponent* TargetASCReceived,
		const AActor* SourceActorReceived, const AActor* TargetActorReceived,const AController* SourceControllerReceived,
		const AController* TargetControllerReceived,FGameplayEffectContextHandle EffectContextHandleReceived)
	{
		SourceASC = SourceASCReceived;
		TargetASC = TargetASCReceived;
		SourceActor = SourceActorReceived;
		TargetActor = TargetActorReceived;
		SourceController = SourceControllerReceived;
		TargetController = TargetControllerReceived;
		EffectContextHandle = EffectContextHandleReceived;
	}
	
	UPROPERTY()
	const UAbilitySystemComponent* SourceASC;
	UPROPERTY()
	const UAbilitySystemComponent* TargetASC;
	UPROPERTY()
	const AActor* SourceActor;
	UPROPERTY()
	const AActor* TargetActor;
	UPROPERTY()
	const AController* SourceController;
	UPROPERTY()
	const AController* TargetController;
	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle;
};

/**
 * 
 */
struct FGameplayEffectModCallbackData;
UCLASS()
class BADAOZHAN_API UBaDaoZhanAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBaDaoZhanAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vital attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaDaoZhanAttributeSet, Health)
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vital attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaDaoZhanAttributeSet, MaxHealth)
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vital attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBaDaoZhanAttributeSet, Stamina)
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vital attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UBaDaoZhanAttributeSet, MaxStamina)
	
	UPROPERTY(BlueprintType,EditAnywhere)
	FEffectProperties LastEffectProperties;
	
private:
	void UpdateEffectProperties(const FGameplayEffectModCallbackData& Data);
};