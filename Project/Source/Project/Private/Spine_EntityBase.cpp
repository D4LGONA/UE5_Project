// Fill out your copyright notice in the Description page of Project Settings.


#include "Spine_EntityBase.h"

// Sets default values
ASpine_EntityBase::ASpine_EntityBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpine_EntityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpine_EntityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

