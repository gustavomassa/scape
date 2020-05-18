// Gustavo Massaneiro @ 2020

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	float DoorRange = 90.0f;

	FRotator CurrentTotation = GetOwner()->GetActorRotation();
	CurrentTotation.Yaw = DoorRange;

	UE_LOG(LogTemp, Warning, TEXT("Opening door %s"), *GetOwner()->GetName());

	GetOwner()->SetActorRotation(CurrentTotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
