// Gustavo Massaneiro @ 2020

#include "GrabberController.h"

// Sets default values for this component's properties
UGrabberController::UGrabberController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabberController::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UGrabberController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerLocation;
	FRotator PlayerRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotator);

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerLocation.ToString(), *PlayerRotator.ToString());

	// Calculate Line Trace End, 100.0 = 1 meter
	FVector LineTraceEnd = PlayerLocation + PlayerRotator.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerLocation, LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.0f,
		0,
		5.0f);

	// Ray-cast out to a cartain distance (Reach)
	FCollisionQueryParams TraceParams(
		FName(TEXT("")),
		false,
		GetOwner());
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByObjectType(
			HitResult,
			PlayerLocation,
			LineTraceEnd,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
			TraceParams))
	{
		AActor *ActorHit = HitResult.GetActor();
		if (ActorHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("HIT %s"), *PlayerLocation.ToString(), *HitResult.GetActor()->GetName());
		}
	}
	// See what its hits
}
