// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spine_EntityBase.h"
#include "Enemy.generated.h"


UCLASS()
class PROJECT_API AEnemy : public ASpine_EntityBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	AMapNode* StartNode = nullptr;
	
	UPROPERTY()
	TArray<AMapNode*> AllowedNodes;  

	TArray<AMapNode*> GetNodesWithinDistance(AMapNode* Origin, int32 MaxDist);

public:
	UFUNCTION(BlueprintCallable)
	AMapNode* NextNode();

	UFUNCTION(BlueprintCallable)
	void InitEnemy(AMapNode* InStartNode);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
