// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NodeGraphData.generated.h"

// 노드 타입 정의
UENUM(BlueprintType)
enum class ENodeType : uint8
{
	Normal    UMETA(DisplayName = "Normal"),
	Lock      UMETA(DisplayName = "Lock"),
	Interact  UMETA(DisplayName = "Interact"),
	Event     UMETA(DisplayName = "Event"),
	Exit      UMETA(DisplayName = "Exit")
};

// 노드 연결 엣지
USTRUCT(BlueprintType)
struct FNodeEdge
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FromId = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ToId = -1;
};

// 개별 노드 정보
USTRUCT(BlueprintType)
struct FNodeDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENodeType Type = ENodeType::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLocked = false;

	// 노드의 월드 위치 (레벨 시각화용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WorldPos = FVector::ZeroVector;

	// 이웃 노드 (엣지에서 자동 생성 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Neighbors;
};

// 몬스터 순찰 경로 정의
USTRUCT(BlueprintType)
struct FMonsterPatrol
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StartNodeId = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> RouteNodeIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLoop = true;
};

// 그래프 전체 정의
UCLASS(BlueprintType)
class PROJECT_API UNodeGraphData : public UDataAsset
{
	GENERATED_BODY()

public:
	// 노드 리스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	TArray<FNodeDef> Nodes;

	// 연결 엣지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	TArray<FNodeEdge> Edges;

	// 몬스터 순찰
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	TArray<FMonsterPatrol> Monsters;

	// 시작 노드와 초기 턴 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	int32 StartNodeId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	int32 StartTurns = 10;

	// (선택) 에디터 버튼으로 Neighbors 자동 생성
	UFUNCTION(CallInEditor, Category = "Build")
	void BuildNeighborsFromEdges();
};
