// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bonfire.generated.h"


USTRUCT(BlueprintType)
struct FBonfireInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector PlayerSpawnPosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator PlayerSpawnRotation;
};

UCLASS()
class DEUS_EX_ASH_API ABonfire : public AActor
{
	GENERATED_BODY()
	
public:	
	ABonfire();

protected:
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;
};
