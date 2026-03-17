// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Components/WidgetComponent.h"

#include "ZWidgetComponent.generated.h"

UCLASS()
class COMMONUIZRUNTIME_API UZWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void K2_SetWidgetClass(TSubclassOf<UUserWidget> newWidgetClass);
	
};


