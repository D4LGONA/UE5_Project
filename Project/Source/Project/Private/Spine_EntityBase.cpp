// Fill out your copyright notice in the Description page of Project Settings.


#include "Spine_EntityBase.h"
#include "Math/UnrealMathUtility.h"

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

void ASpine_EntityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (destNode != nullptr) {
		FVector curPos = GetActorLocation();
		FVector destPos = destNode->GetActorLocation();
		destPos.X = destPos.X + 150.0f;
		destPos.Z = curPos.Z;

		// ===== �¿� ���� �Ǻ� =====
		if (destPos.X > curPos.X) {
			SetActorScale3D(FVector(-1.f, 1.f, 1.f));
		}
		else if (destPos.X < curPos.X) {
			SetActorScale3D(FVector(1.f, 1.f, 1.f));
		}

		// ===== �̵� ó�� =====
		FVector newPos = FMath::VInterpConstantTo(curPos, destPos, DeltaTime, 500.0f);
		SetActorLocation(newPos);

		float distance = FVector::Dist(newPos, destPos);
		if (distance < 10.0f) {
			SetActorLocation(destPos);
			curNode = destNode;
			destNode = nullptr;
		}
	}
}
