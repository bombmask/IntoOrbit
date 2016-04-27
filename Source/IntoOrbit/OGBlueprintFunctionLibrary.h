// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "OrbitBody.h"

#include "OGBlueprintFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class INTOORBIT_API UOGBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Util")
	static TArray<AOrbitBody*> GetOrbitBodies();
	
	
};
