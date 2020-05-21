// Gustavo Massaneiro @ 2020

#include "GrabberController.h"

// Sets default values for this component's properties
UGrabberController::UGrabberController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabberController::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	FindInputComponent();
	RegisterBind();
}

// Called every frame
void UGrabberController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateGrabbedComponentLocation();
}

void UGrabberController::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!ensure(PhysicsHandle))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Physics Handle Component!"), *GetOwner()->GetName());
	}
}

void UGrabberController::FindInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!ensure(InputComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Input Component!"), *GetOwner()->GetName());
	}
}

void UGrabberController::RegisterBind()
{
	InputComponent->BindAction(BindName, IE_Pressed, this, &UGrabberController::Grab);
	InputComponent->BindAction(BindName, IE_Released, this, &UGrabberController::Release);
}

void UGrabberController::UpdatePlayerViewPoint()
{
	// Get The First Player View Point
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPoint.Location, PlayerViewPoint.Rotation);
	// Calculate Line Trace End based on the Location and Rotation
	PlayerViewPoint.LineTraceEnd = PlayerViewPoint.Location + PlayerViewPoint.Rotation.Vector() * ReachDistance;
}

bool UGrabberController::IsPhysicsBodyInReach(FHitResult &out_HitResult, bool bDrawDebugLine = false, bool bDrawDebugLineOnHitOnly = false)
{
	UpdatePlayerViewPoint();

	// Collision Query
	FCollisionQueryParams TraceParams(
		NAME_None,
		false,
		GetOwner());

	// Ray-cast out to reach distance to find the first collided physics body
	bool bReached = GetWorld()->LineTraceSingleByObjectType(
		out_HitResult,
		PlayerViewPoint.Location,
		PlayerViewPoint.LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	if (bDrawDebugLine)
	{
		if (!bDrawDebugLineOnHitOnly || (bDrawDebugLineOnHitOnly && bReached))
		{
			DrawDebugLine(
				GetWorld(),
				PlayerViewPoint.Location,
				PlayerViewPoint.LineTraceEnd,
				FColor(0, 255, 0),
				true,
				0.0f,
				0,
				5.0f);
		}
	}

	return bReached;
}

void UGrabberController::UpdateGrabbedComponentLocation()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		// Update the location of the object that we are holding
		UpdatePlayerViewPoint();
		PhysicsHandle->SetTargetLocation(PlayerViewPoint.LineTraceEnd);
	}
}

void UGrabberController::Grab()
{
	// Make sure we are not holding any object before trying to brab anything
	if (!PhysicsHandle->GrabbedComponent)
	{
		FHitResult HitResult;
		if (IsPhysicsBodyInReach(HitResult))
		{
			AActor *ActorHit = HitResult.GetActor();
			if (ActorHit)
			{
				UE_LOG(LogTemp, Warning, TEXT("HIT %s"), *PlayerViewPoint.Location.ToString(), *HitResult.GetActor()->GetName());

				PhysicsHandle->GrabComponentAtLocationWithRotation(
					HitResult.GetComponent(),
					NAME_None,
					PlayerViewPoint.LineTraceEnd,
					PlayerViewPoint.Rotation);

				bGrabbed = true;
				UE_LOG(LogTemp, Warning, TEXT("Grabbed object"));
			}
		}
	}
}

void UGrabberController::Release()
{
	// Protection to be able to release the bind after an object is just grabbed
	if (bGrabbed)
	{
		bGrabbed = false;
		return;
	}

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
		ResetPlayerViewPoint();
		UE_LOG(LogTemp, Warning, TEXT("Released object"));
	}
}

void UGrabberController::ResetPlayerViewPoint()
{
	PlayerViewPoint.Location = FVector::ZeroVector;
	PlayerViewPoint.Rotation = FRotator::ZeroRotator;
	PlayerViewPoint.LineTraceEnd = FVector::ZeroVector;
}
