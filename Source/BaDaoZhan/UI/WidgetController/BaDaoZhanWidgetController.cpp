// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanWidgetController.h"


UBaDaoZhanWidgetController::UBaDaoZhanWidgetController()
{
	
}

void UBaDaoZhanWidgetController::SetWidgetControllerParams(const FWidgetControllerParams WCParams)
{
	AttributeSet = WCParams.AttributeSet;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
}

void UBaDaoZhanWidgetController::BroadcastInitialValue()
{
	
}

void UBaDaoZhanWidgetController::BindCallBackToDependencies()
{
	
}
