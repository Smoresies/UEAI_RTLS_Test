// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BoidEnemy.generated.h"

UCLASS()
class UEAI_RTLS_TEST_API ABoidEnemy : public ACharacter
{
  GENERATED_BODY()

  static TArray<ABoidEnemy*> boidList;
  static int maxSeen;

  static APawn* player;

  int pbX = 0;
  int pbY = 0;

public:
  // Sets default values for this character's properties
  ABoidEnemy();


protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  // How fast do they turn
  double turnFactor = 0.2;

  // Max distance we should  be from the nearest boid,
  // if nearest boid is further then we go straight to that.
  int visualRange = 200;

  // Closest we should be to any boid. If any are closer we prioritize leaving this range.
  int protectedRange = 75;

  // Tunable value for Cohesion, for greater cohesion increase value.
  double centeringValue = 0.005;

  // Tunable value for Separation. Can turn up to have less clumps
  double avoidValue = 0.01;

  // Tunable value for Alignment, how much we adjust velocity to go same direction as others.
  double matchingFactor = 0.10;

  // Uses this for max/min speed, adjust based on proximity to player.
  double maxSpeed = 6;
  double minSpeed = 3;

  const int cautiousSpeed = 400;
  const int aggressiveSpeed = 800;



public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable)
  void BoidMovement();

  UFUNCTION(BlueprintCallable)
  bool FleeCheck();

  // Called to bind functionality to input
  // virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
