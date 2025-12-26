// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "BaDaoZhan/AbilitySystem/BaDaoZhanAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "BaDaoZhan/Attributes/BaDaoZhanAttributeSet.h"
#include "BaDaoZhan/GameMode/BaDaoZhanBaseGameMode.h"
#include "BaDaoZhanBaseCharacter.generated.h"



UCLASS()
class BADAOZHAN_API ABaDaoZhanBaseCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ABaDaoZhanBaseCharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UFUNCTION(BlueprintCallable, Category = "ASC")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable, Category = "ASC")
	UAttributeSet* GetAttributeSet() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Sheath;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UBaDaoZhanAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	ABaDaoZhanBaseGameMode* ThisGameMode;
	
	UFUNCTION()
	void AddCharacterAbilities();

	UFUNCTION()
	ABaDaoZhanBaseGameMode* GetBaseGameMode() const;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditAnywhere,Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities = TArray<TSubclassOf<UGameplayAbility>>();
};