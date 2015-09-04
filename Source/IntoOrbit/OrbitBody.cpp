// Fill out your copyright notice in the Description page of Project Settings.

#include "IntoOrbit.h"
#include "OrbitBody.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

// Sets default values
AOrbitBody::AOrbitBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Called when the game starts or when spawned
void AOrbitBody::BeginPlay()
{
	Super::BeginPlay();
	
	/// Calculate mass of object
	FVector origin, bounds;
	GetActorBounds(true, origin, bounds);
	BoundsMass = Density * bounds.Size();

	print(FString::Printf(TEXT("Actor %s mass is %f"), *GetActorLabel(), BoundsMass));

	/// Set up Orbit vars
	initDistance = FVector::Dist(RootObject->GetActorLocation(), this->GetActorLocation());
	DeltaV = InitalDeltaV;
	
}

// Called every frame
void AOrbitBody::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	RunningTime += DeltaTime;

	//
	FVector RLocation = RootObject->GetActorLocation();
	FVector TLocation = this->GetActorLocation();

	// If is orbit model root;  NOTE: Turned off in constructor?
	if (RootObject == this){ return; }

	// Get current distance from planitary body
	float rad = FVector::DistSquared(RLocation, TLocation);

	FVector PlanitaryForce = -(TLocation - RLocation);
	PlanitaryForce *= (RootObject->BoundsMass * BoundsMass * .0006111) / rad;

	DeltaV = PlanitaryForce + DeltaV * DeltaV.W;

	SetActorLocation(TLocation + DeltaV);

	//UE_LOG(TLog, Warning TEXT("Rad is: %f"), rad);
	//print(FString::Printf(TEXT("%s %f"), *this->GetName(), rad));

}

