// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanCharacter.h"

#include "BaDaoZhan/AbilitySystem/BaDaoZhanAbilitySystemComponent.h"
#include "BaDaoZhan/UI/HUD/BaDaoZhanHUD.h"
#include "GameFramework/GameModeBase.h"


// Sets default values
ABaDaoZhanCharacter::ABaDaoZhanCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("Katana_weapon_01"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Sheath = CreateDefaultSubobject<USkeletalMeshComponent>("Sheath");
	Sheath->SetupAttachment(GetMesh(), FName("Katana_sheath_01"));
	Sheath->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AttributeSet = CreateDefaultSubobject<UBaDaoZhanAttributeSet>("AttributeSet");
}

// Called when the game starts or when spawned·
void ABaDaoZhanCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponentInitialize();
	check(GetAbilitySystemComponent())
	check(GetAttributeSet())
	check(GetLocalViewingPlayerController()->GetHUD())
	check(Cast<ABaDaoZhanHud>(GetLocalViewingPlayerController()->GetHUD()))
	Cast<ABaDaoZhanHud>(GetLocalViewingPlayerController()->GetHUD())->InitOverlay(GetAbilitySystemComponent(),GetAttributeSet());
	BaDaoZhanPC = Cast<ABaDaoZhanPlayerController>(GetController());
}

void ABaDaoZhanCharacter::AbilitySystemComponentInitialize()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	AbilitySystemComponent->OnAbilityActorInfoSet();
}

// Called every frame
void ABaDaoZhanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaDaoZhanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}