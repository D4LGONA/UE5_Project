// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapNode.h"
#include "MapLink.generated.h"

UCLASS()
class PROJECT_API AMapLink : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapLink();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 링크의 양 끝 노드
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Link")
	AMapNode* StartNode = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Link")
	AMapNode* EndNode = nullptr;

	UFUNCTION(BlueprintNativeEvent)
	void SetupLink();
	virtual void SetupLink_Implementation();

};
