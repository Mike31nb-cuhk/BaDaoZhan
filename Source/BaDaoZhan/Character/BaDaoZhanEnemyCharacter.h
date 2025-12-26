// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaDaoZhanBaseCharacter.h"
#include "BaDaoZhan/Interface/CombatInterface.h"
#include "BaDaoZhan/Interface/EnemyInterface.h"
#include "BaDaoZhan/UI/WidgetController/BaDaoZhanWidgetController.h"
#include "BaDaoZhanEnemyCharacter.generated.h"

UCLASS()
class BADAOZHAN_API ABaDaoZhanEnemyCharacter : public ABaDaoZhanBaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaDaoZhanEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void AbilitySystemComponentInitialize();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float Radius;
	
	// Enemy Interface
	virtual void HighlightActor(float) override;
	virtual void UnHightlightActor() override;
	virtual FVector GetLocation() override;
	virtual float GetRadius() override;
	virtual UAbilitySystemComponent* GetEnemyAbilitySystemComponent() override;
	
	// BlueprintImplementEvent
	UFUNCTION(BlueprintImplementableEvent)
	void Highlight(float ColorType);
	UFUNCTION(BlueprintImplementableEvent)
	void UnHighlight();
};