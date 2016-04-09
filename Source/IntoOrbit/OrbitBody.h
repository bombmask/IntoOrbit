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
	AOrbitBody(const FObjectInitializer & OBJ);
	
	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	float RunningTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", Meta = (ExposeOnSpawn = true)) 
		float Density = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true)) 
		bool ShowDebugLines = false;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", Meta = (MakeEditWidget = true, ExposeOnSpawn = true))
		FVector InitalDeltaV;

	UPROPERTY(Category = Comp, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USphereComponent* Capsule;

	UPROPERTY(Category = Comp, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UDestructibleComponent* MySphere;



	


	FVector DeltaV;

	float BoundsMass;

	float initDistance = 0.0f;

	UPROPERTY(EditAnywhere)
	bool isRoot = false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bomb")
protected:
	static TArray<AOrbitBody*> EffectActors; 

public:
	UFUNCTION(BlueprintCallable, Category = "Util")
	TArray<AOrbitBody*> GetOrbitBodies();

	
};
