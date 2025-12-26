// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "VectorUtil.h"
#include "GameFramework/Character.h"

UBaDaoZhanAttributeSet::UBaDaoZhanAttributeSet():Super()
{
	InitHealth(100.);
	InitMaxHealth(100.);
	InitStamina(10);
	InitMaxStamina(10);
}

void UBaDaoZhanAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// Super::PreAttributeChange(Attribute, NewValue);
	// if (Attribute == GetHealthAttribute())
	// {
	// 	NewValue = FMath::Clamp(NewValue,0.,MaxHealth.GetCurrentValue());
	// }
	// if (Attribute == GetStaminaAttribute())
	// {
	// 	NewValue = FMath::Clamp(NewValue,0.,MaxStamina.GetCurrentValue());
	// }
}

void UBaDaoZhanAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	
	Super::PostGameplayEffectExecute(Data);
	UpdateEffectProperties(Data);

	// if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	// {
	// 	SetHealth(FMath::Clamp(GetHealth(),0,GetMaxHealth()));
	// }
	// if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	// {
	// 	SetStamina(FMath::Clamp(GetStamina(),0,GetMaxStamina()));
	// }
}

void UBaDaoZhanAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.,MaxHealth.GetCurrentValue());
	}
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.,MaxStamina.GetCurrentValue());
	}
}

void UBaDaoZhanAttributeSet::UpdateEffectProperties(const FGameplayEffectModCallbackData& Data)
{
	
	//	命名规则： Source = Causer of effect, Target: Target of effect (也就是owner of this AS)
	const FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
	const UAbilitySystemComponent* SourceASC = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	const AActor* SourceActor;
	const AController* SourceController;
	if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->OwnerActor.IsValid())
	{
		SourceActor = SourceASC->AbilityActorInfo->OwnerActor.Get();
		SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();

		// 以下为 “保险措施”，若报错则酌情增加
		
		if (!SourceController && SourceActor)
		{
			UE_LOG(LogTemp,Warning,TEXT("Source Controller is not available from the ASC"));
			if (const APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}
		// if (SourceController)
		// {
		// 	ACharacter* SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
		// }
	}
	
	const UAbilitySystemComponent* TargetASC = &Data.Target;
	const AActor* TargetActor;
	const AController* TargetController;
	if (IsValid(TargetASC) && TargetASC->AbilityActorInfo.IsValid() && IsValid(TargetASC->AbilityActorInfo->OwnerActor.Get()))
	{
		TargetActor = TargetASC->AbilityActorInfo->OwnerActor.Get();
		TargetController = TargetASC->AbilityActorInfo->PlayerController.Get();
		
		if (!TargetController && TargetActor)
		{
			UE_LOG(LogTemp,Warning,TEXT("Source Controller is not available from the ASC"));
			if (const APawn* Pawn = Cast<APawn>(TargetActor))
			{
				TargetController = Pawn->GetController();
			}
		}
		// if (TargetController)
		// {
		// 	ACharacter* TargetCharacter = Cast<ACharacter>(TargetController->GetPawn());
		// }
	}
	LastEffectProperties = FEffectProperties(SourceASC,TargetASC,SourceActor,TargetActor,SourceController,TargetController,EffectContextHandle);
}