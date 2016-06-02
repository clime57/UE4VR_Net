// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4VR_Net.h"
#include "ConfigActor.h"


// Sets default values
AConfigActor::AConfigActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AConfigActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConfigActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

