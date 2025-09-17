// Fill out your copyright notice in the Description page of Project Settings.


#include "Spine_EntityBase.h"
#include "Math/UnrealMathUtility.h"




// Sets default values
ASpine_EntityBase::ASpine_EntityBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpine_EntityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpine_EntityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 목적지가 있을 때만 동작
	if (destNode != nullptr) {
		FVector curPos = GetActorLocation();
		FVector destPos = destNode->GetActorLocation();
		destPos.X = destPos.X + PADDING_X;
		destPos.Z = curPos.Z;

		// 좌우 방향
		if (destPos.X > curPos.X) {
			SetActorScale3D(FVector(-1.f, 1.f, 1.f));
		}
		else if (destPos.X < curPos.X) {
			SetActorScale3D(FVector(1.f, 1.f, 1.f));
		}

		// 이동
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
