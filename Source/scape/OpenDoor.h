// Gustavo Massaneiro @ 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "OpenDoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float InitialYaw{0.0f};
	float CurrentYaw{0.0f};
	float LastOpened{0.0f};
	float LastClosed{0.0f};
	bool bAllowAnyOverlappingActors{false};

	UPROPERTY(EditAnywhere)
	float OpenAngle{90.0f};

	UPROPERTY(EditAnywhere)
	float OpenSpeed{1.5f};

	UPROPERTY(EditAnywhere)
	float OpenDelay{0.0f};

	//UPROPERTY(EditAnywhere)
	//float CloseAngle = 0.0f;

	UPROPERTY(EditAnywhere)
	float CloseSpeed{1.5f};

	UPROPERTY(EditAnywhere)
	float CloseDelay{0.0f};

	UPROPERTY(EditAnywhere)
	ATriggerVolume *TriggerVolume{nullptr};

	UPROPERTY(EditAnywhere)
	float TriggerMass{0.0f};

	UPROPERTY(EditAnywhere)
	TArray<AActor *> TriggerActorList;

	bool hasOverlappingActors(float &out_TotalMass) const;
};
