// MapManager.cpp
#include "MapManager.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "AssetRegistry/AssetRegistryModule.h" // (신규 생성 시 필요)
#include "Editor.h" 

AMapManager::AMapManager() {
    PrimaryActorTick.bCanEverTick = false;
}

void AMapManager::BakePlacedNodesToAsset(UNodeGraphData* OutAsset)
{
#if WITH_EDITOR
    if (!OutAsset) { 
        UE_LOG(LogTemp, Warning, TEXT("Bake: OutAsset is null")); 
        return; 
    }

    UWorld* W = GetWorld();
    if (!W) return;

    // 1) 레벨의 모든 AMapNode 수집
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(W, AMapNode::StaticClass(), Found);
    if (Found.Num() == 0) { 
        UE_LOG(LogTemp, Warning, TEXT("Bake: No AMapNode found in level.")); 
        return; 
    }

    // 2) NodeId -> AMapNode 매핑 (유효 Id만)
    TMap<int32, AMapNode*> NodeByIdLocal;
    NodeByIdLocal.Reserve(Found.Num());
    for (AActor* A : Found) {
        if (AMapNode* N = Cast<AMapNode>(A)) {
            const int32 Id = N->Nodetype.Id;
            if (Id < 0) {
                UE_LOG(LogTemp, Warning, TEXT("Bake: Node %s has invalid NodeId(%d). Assign IDs first."),
                    *N->GetName(), Id);
                continue;
            }
            NodeByIdLocal.Add(Id, N);
        }
    }
    if (NodeByIdLocal.Num() == 0) { 
        UE_LOG(LogTemp, Warning, TEXT("Bake: No valid NodeId.")); 
        return; 
    }

    // 3) FMapNodeDef 배열 구성 (월드 XY를 그대로 저장)
    TArray<FMapNodeDef> NewDefs;
    NewDefs.Reserve(NodeByIdLocal.Num());

    for (const auto& Pair : NodeByIdLocal) {
        const int32 Id = Pair.Key;
        const AMapNode* N = Pair.Value;
        if (!IsValid(N)) continue;

        FMapNodeDef Def;
        Def.Id  = Id;

        const FVector WP = N->GetActorLocation();
        Def.Pos = FVector2D(WP.X, WP.Y); // 월드 XY 그대로, Z는 무시

        // 이웃 긁어오기: (널/자기참조/미등록 노드 방어)
        TSet<int32> NbSet; // 중복 제거 겸용
        for (AMapNode* Nb : N->Neighbors) {
            if (!IsValid(Nb)) continue;
            const int32 NbId = Nb->Nodetype.Id;
            if (NbId <= 0) continue;
            if (NbId == Id) continue;                         // 자기참조 제거
            if (!NodeByIdLocal.Contains(NbId)) continue;      // 맵에 없는 노드면 스킵
            NbSet.Add(NbId);
        }

        // 정렬된 배열로 변환
        Def.NeighborIds = NbSet.Array();
        Algo::Sort(Def.NeighborIds);

        NewDefs.Add(Def);
    }

    // 4) DA에 덮어쓰기 + 더티 마킹
    OutAsset->Modify();
    OutAsset->Nodes = MoveTemp(NewDefs);
    OutAsset->MarkPackageDirty();

    UE_LOG(LogTemp, Log, TEXT("Bake: Wrote %d nodes to %s"), OutAsset->Nodes.Num(), *OutAsset->GetName());
#else
    UE_LOG(LogTemp, Warning, TEXT("BakePlacedNodesToAsset is editor-only."));
#endif
}

void AMapManager::SpawnPlayer()
{
    UWorld* W = GetWorld();
    if (!W) return;

    // StartNode가 지정되어 있지 않으면 DA에서 Type==Start를 찾아본다 → 실패 시 최소 ID 노드로 폴백
    if (!IsValid(StartNode)) {
        int32 StartId = -1;

        if (TargetAsset) {
            for (const FMapNodeDef& N : TargetAsset->Nodes) {
                if (N.Type == EMapNodeType::Start) { StartId = N.Id; break; }
            }
        }
        if (StartId == -1 && NodeById.Num() > 0) {
            int32 MinId = TNumericLimits<int32>::Max();
            for (const auto& Pair : NodeById) { MinId = FMath::Min(MinId, Pair.Key); }
            StartId = MinId;
        }

        if (StartId != -1) {
            StartNode = NodeById.FindRef(StartId);
        }
    }

    if (!IsValid(StartNode)) {
        UE_LOG(LogTemp, Error, TEXT("SpawnPlayer: Start node not found."));
        return;
    }

    const FVector SpawnLoc = StartNode->GetActorLocation();
    const FRotator SpawnRot = FRotator::ZeroRotator;


    // 새로 스폰
    if (!*PlayerClass) {
        UE_LOG(LogTemp, Warning, TEXT("SpawnPlayer: PlayerPawnClass is not set."));
        return;
    }

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    APawn* NewPawn = W->SpawnActor<APawn>(PlayerClass, SpawnLoc, SpawnRot, Params);
    if (!NewPawn) {
        UE_LOG(LogTemp, Error, TEXT("SpawnPlayer: Failed to spawn player pawn."));
        return;
    }
}

void AMapManager::BeginPlay()
{
    LoadData();
    SpawnPlayer();
}

void AMapManager::LoadData()
{
    UWorld* W = GetWorld();
    if (!W) return;

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(W, AMapNode::StaticClass(), Found);
    if (Found.Num() == 0) {
        UE_LOG(LogTemp, Warning, TEXT("Bake: No AMapNode found in level."));
        return;
    }

    for (AActor* A : Found) {
        if (AMapNode* N = Cast<AMapNode>(A)) {
            const int32 Id = N->Nodetype.Id;
            NodeById.Add(Id, N);
            if (N->Nodetype.Type == EMapNodeType::Start)
                StartNode = N;
        }
    }
}

void AMapManager::ClearGraph() {
    for (AActor* L : SpawnedLinks) if (IsValid(L)) L->Destroy();
    SpawnedLinks.Empty();

    for (auto& Pair : NodeById) if (IsValid(Pair.Value)) Pair.Value->Destroy();
    NodeById.Empty();

    EdgeSet.Empty();
}

void AMapManager::BuildGraph() {
    ClearGraph();
    if (!TargetAsset || !*NodeClass) return;

    UWorld* W = GetWorld();
    if (!W) return;

    // 1) 노드 스폰
    for (const FMapNodeDef& Def : TargetAsset->Nodes) {
        FActorSpawnParameters S;
        S.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AMapNode* Node = W->SpawnActor<AMapNode>(NodeClass, FVector(Def.Pos, 0.0f), FRotator::ZeroRotator, S);
        if (!Node) continue;

        Node->Nodetype.Id = Def.Id;
        Node->Nodetype.NeighborIds = Def.NeighborIds;
        if(Def.Type == EMapNodeType::Start)
            StartNode = Node;

        NodeById.Add(Def.Id, Node);
    }

    // === 1) NeighborIds를 양방향으로 보정 ===
    for (auto& Pair : NodeById)
    {
        const int32 AId = Pair.Key;
        AMapNode* ANode = Pair.Value;
        if (!IsValid(ANode)) continue;

        // A의 이웃을 순회하면서 B에도 A를 추가
        for (int32 BId : ANode->Nodetype.NeighborIds)
        {
            if (BId == AId) continue; // 자기참조 방지
            AMapNode* BNode = NodeById.FindRef(BId);
            if (!IsValid(BNode)) continue;

            // B에 A가 없으면 추가
            if (!BNode->Nodetype.NeighborIds.Contains(AId))
            {
                BNode->Nodetype.NeighborIds.Add(AId);
            }
        }
    }

    // (선택) 정렬 + 중복제거
    for (auto& Pair : NodeById)
    {
        const int32 NodeId = Pair.Key;
        AMapNode* Node = Pair.Value;
        if (!IsValid(Node)) continue;

        // 중복 제거 + 자기 자신 제거
        TSet<int32> Dedup(Node->Nodetype.NeighborIds);
        Dedup.Remove(NodeId);

        // 배열로 되돌리고 정렬
        Node->Nodetype.NeighborIds = Dedup.Array();
        Node->Nodetype.NeighborIds.Sort();
    }

    // === 2) 포인터 배열 Neighbors도 최종 Id 기준으로 채우기 ===
    for (auto& Pair : NodeById)
    {
        const int32 AId = Pair.Key;
        AMapNode* ANode = Pair.Value;
        if (!IsValid(ANode)) continue;

        ANode->Neighbors.Reset();
        for (int32 BId : ANode->Nodetype.NeighborIds)
        {
            if (BId == AId) continue;
            if (AMapNode* BNode = NodeById.FindRef(BId))
            {
                ANode->Neighbors.Add(BNode);
            }
        }
    }

    // 2) 링크 스폰: 각 노드의 NeighborIds 사용, (min,max) 키로 중복 방지
    EdgeSet.Empty();

    auto KeyOf = [](int32 A, int32 B) {
        return (A < B) ? FIntPoint(A, B) : FIntPoint(B, A);
        };

    for (const TPair<int32, AMapNode*>& Pair : NodeById)
    {
        const int32 AId = Pair.Key;
        AMapNode* ANode = Pair.Value;
        if (!IsValid(ANode)) continue;

        // 자기참조/유효하지 않은 이웃 방어
        for (int32 BId : ANode->Nodetype.NeighborIds)
        {
            if (BId == AId) continue;                 // 자기참조 제거
            AMapNode* BNode = NodeById.FindRef(BId);
            if (!IsValid(BNode)) continue;            // 맵에 없는 이웃 스킵

            FIntPoint Key = KeyOf(AId, BId);
            if (EdgeSet.Contains(Key)) continue;      // 이미 만든 간선은 스킵

            SpawnLinkOnce(AId, BId);                  // 실제 스폰
            EdgeSet.Add(Key);
        }
    }
}

void AMapManager::SpawnLinkOnce(int32 AId, int32 BId) {
    if (!*LinkClass) return;

    AMapNode* A = NodeById.FindRef(AId);
    AMapNode* B = NodeById.FindRef(BId);
    if (!A || !B) return;

    UWorld* W = GetWorld();
    if (!W) return;

    FActorSpawnParameters S;
    S.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    const FTransform SpawnTransform(FRotator::ZeroRotator, FVector::ZeroVector);
    AMapLink* Link = W->SpawnActor<AMapLink>(LinkClass, SpawnTransform, S);
    Link->StartNode = A;
    Link->EndNode = B;

    Link->SetupLink();

    if (Link) {
        SpawnedLinks.Add(Link);
    }
}
