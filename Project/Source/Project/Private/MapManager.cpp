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
//    // ��ũ ���� �ı�
//    for (AActor* L : SpawnedLinks) {
//        if (IsValid(L)) { L->Destroy(); }
//    }
//    SpawnedLinks.Empty();
//
//    // ��� �ı�
//    for (auto& Pair : NodeById) {
//        if (IsValid(Pair.Value)) { Pair.Value->Destroy(); }
//    }
//    NodeById.Empty();
//}
//
//void AMapManager::BuildGraph() {
//    if (!*NodeClass) return; // NodeClass ������
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
//        // �� AMapNode�� ���� �ʵ� ���� (������)
//        Node->NodeId = Def.Id;
//        Node->MapPos2D = Def.Pos;
//
//        NodeById.Add(Def.Id, Node);
//    }
//
//    // Neighbors ä��� (����� ����)
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
//    if (!*LinkClass) return; // ��ũ Ŭ������ �� �������� �н�
//
//    UWorld* W = GetWorld();
//    if (!W) return;
//
//    // (Id ���� ��, ū ��) ������ �ߺ� ����
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
//            // ��ũ�� BP�� �� �����Ƿ� "���� ����" �� BP ����(NodeA/NodeB)�� �����ϰ� Finish
//            AActor* Link = W->SpawnActorDeferred<AActor>(LinkClass, FTransform::Identity, this);
//            if (!Link) continue;
//
//            // BP_MapLinkSegments�� "NodeA", "NodeB" ��� Object ����(AMapNode*)�� �־�� ��!
//            UKismetSystemLibrary::SetObjectPropertyByName(Link, TEXT("NodeA"), A);
//            UKismetSystemLibrary::SetObjectPropertyByName(Link, TEXT("NodeB"), B);
//
//            // ��ũ BP�� Construction Script�� NodeA/NodeB�� ���� ����/ȸ��/�ν��Ͻ� ��ġ
//            UGameplayStatics::FinishSpawningActor(Link, FTransform::Identity);
//            SpawnedLinks.Add(Link);
//        }
//    }
//}
