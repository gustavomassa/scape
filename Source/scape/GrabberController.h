// Gustavo Massaneiro @ 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabberController.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SCAPE_API UGrabberController : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabberController();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void Grab();
	void Release();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool bGrabbed{false};
	struct FPlayerViewPoint
	{
		FVector Location{FVector::ZeroVector};
		FRotator Rotation{FRotator::ZeroRotator};
		FVector LineTraceEnd{FVector::ZeroVector};
	} PlayerViewPoint;

	//Required components
	UPROPERTY()
	UPhysicsHandleComponent *PhysicsHandle{nullptr};
	UPROPERTY()
	UInputComponent *InputComponent{nullptr};

	UPROPERTY(EditAnywhere)
	float ReachDistance{100.0f};

	UPROPERTY(EditAnywhere)
	FName BindName{"Grab"};

	void FindPhysicsHandle();
	void FindInputComponent();
	void RegisterBind();
	void UpdatePlayerViewPoint();
	bool IsPhysicsBodyInReach(FHitResult &out_HitResult, bool bDrawDebugLine, bool bDrawDebugLineOnHitOnly);
	void UpdateGrabbedComponentLocation();
	void ResetPlayerViewPoint();
};
