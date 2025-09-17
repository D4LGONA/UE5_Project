// 2D Spine Object Base Class

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapNode.h"
#include "Spine_EntityBase.generated.h"

#define PADDING_X 150.0f


UCLASS()
class PROJECT_API ASpine_EntityBase : public AActor
{
	GENERATED_BODY()
	
protected:
	// 현재 위치한 노드를 지정
	AMapNode* curNode = nullptr;
	AMapNode* destNode = nullptr;

public:	
	// Sets default values for this actor's properties
	ASpine_EntityBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetDest(AMapNode* node) { destNode = node; }

public:
};
