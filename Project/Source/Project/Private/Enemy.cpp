// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

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