// Fill out your copyright notice in the Description page of Project Settings.

#include "IntoOrbit.h"
#include "OrbitBody.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.0, FColor::White,text)

// Sets default values
AOrbitBody::AOrbitBody(const FObjectInitializer & OBJ) : Super(OBJ)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = true;

	auto MySphere = OBJ.CreateOptionalDefaultSubobject<UStaticMeshComponent>(this, TEXT("Sphere"));
	/*MySphere->*/
	RootComponent = MySphere;
	
	
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
	//initDistance = FVector::Dist(RootObject->GetActorLocation(), this->GetActorLocation());
	DeltaV = InitalDeltaV;

	if (isRoot) { this->SetActorTickEnabled(false); }

	TSubclassOf<AOrbitBody> ClassToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, EffectActors);

	EffectActors.Remove(this);
	


}

// Called every frame
void AOrbitBody::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	RunningTime += DeltaTime;

	// Get our world location
	FVector CLoc = GetActorLocation();

	FVector DV(0.0);

	for (auto *Body : EffectActors) {
		float r = FVector::DistSquared(CLoc, Body->GetActorLocation());
		FVector PlanitaryForce = (CLoc - Body->GetActorLocation());
		PlanitaryForce.Normalize();
		PlanitaryForce *= (((Cast<AOrbitBody>(Body)->BoundsMass * BoundsMass) * 0.0006111) / r);
		DV += PlanitaryForce;
	}

	//// Get current Locations of reference objects
	//FVector RLocation = RootObject->GetActorLocation();
	//FVector TLocation = this->GetActorLocation();


	//// Get current distance from planitary body, Bla bla bla n^2 + n1^2 of x y z
	//float rad = FVector::DistSquared(RLocation, TLocation);
	//
	//// Calculate force direction vector normalized
	//// EDIT: Should be normalized and then multiplied by newtons law? 
	//// Gravitational contant is scaled by the size of the world, calc for scale?
	//FVector PlanitaryForce = -(TLocation - RLocation);
	//// Newtons law applied, inverse square, universal gravitation
	//PlanitaryForce *= ((RootObject->BoundsMass * BoundsMass) * .0006111) / rad;
	//
	//// Calc movement vector, Addition of vectors handels magnitude bias
	//DeltaV = PlanitaryForce + DeltaV * DeltaV.W;

	AddActorWorldOffset(DV);

	print(FString::Printf(TEXT("%s ticked"), *this->GetName()));
}
