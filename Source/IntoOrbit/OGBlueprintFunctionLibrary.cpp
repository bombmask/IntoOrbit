// Fill out your copyright notice in the Description page of Project Settings.

#include "IntoOrbit.h"
#include "OGBlueprintFunctionLibrary.h"

TArray<AOrbitBody*> UOGBlueprintFunctionLibrary::GetOrbitBodies()
{
	return AOrbitBody::GetOrbitBodiesStatic();
}
