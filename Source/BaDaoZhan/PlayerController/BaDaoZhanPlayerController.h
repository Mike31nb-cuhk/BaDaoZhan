// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BaDaoZhan/GameMode/BaDaoZhanBaseGameMode.h"
#include "BaDaoZhan/Interface/EnemyInterface.h"
#include "GameFramework/PlayerController.h"
#include "BaDaoZhanPlayerController.generated.h"

UENUM(BlueprintType)
enum class EDetectionMode : uint8
{
	BaDaoZhanDetection,
	CasualDetection,
	// 未来将添加其他模式
};

USTRUCT(BlueprintType)
struct FDetectionConfig
{
	GENERATED_BODY()

	// 检测模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	EDetectionMode DetectionMode = EDetectionMode::CasualDetection;

	// 通用参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	float DetectionRadius = 0; // 检测半径，用于所有模式

	// 拔刀斩特有参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection", meta = (EditCondition = "DetectionMode == EDetectionMode::BaDaoZhanDetection", EditConditionHides))
	float Length = 0; // 检测长度，仅用于拔刀斩	

	FDetectionConfig(){}

	FDetectionConfig(EDetectionMode InMode, float InRadius, float InLength = 400.0f)
		: DetectionMode(InMode), DetectionRadius(InRadius), Length(InLength) {
		switch (InMode) {
		case EDetectionMode::BaDaoZhanDetection:
			if (InRadius == 0){DetectionRadius = 200;}
			if (InLength == 0){Length = 400;}
			break;
		case EDetectionMode::CasualDetection:
			if (InRadius == 0){DetectionRadius = 100;}
			break;
		}
	}
};

USTRUCT()
struct FDetectionResultData
{
	GENERATED_BODY()
	FVector DetectedLocation;
	bool bIsSuccessful;
};



struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class BADAOZHAN_API ABaDaoZhanPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	// Interface Function For Debug Purpose
	UFUNCTION(BlueprintCallable)
	void DrawDebug(const bool bDrawDebug);
	bool bDrawDebugDetectionShape;

	// Interface Function for GA to change Detection Config
	UFUNCTION(BlueprintCallable)
	void SetDetectionConfig(const FDetectionConfig& NewConfig);
	
	// Interface Function is for Async Function
	FDetectionResultData GetDetectedData4BaDaoZhan();


protected:
	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;


	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputMappingContext> BaDaoZhanContext;
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	FInputActionHandlerSignature InputActionHandler;
	
private:
	// ASC target
	UAbilitySystemComponent* ASC;
	FGameplayTag TDTag;
	FGameplayTag TPTag;
	
	
	// World Context: EnemyList, Cursor and pawn location
	UPROPERTY()
	ABaDaoZhanBaseGameMode* ThisGameMode;
	TArray<TScriptInterface<IEnemyInterface>> EnemyList;
	void OnEnemyListChanged();
	FVector CursorLocation;
	FVector PawnLocation;
	FDetectionConfig DetectionConfig;
	
	// Cursor Trace
	void CursorTrace();
	
	// Where detection is determined. Called in void Tick()
	void DetectionTrace_TD();
	void DetectionTrace_TP();
	
	// Casual-Detection
	IEnemyInterface* EnemyThisFrame;
	IEnemyInterface* EnemyLastFrame;
	void CasualDetection_TD();
	
	// BaDaoZhan-Detection
	IEnemyInterface* EnemyAutoDetected;
	TArray<TScriptInterface<IEnemyInterface>> EnemiesHighlightedThisFrame;
	FGameplayTag ImmuneToBaDaoZhanDetection;
	float RecLength;
	void BaDaoZhanDetection_TD(FVector Direction = FVector(0.0f, 0.0f, 0.0f), 
		FVector RootLocation = FVector(0,0,0), bool bOverrideDirection = false);
	void BaDaoZhanDetection_TP();

	void OnCameraModeChanged(FGameplayTag Tag, int32 NewCount);

	
	virtual void PlayerTick(float DeltaTime) override;
};