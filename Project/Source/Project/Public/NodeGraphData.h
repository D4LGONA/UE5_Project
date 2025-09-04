// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NodeGraphData.generated.h"

// ��� Ÿ�� ����
UENUM(BlueprintType)
enum class ENodeType : uint8
{
	Normal    UMETA(DisplayName = "Normal"),
	Lock      UMETA(DisplayName = "Lock"),
	Interact  UMETA(DisplayName = "Interact"),
	Event     UMETA(DisplayName = "Event"),
	Exit      UMETA(DisplayName = "Exit")
};

// ��� ���� ����
USTRUCT(BlueprintType)
struct FNodeEdge
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FromId = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ToId = -1;
};

// ���� ��� ����
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

	// ����� ���� ��ġ (���� �ð�ȭ��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WorldPos = FVector::ZeroVector;

	// �̿� ��� (�������� �ڵ� ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Neighbors;
};

// ���� ���� ��� ����
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

// �׷��� ��ü ����
UCLASS(BlueprintType)
class PROJECT_API UNodeGraphData : public UDataAsset
{
	GENERATED_BODY()

public:
	// ��� ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	TArray<FNodeDef> Nodes;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	TArray<FNodeEdge> Edges;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	TArray<FMonsterPatrol> Monsters;

	// ���� ���� �ʱ� �� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	int32 StartNodeId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	int32 StartTurns = 10;

	// (����) ������ ��ư���� Neighbors �ڵ� ����
	UFUNCTION(CallInEditor, Category = "Build")
	void BuildNeighborsFromEdges();
};
