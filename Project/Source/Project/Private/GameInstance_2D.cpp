// GameInstance_2D.cpp
#include "GameInstance_2D.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// 노드 클래스/정의 헤더 (프로젝트 경로에 맞게 include)
#include "MapNode.h"            // AMapNode
#include "NodeGraphData.h"      // FMapNodeDef, EMapNodeType 등이 여기에 있을 가능성이 높음

void UGameInstance_2D::Init()
{
    Super::Init();

}

void UGameInstance_2D::Shutdown()
{

    Super::Shutdown();
}

APawn* UGameInstance_2D::SpawnPlayer()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("[GameInstance_2D] World 없음"));
        return nullptr;
    }

    // 스폰 클래스 결정: 인자로 우선, 없으면 Default, 그것도 없으면 GameMode DefaultPawnClass
    if (!*PlayerPawnClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("[GameInstance_2D] 스폰할 PawnClass가 지정되지 않음"));
        return nullptr;
    }

    // 맵 내 Start 노드 찾기
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(World, AMapNode::StaticClass(), Found);

    AMapNode* StartNode = nullptr;
    for (AActor* A : Found)
    {
        if (AMapNode* Node = Cast<AMapNode>(A))
        {
            // 프로젝트에 맞춰 접근자 이름 확인: Nodetype.Type == EMapNodeType::Start
            if (Node->Nodetype.Type == EMapNodeType::Start)
            {
                StartNode = Node;
                break; // 첫 번째 Start 사용
            }
        }
    }

    if (!StartNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("[GameInstance_2D] Start 노드를 찾지 못함"));
        return nullptr;
    }

    // 스폰
    
    // 원래 노드 위치에 패딩 추가
    FVector SpawnLoc = StartNode->GetActorLocation();
    SpawnLoc.X += PADDING_X;
    SpawnLoc.Z += PADDING_Z;

    // 회전은 무조건 0,0,0
    FRotator SpawnRot = FRotator::ZeroRotator;

    const FTransform SpawnTM(SpawnRot, SpawnLoc, FVector(1.f));
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    APawn* NewPawn = World->SpawnActor<APawn>(*PlayerPawnClass, SpawnTM, Params);
    if (!NewPawn)
    {
        return nullptr;
    }

    UE_LOG(LogTemp, Log, TEXT("[GameInstance_2D] Start 노드(%s)에 Pawn 스폰 완료: %s"),
        *StartNode->GetName(), *NewPawn->GetName());

    return NewPawn;
}

bool UGameInstance_2D::ConsumeMovePoint()
{
    if (CurrentMovePoints > 0)
    {
        CurrentMovePoints--;
        return true;
    }
    else
    {
        return false;
    }
}
