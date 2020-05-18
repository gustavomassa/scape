// Gustavo Massaneiro @ 2020

#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;
	//CloseAngle = InitialYaw;
	LastOpened = 0.0f;
	LastClosed = 0.0f;

	if (!ensure(TriggerVolume))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Trigger Volume is missing!"), *GetOwner()->GetName());
	}

	// Get the default actor based on the World top-down
	auto DefaultPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(DefaultPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Default Pawn!"), *GetOwner()->GetName());
	}

	// If TriggerActor is not defined, use Default Pawn
	if (!TriggerActor)
	{
		TriggerActor = DefaultPawn;
		UE_LOG(LogTemp, Warning, TEXT("%s: Using Default Pawn as Trigger Actor!"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// TODO: Use Event Driven instead of Pooling
	if (ensure(TriggerVolume) && ensure(TriggerActor) && TriggerVolume->IsOverlappingActor(TriggerActor))
	{
		if ((GetWorld()->GetTimeSeconds() - LastClosed) >= OpenDelay)
		{
			OpenDoor(DeltaTime);
			LastOpened = GetWorld()->GetTimeSeconds();
		}
	}
	else
	{
		if ((GetWorld()->GetTimeSeconds() - LastOpened) >= CloseDelay)
		{
			CloseDoor(DeltaTime);
			LastClosed = GetWorld()->GetTimeSeconds();
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// Exponential Linear Interpolation for a smooth movement
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, OpenSpeed);

	if (CurrentYaw > 0.0f && CurrentYaw < (OpenAngle - 0.1f))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	}

	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	// Exponential Linear Interpolation for a smooth movement
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, CloseSpeed);

	if (CurrentYaw < (InitialYaw - 0.1f))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	}

	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}
