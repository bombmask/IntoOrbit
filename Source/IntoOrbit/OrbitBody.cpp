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
	//Capsule = OBJ.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collision0"));
	MySphere = OBJ.CreateDefaultSubobject<UDestructibleComponent>(this, TEXT("Sphere0"));
	//MySphere->AttachTo(Capsule);

	RootComponent = MySphere;
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

	//print(FString::Printf(TEXT("Actor %s mass is %f"), *GetActorLabel(), BoundsMass));

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
		// I believe R^2 in this case may be 0
		// Also, We don't need to have force on ourself
		if (Body == this) {
			continue;
		}
		
		// Checking from old code.
		// I used to do a call for get all actors of class but now 
		// we just add ourselves to the static array in the class.
		// this avoids any issues if a body is about to leave the
		// bounds zone and is scheduled for destruction
		if (!IsValid(Body)) {
			EffectActors.Remove(Body);
			continue;
		}
		
		// We only need R^2, therefore DistSquared is less expensive
		// and only does what we need. This is used for force calcuation
		// The inverse square law
		float r = FVector::DistSquared(CLoc, Body->GetActorLocation());

		// Get vector force from this body to their body. 
		// I feel like there may need to be a mass test here.
		// because earth moves to moons instead of the oposite
		FVector PlanitaryForce = -(CLoc - Body->GetActorLocation());

		PlanitaryForce.Normalize();
		
		// Force = G(constant) * (M1 * M2 / r^2)
		PlanitaryForce *= ((Body->BoundsMass * BoundsMass) / r)* 0.0006111;
		
		// Accumulate Current delta velocity
		DeltaV += PlanitaryForce;
	}
	// These are DEBUG lines
	// No reason to do a dumb if check if draw debug will fail anyway
	#ifndef UE_BUILD_SHIPPING
	if (ShowDebugLines == true)
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + DeltaV * 100, FColor::Green, false, 0.0);
	}
	#end


	//// Get current Locations of reference objects
	//FVector RLocation = RootObject->GetActorLocation();
	//FVector TLocation = this->GetActorLocation();


	//// Get current distance from planitary body, Bla bla bla n^2 + n1^2 of x y z
	//// Gravitational contant is scaled by the size of the world, calc for scale?
	//// Newtons law applied, inverse square, universal gravitation

	FHitResult* Out = new FHitResult();
	AddActorWorldOffset(DeltaV, true, Out);

	if (Out->bBlockingHit) {
		Out->GetComponent()->AddImpulse(DeltaV);
		MySphere->AddImpulse(-DeltaV);
	

	}
	// delete what you new
	delete Out;
	
	const float END = 40000;
	if (FMath::Abs(CLoc.X) > END || FMath::Abs(CLoc.Y) > END || FMath::Abs(CLoc.Z) > END) {
		Destroy();

	}
}

// This is a super cool accessor function to access static variables
// for this class in unreal so that I'm not calling "get all actors of class"
// Every single frame
// Also actors Add and Delete from this Var on Begin and End Play
TArray<AOrbitBody*> AOrbitBody::GetOrbitBodies()
{
	return EffectActors;
}



