// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanBaseCharacter.h"
#include "BaDaoZhan/AbilitySystem/BaDaoZhanAbilitySystemComponent.h"
#include "BaDaoZhan/Attributes/BaDaoZhanAttributeSet.h"
#include "BaDaoZhan/GameMode/BaDaoZhanBaseGameMode.h"


// Sets default values
ABaDaoZhanBaseCharacter::ABaDaoZhanBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UBaDaoZhanAbilitySystemComponent>(TEXT("AbilitySystem"));
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create AbilitySystemComponent in ABaDaoZhanBaseCharacter"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AbilitySystemComponent created successfully in ABaDaoZhanBaseCharacter"));
	}
	check(AbilitySystemComponent)
}

UAbilitySystemComponent* ABaDaoZhanBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ABaDaoZhanBaseCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

// Called when the game starts or when spawned
void ABaDaoZhanBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	ThisGameMode = Cast<ABaDaoZhanBaseGameMode>(GetWorld()->GetAuthGameMode());
	check(ThisGameMode)
	AddCharacterAbilities();
}

void ABaDaoZhanBaseCharacter::AddCharacterAbilities()
{
	AbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
}

ABaDaoZhanBaseGameMode* ABaDaoZhanBaseCharacter::GetBaseGameMode() const
{
	return ThisGameMode;
}

// Called every frame
void ABaDaoZhanBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaDaoZhanBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}