// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIController.h"
#include "BoidEnemyController.generated.h"

/**
 *
 */
UCLASS()
class UEAI_RTLS_TEST_API ABoidEnemyController : public AAIController
{
  GENERATED_BODY()

  static TArray<ABoidEnemyController*> boidList;
  static int maxSeen;

public:

  ABoidEnemyController();

  // Called every frame
  virtual void Tick(float DeltaSeconds) override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
  void BoidMovement();

  // How fast do they turn
  double turnFactor = 0.2;

  // Max distance we should  be from the nearest boid,
  // if nearest boid is further then we go straight to that.
  int visualRange = 200;

  // Closest we should be to any boid. If any are closer we prioritize leaving this range.
  int protectedRange = 75;

  // Tunable value for Cohesion, for greater cohesion increase value.
  double centeringValue = 0.0005; 

  // Tunable value for Separation. Can turn up to have less clumps
  double avoidValue = 0.05;

  // Tunable value for Alignment, how much we adjust velocity to go same direction as others.
  double matchingFactor = 0.05;

  // Uses this for max/min speed, adjust based on proximity to player.
  double maxSpeed = 400;
  double minSpeed = 200;

};
