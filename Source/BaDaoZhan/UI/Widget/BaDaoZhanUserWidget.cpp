// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanUserWidget.h"


void UBaDaoZhanUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController; 
	WidgetControllerSet();
}