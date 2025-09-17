// GameInstance_2D.cpp
#include "GameInstance_2D.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// ��� Ŭ����/���� ��� (������Ʈ ��ο� �°� include)
#include "MapNode.h"            // AMapNode
#include "NodeGraphData.h"      // FMapNodeDef, EMapNodeType ���� ���⿡ ���� ���ɼ��� ����

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
        UE_LOG(LogTemp, Warning, TEXT("[GameInstance_2D] World ����"));
        return nullptr;
    }

    // ���� Ŭ���� ����: ���ڷ� �켱, ������ Default, �װ͵� ������ GameMode DefaultPawnClass
    if (!*PlayerPawnClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("[GameInstance_2D] ������ PawnClass�� �������� ����"));
        return nullptr;
    }

    // �� �� Start ��� ã��
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(World, AMapNode::StaticClass(), Found);

    AMapNode* StartNode = nullptr;
    for (AActor* A : Found)
    {
        if (AMapNode* Node = Cast<AMapNode>(A))
        {
            // ������Ʈ�� ���� ������ �̸� Ȯ��: Nodetype.Type == EMapNodeType::Start
            if (Node->Nodetype.Type == EMapNodeType::Start)
            {
                StartNode = Node;
                break; // ù ��° Start ���
            }
        }
    }

    if (!StartNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("[GameInstance_2D] Start ��带 ã�� ����"));
        return nullptr;
    }

    // ����
    
    // ���� ��� ��ġ�� �е� �߰�
    FVector SpawnLoc = StartNode->GetActorLocation();
    SpawnLoc.X += PADDING_X;
    SpawnLoc.Z += PADDING_Z;

    // ȸ���� ������ 0,0,0
    FRotator SpawnRot = FRotator::ZeroRotator;

    const FTransform SpawnTM(SpawnRot, SpawnLoc, FVector(1.f));
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    APawn* NewPawn = World->SpawnActor<APawn>(*PlayerPawnClass, SpawnTM, Params);
    if (!NewPawn)
    {
        return nullptr;
    }

    UE_LOG(LogTemp, Log, TEXT("[GameInstance_2D] Start ���(%s)�� Pawn ���� �Ϸ�: %s"),
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
