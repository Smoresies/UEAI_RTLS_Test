// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidEnemyController.h"
#include "Kismet/GameplayStatics.h"

// static declarations
TArray<ABoidEnemyController*> ABoidEnemyController::boidList;
int ABoidEnemyController::maxSeen = 0;

ABoidEnemyController::ABoidEnemyController()
{
  PrimaryActorTick.bCanEverTick = true;
}

void ABoidEnemyController::BoidMovement()
{
  int numBoids = ABoidEnemyController::boidList.Num();
  int xpos_avg = 0, ypos_avg = 0, xvel_avg = 0, yvel_avg = 0,
    neighboring_boids = 0, close_dx = 0, close_dy = 0;

  if (numBoids > ABoidEnemyController::maxSeen)
    ABoidEnemyController::maxSeen = numBoids;

  if (numBoids <= ABoidEnemyController::maxSeen / 2)
    return; // This will eventually be the fleeing

  for (ABoidEnemyController* pBoid : ABoidEnemyController::boidList)
  {
    // Don't check against ourselves.
    if (pBoid == this)
      continue;

    // Find distance to all boids.
    float dx = this->GetPawn()->GetActorLocation().X - pBoid->GetPawn()->GetActorLocation().X;
    float dy = this->GetPawn()->GetActorLocation().Y - pBoid->GetPawn()->GetActorLocation().Y;

    if (fabs(dx) < visualRange && fabs(dy) < visualRange)
    {
      double squared_distance = dx * dx + dy * dy;

      // Check to see if any are TOO close (for Avoidance)
      if (squared_distance < protectedRange * protectedRange)
      {
        close_dx += dx;
        close_dy += dy;
      }
      // Check for all boids within visual range, to get their position (for Cohesion) and velocity (for Alignment)
      else if (squared_distance < visualRange * visualRange)
      {
        xpos_avg += pBoid->GetPawn()->GetActorLocation().X;
        ypos_avg += pBoid->GetPawn()->GetActorLocation().Y;
        xvel_avg += pBoid->GetPawn()->GetVelocity().X;
        yvel_avg += pBoid->GetPawn()->GetVelocity().Y;

        // Increment number of boids within visual range
        neighboring_boids++;
      }

    }
  }

  float bX;
  float bY;

  if (neighboring_boids > 0)
  {
    xpos_avg = xpos_avg / neighboring_boids;
    ypos_avg = ypos_avg / neighboring_boids;
    xvel_avg = xvel_avg / neighboring_boids;
    yvel_avg = yvel_avg / neighboring_boids;

    bX = (this->GetPawn()->GetVelocity().X +
      (xpos_avg - this->GetPawn()->GetActorLocation().X) * centeringValue +
      (xvel_avg - this->GetPawn()->GetVelocity().X) * matchingFactor);

    bY = (this->GetPawn()->GetVelocity().Y +
      (ypos_avg - this->GetPawn()->GetActorLocation().X) * centeringValue +
      (yvel_avg - this->GetPawn()->GetVelocity().Y) * matchingFactor);
  }

  bX += close_dx * avoidValue;
  bY += close_dy * avoidValue;


}

void ABoidEnemyController::BeginPlay()
{
  Super::BeginPlay();
  ABoidEnemyController::boidList.Add(this);
}

void ABoidEnemyController::Tick(float DeltaSeconds)
{
  Super::Tick(DeltaSeconds);
}

void ABoidEnemyController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
  ABoidEnemyController::boidList.Remove(this);
}