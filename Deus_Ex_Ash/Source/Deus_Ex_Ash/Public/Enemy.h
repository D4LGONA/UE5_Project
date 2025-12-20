// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spine_EntityBase.h"
#include "Enemy.generated.h"


// 선택페이즈 -> 액션페이즈 -> 승리/패배
UENUM(BlueprintType)
enum class EAtkType : uint8 { Tutorial, Phase1, Phase2 };

UENUM(BlueprintType)
enum class EEnemyActionType : uint8
{
	Stay,
	Random
};

UCLASS()
class DEUS_EX_ASH_API AEnemy : public ASpine_EntityBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	AMapNode* StartNode = nullptr;
	
	UPROPERTY()
	TArray<AMapNode*> AllowedNodes;  
	TArray<AMapNode*> GetNodesWithinDistance(AMapNode* Origin, int32 MaxDist);

	UPROPERTY(EditAnywhere)
	EAtkType AtkType = EAtkType::Tutorial; // 이 적의 공격 타입

	UPROPERTY(EditAnywhere)
	EEnemyActionType MoveType = EEnemyActionType::Stay;

public:
	UFUNCTION(BlueprintCallable)
	AMapNode* NextNode();
	AMapNode* Random();
	AMapNode* Stay();

	UFUNCTION(BlueprintCallable)
	void InitEnemy(AMapNode* InStartNode);

	UFUNCTION(BlueprintCallable)
	EAtkType GetAtkType() const { return AtkType; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
