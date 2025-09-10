// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

AMapNode* AEnemy::NextNode()
{
    if (!curNode) return nullptr;

    // 1. 현재 노드 이웃 중
    const TArray<AMapNode*>& Neighbors = curNode->GetNeighbors();

    // 2. AllowedNodes 안에 포함된 것만 후보
    TArray<AMapNode*> ValidNodes;
    for (AMapNode* Node : Neighbors)
    {
        if (AllowedNodes.Contains(Node))
        {
            ValidNodes.Add(Node);
        }
    }

    // 3. 랜덤 선택
    if (ValidNodes.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, ValidNodes.Num() - 1);
        return ValidNodes[Index];
    }

    return nullptr;
}
void AEnemy::InitEnemy(AMapNode* InStartNode)
{
    StartNode = InStartNode;
    AllowedNodes = GetNodesWithinDistance(StartNode, 2);
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