// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameInstance_2D.h"

// todo: 다양한 이동 로직 만들기 -> 플레이어 쫓아가기, 반복 패턴
// todo: 


// 이동할 노드 찾기
AMapNode* AEnemy::NextNode()
{
    if (!curNode) return nullptr;

    const TArray<AMapNode*>& Neighbors = curNode->GetNeighbors();

    TArray<AMapNode*> ValidNodes;
    for (AMapNode* Node : Neighbors)
    {
        if (AllowedNodes.Contains(Node))
        {
            ValidNodes.Add(Node);
        }
    }

    if (ValidNodes.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, ValidNodes.Num() - 1);
        return ValidNodes[Index];
    }

    return nullptr;
}

// 스폰때 호출
void AEnemy::InitEnemy(AMapNode* InStartNode)
{
    StartNode = InStartNode;
    curNode = StartNode;
    AllowedNodes = GetNodesWithinDistance(StartNode, 2);
    // 값을 세팅하는 부분
    Stat.MaxHP = 150;
	Stat.HP = Stat.MaxHP;
    Stat.ATK = 40;
    Stat.DEF = false;
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(StartNode))
    {
        FVector BaseLoc = StartNode->GetActorLocation();

        FVector NewLoc = BaseLoc;
        NewLoc.X += PADDING_X;
        NewLoc.Z = PADDING_Z;

        SetActorLocation(NewLoc);

        AllowedNodes = { StartNode };
    }

    if (auto* GI = GetGameInstance<UGameInstance_2D>())
    {
        GI->RegisterEnemy(this);
    }

    InitEnemy(StartNode);
}

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (auto* GI = GetGameInstance<UGameInstance_2D>())
    {
        GI->UnregisterEnemy(this);
    }
    Super::EndPlay(EndPlayReason);
}

TArray<AMapNode*> AEnemy::GetNodesWithinDistance(AMapNode* Origin, int32 MaxDist)
{
    TArray<AMapNode*> Result;
    if (!Origin) return Result;

    TQueue<TPair<AMapNode*, int32>> Queue;
    TSet<AMapNode*> Visited;

    Queue.Enqueue(TPair<AMapNode*, int32>(Origin, 0));
    Visited.Add(Origin);

    while (!Queue.IsEmpty())
    {
        TPair<AMapNode*, int32> Pair;
        Queue.Dequeue(Pair);

        AMapNode* Node = Pair.Key;
        int32 Dist = Pair.Value;

        if (Dist <= MaxDist)
        {
            Result.Add(Node);

            for (AMapNode* Neighbor : Node->GetNeighbors())
            {
                if (!Neighbor || Visited.Contains(Neighbor)) continue;
                Queue.Enqueue(TPair<AMapNode*, int32>(Neighbor, Dist + 1));
                Visited.Add(Neighbor);
            }
        }
    }
    return Result;
}