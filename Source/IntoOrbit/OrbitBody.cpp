// Fill out your copyright notice in the Description page of Project Settings.

#include "IntoOrbit.h"
#include "OrbitBody.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.0, FColor::White,text)
#define printl(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::White,text)

TArray<AOrbitBody*> AOrbitBody::EffectActors = TArray<AOrbitBody*>();

// Sets default values
AOrbitBody::AOrbitBody(const FObjectInitializer & OBJ) : Super(OBJ)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = true;
	Capsule = OBJ.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collision0"));
	MySphere = OBJ.CreateDefaultSubobject<UDestructibleComponent>(this, TEXT("Sphere0"));
	MySphere->AttachTo(Capsule);

	RootComponent = Capsule;
}

// Called when the game starts or when spawned
void AOrbitBody::BeginPlay()
{
	Super::BeginPlay();
	EffectActors.Add(this);
	
	/// Calculate mass of object
	FVector origin, bounds;
	GetActorBounds(true, origin, bounds);
	BoundsMass = Density * bounds.Size();

	print(FString::Printf(TEXT("Actor %s mass is %f"), *GetActorLabel(), BoundsMass));

	/// Set up Orbit vars
	//initDistance = FVector::Dist(RootObject->GetActorLocation(), this->GetActorLocation());
	DeltaV = InitalDeltaV;

	if (isRoot) { this->SetActorTickEnabled(false); }

	DeltaV = InitalDeltaV / 200.0f;
	
	
}

void AOrbitBody::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	EffectActors.Remove(this);
}

// Called every frame
void AOrbitBody::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	RunningTime += DeltaTime;

	// Get our world location
	FVector CLoc = GetActorLocation();

	//FVector DV(0.0);
	
	for (AOrbitBody *Body : GetOrbitBodies()) {
		if (Body == this) {
			continue;
		}

		if (!IsValid(Body)) {
			EffectActors.Remove(Body);
			continue;
		}
		float r = FVector::DistSquared(CLoc, Body->GetActorLocation());

		FVector PlanitaryForce = -(CLoc - Body->GetActorLocation());

		PlanitaryForce.Normalize();
		PlanitaryForce *= ((((Body)->BoundsMass * BoundsMass) * 0.06111) / r);

		DeltaV += PlanitaryForce;
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
	FHitResult* Out = new FHitResult();
	AddActorWorldOffset(DeltaV, true, Out);

	if (Out->bBlockingHit) {
		Out->GetComponent()->AddImpulse(DeltaV);
		MySphere->AddImpulse(-DeltaV);
	

	}
	delete Out;

	//print(FString::Printf(TEXT("%s %f %d ticked"), *this->GetName(), DeltaV.Size(), EffectActors.Num()));
	const float END = 40000;
	if (FMath::Abs(CLoc.X) > END || FMath::Abs(CLoc.Y) > END || FMath::Abs(CLoc.Z) > END) {
		Destroy();

	}
}

TArray<AOrbitBody*> AOrbitBody::GetOrbitBodies()
{
	return EffectActors;
}



