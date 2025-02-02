// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ObjectMacros.h"

// Static Definitions
TArray<ABoidEnemy*> ABoidEnemy::boidList;
int ABoidEnemy::maxSeen = 0;
APawn* ABoidEnemy::player;

// Sets default values
ABoidEnemy::ABoidEnemy()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoidEnemy::BeginPlay()
{
  Super::BeginPlay();
  ABoidEnemy::boidList.Add(this);
  ABoidEnemy::player = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);
}


void ABoidEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
  ABoidEnemy::boidList.Remove(this);
}

// Called every frame
void ABoidEnemy::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  if (player != nullptr)
  {
    
    float dist = this->GetDistanceTo(player);
    // UE_LOG(LogTemp, Warning, TEXT("dist is %f"), dist);
    if (dist <= 2000)
    {
      minSpeed = aggressiveSpeed - 100;
      maxSpeed = aggressiveSpeed + 100;
      BoidMovement();
    }
    else if (dist <= 7000)
    {
      minSpeed = cautiousSpeed - 100;
      maxSpeed = cautiousSpeed + 100;
      BoidMovement();
    }
  }
  // if player is nullptr, try to get reference to the player.
  else
  {
    ABoidEnemy::player = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);
    UE_LOG(LogTemp, Warning, TEXT("Hello World 2!"));
  }
  
  
}


bool ABoidEnemy::FleeCheck()
{
  return ABoidEnemy::boidList.Num() <= ABoidEnemy::maxSeen / 2;
}

void ABoidEnemy::BoidMovement()
{
  int numBoids = ABoidEnemy::boidList.Num();
  int xpos_avg = 0, ypos_avg = 0, xvel_avg = 0, yvel_avg = 0,
    neighboring_boids = 0, close_dx = 0, close_dy = 0;
  ABoidEnemy* randBoid = nullptr;

  

  if (numBoids > ABoidEnemy::maxSeen)
    ABoidEnemy::maxSeen = numBoids;

  for (ABoidEnemy* pBoid : ABoidEnemy::boidList)
  {
    // Don't check against ourselves.
    if (pBoid == this)
      continue;

    randBoid = pBoid;

    // Find distance to all boids.
    float dx = this->GetActorLocation().X - pBoid->GetActorLocation().X;
    float dy = this->GetActorLocation().Y - pBoid->GetActorLocation().Y;

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
        xpos_avg += pBoid->GetActorLocation().X;
        UE_LOG(LogTemp, Warning, TEXT("%f"), xpos_avg);
        ypos_avg += pBoid->GetActorLocation().Y;
        xvel_avg += pBoid->pbX;
        yvel_avg += pBoid->pbY;

        // Increment number of boids within visual range
        neighboring_boids++;
      }

    }
  }

  // Goal X/Y velocity, given our current velocity/location and others around us.
  float bX = 0;
  float bY = 0;

  if (neighboring_boids > 0)
  {
    xpos_avg = xpos_avg / neighboring_boids;
    ypos_avg = ypos_avg / neighboring_boids;
    xvel_avg = xvel_avg / neighboring_boids;
    yvel_avg = yvel_avg / neighboring_boids;

    bX = (this->pbX +
      (xpos_avg - this->GetActorLocation().X) * centeringValue +
      (xvel_avg - this->pbX) * matchingFactor);

    bY = (this->pbY +
      (ypos_avg - this->GetActorLocation().Y) * centeringValue +
      (yvel_avg - this->pbY) * matchingFactor);
  }
  // If no other are in 200 and we aren't the last one.
  else if(randBoid != nullptr)
  {
    // Choose random boid to go towards if none are within 200
    bX = (this->pbX +
      (randBoid->GetActorLocation().X - this->GetActorLocation().X) * centeringValue +
      (randBoid->pbX - this->pbX) * matchingFactor);

    bY = (this->pbY +
      (randBoid->GetActorLocation().Y - this->GetActorLocation().Y) * centeringValue +
      (randBoid->GetActorLocation().Y - this->pbY) * matchingFactor);
  }

  bX += close_dx * avoidValue;
  bY += close_dy * avoidValue;

  float speed = sqrt(bX * bX + bY * bY);

  // Enforce min and max speeds
  
  if (speed == 0)
  {
    bX = minSpeed;
    bY = minSpeed;
  }
  else if (speed < minSpeed)
  {
    bX = (bX / speed) * minSpeed;
    bY = (bY / speed) * minSpeed;
  }
  else if (speed > maxSpeed)
  {
    bX = (bX / speed) * maxSpeed;
    bY = (bY / speed) * maxSpeed;

  }

  // Update boid's position
  // boid.x = boid.x + bX;
  // boid.y = boid.y + bY;
  this->AddMovementInput(FVector(bX, bY, 0));
  pbX = bX;
  pbY = bY;


}


