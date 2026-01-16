// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanPlayerController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BaDaoZhan/AbilitySystem/BaDaoZhanAbilitySystemLibrary.h"
#include "BaDaoZhan/Attributes/BaDaoZhanAttributeSet.h"
#include "BaDaoZhan/Interface/EnemyInterface.h"
#include "Nodes/Blends/LookAtBlendCameraNode.h"

void ABaDaoZhanPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Movement Input Initialize
	check(BaDaoZhanContext)
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem)
	Subsystem->AddMappingContext(BaDaoZhanContext,0);
	// Detection Initialize
	ThisGameMode = Cast<ABaDaoZhanBaseGameMode>(GetWorld()->GetAuthGameMode());
	EnemyList = ThisGameMode->EnemyList;
	ThisGameMode->OnEnemyListChanged.AddUObject(this,&ABaDaoZhanPlayerController::OnEnemyListChanged);
	ImmuneToBaDaoZhanDetection = FGameplayTag::RequestGameplayTag(FName("BaDaoZhan.Enemy.BaDaoZhanUnAutoDetected"));
	
	ULookAtBlendCameraNode::OnLookAtBlendStarted.AddUObject(this,&ABaDaoZhanPlayerController::OnLookAtBlendStarted);
	ULookAtBlendCameraNode::OnLookAtBlendFinished.AddUObject(this,&ABaDaoZhanPlayerController::OnLookAtBlendFinished);
	
	ASC = Cast<IAbilitySystemInterface>(GetPawn())->GetAbilitySystemComponent();
	TDTag = FGameplayTag::RequestGameplayTag("BaDaoZhan.Camera.Mode.TD");
	TPTag = FGameplayTag::RequestGameplayTag("BaDaoZhan.Camera.Mode.TP");
	ASC->RegisterGameplayTagEvent(TDTag, EGameplayTagEventType::NewOrRemoved)
	.AddUObject(this, &ThisClass::OnCameraModeChanged);
	ASC->RegisterGameplayTagEvent(TPTag, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ThisClass::OnCameraModeChanged);
}

void ABaDaoZhanPlayerController::OnLookAtBlendStarted()
{
	UBaDaoZhanAbilitySystemLibrary::SetViewModeTag(this,ECameraViewMode::Transition);
}

void ABaDaoZhanPlayerController::OnLookAtBlendFinished()
{
	UBaDaoZhanAbilitySystemLibrary::SetViewModeTag(this,ECameraViewMode::NonTransition);
}

void ABaDaoZhanPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	// Bind TriggerEvent of MoveAction to lambda move function
	EnhancedInputComponent->BindActionValueLambda(MoveAction,ETriggerEvent::Triggered,[this](const FInputActionValue& InputActionValue)->void
	{
		const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
		
		// 从 Controller 的 Yaw Rotation 获取 Forward Vector 和 Right Vector
		const FRotator YawRotation = FRotator(0,GetControlRotation().Yaw,0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		if (APawn* ControlledPawn = GetPawn())
		{
			ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
			ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
			// UE_LOG(LogTemp, Log, TEXT("Direction=%f, Y=%f, Z = %f"), ForwardDirection.X * InputAxisVector.Y
			// 	+ RightDirection.X*InputAxisVector.X, ForwardDirection.Y*InputAxisVector.Y + RightDirection.Y*InputAxisVector.X
			// 	,ForwardDirection.Z*InputAxisVector.Y + RightDirection.Z*InputAxisVector.X);
			FVector InputDir = FVector(ForwardDirection.X * InputAxisVector.Y + RightDirection.X * InputAxisVector.X,
						   ForwardDirection.Y * InputAxisVector.Y + RightDirection.Y * InputAxisVector.X,
						   0.f);

		if (!InputDir.IsNearlyZero())
		{
			FRotator TargetRot = InputDir.Rotation();
			TargetRot.Pitch = 0.f; // 保持水平
			TargetRot.Roll = 0.f;
    
			// 平滑旋转
			ControlledPawn->SetActorRotation(FMath::RInterpTo(ControlledPawn->GetActorRotation(), TargetRot, 0.2, 10.f));
		}
		}
	}
	);
}



void ABaDaoZhanPlayerController::OnCameraModeChanged(FGameplayTag Tag, int32 NewCount)
{
	if (Tag == TPTag && NewCount != 0)
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	if (Tag == TDTag && NewCount != 0)
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
}

void ABaDaoZhanPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	if (ASC->HasMatchingGameplayTag(TDTag))
	{
		ViewMode = EViewMode::TD;
		bShowMouseCursor = true;
		DefaultMouseCursor = EMouseCursor::TextEditBeam;
		FInputModeGameAndUI InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputModeData.SetHideCursorDuringCapture(false);
		SetInputMode(InputModeData);
		DetectionTrace_TD();
	}
	if (ASC->HasMatchingGameplayTag(TPTag))
	{
		ViewMode = EViewMode::TP;
		bShowMouseCursor = false;
		DetectionTrace_TP();
	}
}

void ABaDaoZhanPlayerController::CursorTrace()
{
	PawnLocation = GetPawn()->GetActorLocation();
	FVector WorldOrigin, WorldDirection;
	DeprojectMousePositionToWorld(WorldOrigin, WorldDirection);
	FPlane GroundPlane(FVector::UpVector, PawnLocation.Z);  // 创建Pawn所在的水平面
	FVector RayEnd = WorldOrigin + WorldDirection * 100000.f; // 创建一个射线终点（比如向前延伸1万单位）
	CursorLocation = FMath::LinePlaneIntersection(WorldOrigin, RayEnd, GroundPlane); // 射线与水平面的交点
}

void ABaDaoZhanPlayerController::DetectionTrace_TD()
{
	// 进行 Detection
	switch (DetectionConfig.DetectionMode)
	{
		// Trace Mode 1: BaDaoZhanAutoDetection
		case EDetectionMode::BaDaoZhanDetection:
			BaDaoZhanDetection();
			break;
		// Trace Mode 2: CasualDetection
		case EDetectionMode::CasualDetection:
			CasualDetection_TD();
			break;
	}
}

void ABaDaoZhanPlayerController::DetectionTrace_TP()
{	
	switch (DetectionConfig.DetectionMode)
	{
		// Trace Mode 1: BaDaoZhanAutoDetection
		case EDetectionMode::BaDaoZhanDetection:
			BaDaoZhanDetection_TP();
			break;
		// Trace Mode 2: CasualDetection
		case EDetectionMode::CasualDetection:
			break;
	}
}

void ABaDaoZhanPlayerController::DrawDebug(const bool bDrawDebug)
{
	bDrawDebugDetectionShape = bDrawDebug;
}

void ABaDaoZhanPlayerController::OnEnemyListChanged()
{
	EnemyList = ThisGameMode->EnemyList;
}

void ABaDaoZhanPlayerController::SetDetectionConfig(const FDetectionConfig& NewConfig)
{
	DetectionConfig = NewConfig;
	RecLength = FMath::Sqrt(FMath::Square(DetectionConfig.Length) - FMath::Square(DetectionConfig.DetectionRadius));
	// 处理前一帧 BaDaoZhanDetection 留下的烂摊子
	if (DetectionConfig.DetectionMode != EDetectionMode::BaDaoZhanDetection)
	{
		if (EnemyAutoDetected)
		{
			EnemyAutoDetected = nullptr;
			for (TScriptInterface<IEnemyInterface> Enemy : EnemiesHighlightedThisFrame)
			{
				Enemy.GetInterface()->UnHightlightActor();
			}
			EnemiesHighlightedThisFrame.Empty();
		}
	}
	
	if (DetectionConfig.DetectionMode != EDetectionMode::CasualDetection)
	{
		if (EnemyLastFrame)
		{
			EnemyLastFrame->UnHightlightActor();
		}
		EnemyLastFrame = nullptr;
		EnemyThisFrame = nullptr;
	}
}

void ABaDaoZhanPlayerController::BaDaoZhanDetection(FVector Direction, FVector RootLocation, bool bOverride)
{
	
	// Init
	double GreatestForwardDist = 0;
	EnemyAutoDetected = nullptr;
	
	if (ViewMode == EViewMode::TP)
	{
		FVector CamForward = PlayerCameraManager->GetCameraRotation().Vector();
		(CamForward = FVector(CamForward.X, CamForward.Y, 0)).Normalize();
		Direction = CamForward;
		RootLocation = PlayerCameraManager->GetCameraLocation();
	}
	if (ViewMode == EViewMode::TD)
	{
		Direction = (CursorLocation - PawnLocation).GetSafeNormal();
		RootLocation = PawnLocation;
	}
	
	// UE_LOG(LogTemp, Log, TEXT("Direction: %s"), *Direction.ToString());
	// 配置扇形角度（例如，90 度扇形，半角 45 度）
	float HalfAngleRadians = FMath::Asin(DetectionConfig.DetectionRadius/DetectionConfig.Length);
	
	// 在for each loop 中循环遍历 Enemy List
	for(TScriptInterface<IEnemyInterface> Enemy : EnemyList)
	{
		// 检测 Enemy 是否有 AutoDetect GameplayTag。没有则什么都不干。
		if (Enemy.GetInterface()->GetEnemyAbilitySystemComponent()->HasMatchingGameplayTag(ImmuneToBaDaoZhanDetection)){return;}
	
		FVector EnemyLocation = Enemy->GetLocation();
		EnemyLocation = FVector(EnemyLocation.X,EnemyLocation.Y,RootLocation.Z);
		
		// 扇形区域检测
		float Distance = FVector::DistXY(EnemyLocation, RootLocation);
		FVector EnemyDirection = (EnemyLocation - RootLocation).GetSafeNormal();
		float CosAngle = FVector::DotProduct(EnemyDirection, Direction);
		float Angle = FMath::Acos(CosAngle);
		bool bInFan = Distance <= DetectionConfig.Length+(PawnLocation-RootLocation).Length()/2 
		&& Angle <= HalfAngleRadians;
		
		
		FVector Local = EnemyLocation - RootLocation;
		float ForwardDist = FVector::DotProduct(Local, Direction);
		bool bInRec = false;
		if (ViewMode == EViewMode::TD)
		{
			// 矩形区域检测
			float SideDist = FVector::CrossProduct(Direction, Local).Size();
			bInRec = ForwardDist >= 0 && ForwardDist <= RecLength && SideDist <= DetectionConfig.DetectionRadius;
		}
		
		
		// 若返回真：则高亮显示 Enemy，并检查 Enemy 是否离角色最远，若是则更新 EnemyDetected 变量。
		// 若返回否：消除 Enemy 高亮显示
		// Factor 是敌人离角色距离的线性spectrum，0则重叠，1则距离等于Length
		if (bInFan || bInRec)
		{
			const UBaDaoZhanAttributeSet* AttrSet = ASC->GetSet<UBaDaoZhanAttributeSet>();
			if (AttrSet && AttrSet->GetStamina() < 5.f){return;}
			Enemy->HighlightActor(2);
			EnemiesHighlightedThisFrame.Add(Enemy);
			if (ForwardDist >= GreatestForwardDist)
			{
				EnemyAutoDetected = Enemy.GetInterface();
				GreatestForwardDist = ForwardDist;
			}
		}
		else
		{
			Enemy->UnHightlightActor();
		}
	}
	
	if (bDrawDebugDetectionShape)
	{
		// todo: 不要hard-coded
		RootLocation.Z = 120;
		// Debug Draw
		FVector UpAxis = FVector::UpVector; // 如果你的项目是Z轴朝上
	
		// 配置参数
		int32 SegmentCount = 20; // 弧形线段数，越多越圆滑
		float AngleStep = HalfAngleRadians * 2 / SegmentCount;
	
		for (int32 i = 0; i < SegmentCount; ++i)
		{
			float CurrentAngle = -HalfAngleRadians + AngleStep * i;
			float NextAngle = CurrentAngle + AngleStep;

			FVector CurrentDir = Direction.RotateAngleAxis(FMath::RadiansToDegrees(CurrentAngle), FVector::UpVector);
			FVector NextDir = Direction.RotateAngleAxis(FMath::RadiansToDegrees(NextAngle), FVector::UpVector);

			FVector StartPoint = RootLocation + CurrentDir * (DetectionConfig.Length+(PawnLocation-RootLocation).Length()/2);
			FVector EndPoint = RootLocation + NextDir * (DetectionConfig.Length+(PawnLocation-RootLocation).Length()/2);

			DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Green, false, 0.1f);
		}
	
		// 两条边（从中心射出的）
		FVector LeftDir = Direction.RotateAngleAxis(FMath::RadiansToDegrees(HalfAngleRadians), FVector::UpVector);
		FVector RightDir = Direction.RotateAngleAxis(-FMath::RadiansToDegrees(HalfAngleRadians), FVector::UpVector);

		DrawDebugLine(GetWorld(), RootLocation, RootLocation + LeftDir * (DetectionConfig.Length+(PawnLocation-RootLocation).Length()/2), FColor::Green, false, 0.1f);
		DrawDebugLine(GetWorld(), RootLocation, RootLocation + RightDir * (DetectionConfig.Length+(PawnLocation-RootLocation).Length()/2), FColor::Green, false, 0.1f);
	
	
		FVector Forward = Direction * RecLength;
		FVector Right = FVector::CrossProduct(UpAxis, Direction).GetSafeNormal();
		FVector Offset = Right * DetectionConfig.DetectionRadius;
	
		// 4个顶点
		FVector P0 = RootLocation - Offset;
		FVector P1 = RootLocation + Offset;
		FVector P2 = RootLocation + Offset + Forward;
		FVector P3 = RootLocation - Offset + Forward;
	
		// 绘制矩形边界
		DrawDebugLine(GetWorld(), P0, P1, FColor::Blue, false, 0.1f);
		DrawDebugLine(GetWorld(), P1, P2, FColor::Blue, false, 0.1f);
		DrawDebugLine(GetWorld(), P2, P3, FColor::Blue, false, 0.1f);
		DrawDebugLine(GetWorld(), P3, P0, FColor::Blue, false, 0.1f);	
	}
}

void ABaDaoZhanPlayerController::BaDaoZhanDetection_TP()
{
	// 获取当前摄像机的正前方向量
	FVector CamForward = PlayerCameraManager->GetCameraRotation().Vector();
	(CamForward = FVector(CamForward.X, CamForward.Y, 0)).Normalize();
	BaDaoZhanDetection(CamForward,PlayerCameraManager->GetCameraLocation(),true);
}



void ABaDaoZhanPlayerController::CasualDetection_TD()
{
	// Detection的预处理
	// 变量初始化
	EnemyLastFrame = EnemyThisFrame;
	EnemyThisFrame = nullptr;
	
	float MinimumDistance = DetectionConfig.DetectionRadius;
	for (TScriptInterface<IEnemyInterface> Enemy : EnemyList)
	{
		float ThisDistance = FVector::Dist( FVector(Enemy->GetLocation().X,Enemy->GetLocation().Y,CursorLocation.Z), CursorLocation) - Enemy->GetRadius();
		if (ThisDistance <= MinimumDistance)
		{
			EnemyThisFrame = Enemy.GetInterface();
			MinimumDistance = ThisDistance;
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::SanitizeFloat(ThisDistance));
		}
	}
	// 对 EnemyThisFrame 和 EnemyLastFrame 进行 highlight/unhighlight 操作
	if (EnemyThisFrame)
	{
		EnemyThisFrame->HighlightActor(1);
	}
	if (EnemyLastFrame && EnemyLastFrame != EnemyThisFrame)
	{
		EnemyLastFrame->UnHightlightActor();
	}

	if (bDrawDebugDetectionShape)
	{
		DrawDebugCircle(
		GetWorld(),
		CursorLocation,                  // 圆心
		DetectionConfig.DetectionRadius,          // 半径
		20,                              // 线段数，越大越圆滑
		FColor::Blue,                  // 颜色
		false,                           // 持久性（false = 一帧）
		0.1f,                            // 显示时间
		0,                               // 深度优先级
		1.0f,                            // 线宽
		FVector(1, 0, 0),                // Y轴方向 → 绕Z轴绘制（地面）
		FVector(0, 1, 0),                // X轴方向
		false                            // 是否在3D中使用法线旋转（一般设false）
	);
	}
}

// 查询函数，用于AbilityTask：TargetDataDetected
FDetectionResultData ABaDaoZhanPlayerController::GetDetectedData4BaDaoZhan()
{
	if (DetectionConfig.DetectionMode == EDetectionMode::BaDaoZhanDetection && EnemyAutoDetected)
	{
		if (Cast<IAbilitySystemInterface>(Cast<IAbilitySystemInterface>(EnemyAutoDetected))->GetAbilitySystemComponent()
			->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("BaDaoZhan.Enemy.BaDaoZhanUpwardSlashable"))))
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Scenario 1 : Upward");
			return FDetectionResultData(EnemyAutoDetected->GetLocation(),true);
		}

		const FVector TargetLocation = EnemyAutoDetected->GetLocation();
		const FVector ATargetLocation = FVector(TargetLocation.X,TargetLocation.Y,PawnLocation.Z);
		EnemyAutoDetected = nullptr;
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Scenario 2 : ToEnemySlash");
		return FDetectionResultData(ATargetLocation,true);
	}

	if (ASC->HasMatchingGameplayTag(TPTag))
	{
		FVector TargetLocation = PlayerCameraManager->GetCameraLocation()+
			PlayerCameraManager->GetCameraRotation().Vector()*DetectionConfig.Length;
		return FDetectionResultData(FVector(TargetLocation.X,TargetLocation.Y,PawnLocation.Z),false);
	}
	
	if (ASC->HasMatchingGameplayTag(TDTag))
	{
		if ((CursorLocation - PawnLocation).Length() <= DetectionConfig.Length)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Scenario 3 : ToMouseSlash");
			return FDetectionResultData(CursorLocation,false);
		}
		else
		{
			FVector TargetLocation = PawnLocation + (CursorLocation-PawnLocation).GetSafeNormal() *DetectionConfig.Length;
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Scenario 4 : ToLocalSlash");
			return FDetectionResultData(FVector(TargetLocation.X,TargetLocation.Y,PawnLocation.Z),false);
		}
	}
	
	else
	{
		return FDetectionResultData(PawnLocation,false);
	}
}