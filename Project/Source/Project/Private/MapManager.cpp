//#include "MapManager.h"
//#include "MapNode.h"
//#include "Kismet/GameplayStatics.h"
//#include "Kismet/KismetSystemLibrary.h" // SetObjectPropertyByName
//#include "Engine/World.h"
//
//AMapManager::AMapManager() {
//    PrimaryActorTick.bCanEverTick = false;
//}
//
//void AMapManager::BeginPlay() {
//    Super::BeginPlay();
//    BuildGraph();
//}
//
//void AMapManager::OnConstruction(const FTransform& Transform) {
//    Super::OnConstruction(Transform);
//    if (bRebuildOnConstruction) {
//        BuildGraph();
//    }
//}
//
//void AMapManager::ClearGraph() {
//    // 링크 먼저 파괴
//    for (AActor* L : SpawnedLinks) {
//        if (IsValid(L)) { L->Destroy(); }
//    }
//    SpawnedLinks.Empty();
//
//    // 노드 파괴
//    for (auto& Pair : NodeById) {
//        if (IsValid(Pair.Value)) { Pair.Value->Destroy(); }
//    }
//    NodeById.Empty();
//}
//
//void AMapManager::BuildGraph() {
//    if (!*NodeClass) return; // NodeClass 미지정
//
//    ClearGraph();
//    BuildNodes();
//    BuildLinks();
//}
//
//void AMapManager::BuildNodes() {
//    UWorld* W = GetWorld();
//    if (!W) return;
//
//    for (const FMapNodeDef& Def : NodeDefs) {
//        const FVector Loc(Def.Pos.X, Def.Pos.Y, ZHeight);
//
//        FActorSpawnParameters Params;
//        Params.Owner = this;
//
//        AMapNode* Node = W->SpawnActor<AMapNode>(NodeClass, Loc, FRotator::ZeroRotator, Params);
//        if (!Node) continue;
//
//        // 네 AMapNode가 가진 필드 세팅 (있으면)
//        Node->NodeId = Def.Id;
//        Node->MapPos2D = Def.Pos;
//
//        NodeById.Add(Def.Id, Node);
//    }
//
//    // Neighbors 채우기 (양방향 보장)
//    for (const FMapNodeDef& Def : NodeDefs) {
//        AMapNode** NodePtr = NodeById.Find(Def.Id);
//        if (!NodePtr) continue;
//        AMapNode* A = *NodePtr;
//
//        for (int32 NId : Def.NeighborIds) {
//            if (AMapNode** BPtr = NodeById.Find(NId)) {
//                A->Neighbors.AddUnique(*BPtr);
//                (*BPtr)->Neighbors.AddUnique(A);
//            }
//        }
//    }
//}
//
//void AMapManager::BuildLinks() {
//    if (!*LinkClass) return; // 링크 클래스를 안 정했으면 패스
//
//    UWorld* W = GetWorld();
//    if (!W) return;
//
//    // (Id 작은 쪽, 큰 쪽) 쌍으로 중복 방지
//    TSet<FIntPoint> Seen;
//
//    for (const FMapNodeDef& Def : NodeDefs) {
//        AMapNode** APtr = NodeById.Find(Def.Id);
//        if (!APtr) continue;
//        AMapNode* A = *APtr;
//
//        for (int32 NId : Def.NeighborIds) {
//            AMapNode** BPtr = NodeById.Find(NId);
//            if (!BPtr) continue;
//            AMapNode* B = *BPtr;
//
//            const FIntPoint Key(FMath::Min(Def.Id, NId), FMath::Max(Def.Id, NId));
//            if (Seen.Contains(Key)) continue;
//            Seen.Add(Key);
//
//            // 링크는 BP일 수 있으므로 "지연 스폰" 후 BP 변수(NodeA/NodeB)를 세팅하고 Finish
//            AActor* Link = W->SpawnActorDeferred<AActor>(LinkClass, FTransform::Identity, this);
//            if (!Link) continue;
//
//            // BP_MapLinkSegments에 "NodeA", "NodeB" 라는 Object 변수(AMapNode*)가 있어야 함!
//            UKismetSystemLibrary::SetObjectPropertyByName(Link, TEXT("NodeA"), A);
//            UKismetSystemLibrary::SetObjectPropertyByName(Link, TEXT("NodeB"), B);
//
//            // 링크 BP의 Construction Script가 NodeA/NodeB를 보고 길이/회전/인스턴스 배치
//            UGameplayStatics::FinishSpawningActor(Link, FTransform::Identity);
//            SpawnedLinks.Add(Link);
//        }
//    }
//}
