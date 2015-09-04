// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "OrbitBody.generated.h"

UCLASS()
class INTOORBIT_API AOrbitBody : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbitBody();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	float RunningTime = 0.0f;

	UPROPERTY(EditAnywhere)	AOrbitBody *RootObject = this;

	UPROPERTY(EditAnywhere) float Density = 1.0f;

	UPROPERTY(EditAnywhere) float Distance = 0.f;

	UPROPERTY(EditAnywhere)	FVector4 InitalDeltaV;

	FVector4 DeltaV;

	float BoundsMass;

	float initDistance = 0.0f;
	
};
