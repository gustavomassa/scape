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

	// If TriggerActorList is not provided, allow any actor to trigger the volume
	if (TriggerActorList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Trigger Actor List is empty, allowing any Actors as Triggers!"), *GetOwner()->GetName());
		bAllowAnyOverlappingActors = true;
	}

	FindAudioComponent();

	/* 	// Get the default actor based on the World top-down
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
	} */
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// TODO: Use Event Driven instead of Pooling
	float TotalMass{0.0f};
	bool isOverlapping = hasOverlappingActors(TotalMass);
	if (isOverlapping && TotalMass >= TriggerMass)
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

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!ensure(AudioComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to get Audio Component!"), *GetOwner()->GetName());
	}
}

bool UOpenDoor::hasOverlappingActors(float &out_TotalMass) const
{
	if (!TriggerVolume)
	{
		out_TotalMass = 0.0f;
		return false;
	}

	//TODO: Implement Class Filter
	TArray<AActor *> OverlapingActors;
	TArray<AActor *> AllowedActors;
	TriggerVolume->GetOverlappingActors(OverlapingActors);
	if (OverlapingActors.Num() == 0)
	{
		out_TotalMass = 0.0f;
		return false;
	}

	for (auto Actor : OverlapingActors)
	{
		bool bAllowedActor = TriggerActorList.Contains(Actor);
		if (bAllowedActor)
		{
			AllowedActors.Add(Actor);
		}
		float ActorMass = Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

		UE_LOG(LogTemp, Warning, TEXT("%s - Allowed: %s, is overlapping door trigger with mass: %f"), *Actor->GetName(), (bAllowedActor ? TEXT("True") : TEXT("False")), ActorMass);

		if (bAllowAnyOverlappingActors || bAllowedActor)
		{
			// Sum mas
			out_TotalMass += ActorMass;
		}
	}

	// Even though the Trigger Mass to open the door could be 0, we need to check if we have at least one allowed actor to open it
	if (!bAllowAnyOverlappingActors && AllowedActors.Num() == 0)
	{
		out_TotalMass = 0.0f;
		return false;
	}

	return true;
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	if (CurrentYaw >= (OpenAngle - 0.1f))
	{
		// Dor is opening/opened
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("%s Opening Door"), *GetOwner()->GetName());

	// Exponential Linear Interpolation for a smooth movement
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, OpenSpeed);

	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	bCloseDoorSound = false;
	if (!bOpenDoorSound)
	{
		PlaySound();
		bOpenDoorSound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	if (CurrentYaw <= 0.1f)
	{
		// Door is closing/closed
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s Closing Door"), *GetOwner()->GetName());

	// Exponential Linear Interpolation for a smooth movement
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, CloseSpeed);

	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	bOpenDoorSound = false;
	if (!bCloseDoorSound)
	{
		PlaySound();
		bCloseDoorSound = true;
	}
}

void UOpenDoor::PlaySound()
{
	if (AudioComponent)
		AudioComponent->Play();
}
