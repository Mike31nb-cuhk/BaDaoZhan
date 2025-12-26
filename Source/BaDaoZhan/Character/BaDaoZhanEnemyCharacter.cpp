// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanEnemyCharacter.h"


// Sets default values
ABaDaoZhanEnemyCharacter::ABaDaoZhanEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaDaoZhanEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponentInitialize();
	GetBaseGameMode()->AddEnemy(TScriptInterface<IEnemyInterface>(this));
}

void ABaDaoZhanEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetBaseGameMode()->RemoveEnemy(this);
}

void ABaDaoZhanEnemyCharacter::AbilitySystemComponentInitialize()
{
	if (!AbilitySystemComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "AbilitySystemComponent is null in ABaDaoZhanEnemyCharacter::AbilitySystemComponentInitialize");
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent is null in ABaDaoZhanEnemyCharacter::AbilitySystemComponentInitialize"));
		return;
	}
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	AbilitySystemComponent->OnAbilityActorInfoSet();
}

// Called every frame
void ABaDaoZhanEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaDaoZhanEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaDaoZhanEnemyCharacter::HighlightActor(float ColorType)
{
	Highlight(ColorType);
}

void ABaDaoZhanEnemyCharacter::UnHightlightActor()
{
	UnHighlight();
}

FVector ABaDaoZhanEnemyCharacter::GetLocation()
{
	return GetActorLocation();
}

float ABaDaoZhanEnemyCharacter::GetRadius()
{
	return Radius;
}

UAbilitySystemComponent* ABaDaoZhanEnemyCharacter::GetEnemyAbilitySystemComponent()
{
	return GetAbilitySystemComponent();
}
