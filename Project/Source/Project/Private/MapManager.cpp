#include "MapManager.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"

// ====== ���� ======
static TPair<int32, int32> UndirectedKey(int32 A, int32 B)
{
    return (A < B) ? TPair<int32, int32>(A, B) : TPair<int32, int32>(B, A);
}

ENodeType AMapManager::MapNodeType(EMapNodeType In)
{
    switch (In)
    {
    case EMapNodeType::Start:       return ENodeType::Normal; // ������ ǥ�ÿ�, ���� ������ Asset.StartNodeId
    case EMapNodeType::Normal:      return ENodeType::Normal;
    case EMapNodeType::Locked:      return ENodeType::Lock;
    case EMapNodeType::Interaction: return ENodeType::Interact;
    case EMapNodeType::Event:       return ENodeType::Event;
    case EMapNodeType::Exit:        return ENodeType::Exit;
    default:                        return ENodeType::Normal;
    }
}
bool AMapManager::IsLockedDesign(EMapNodeType In)
{
    return In == EMapNodeType::Locked;
}

AMapManager::AMapManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

// ====== ����ũ ======
void AMapManager::BakeFromLevel()
{
#if WITH_EDITOR
    if (!TargetAsset)
    {
        UE_LOG(LogTemp, Error, TEXT("[MapManager] TargetAsset�� ����ֽ��ϴ�."));
        return;
    }

    TArray<AMapNode*> NodesInLevel;
    GatherLevelNodes(NodesInLevel);

    if (NodesInLevel.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("[MapManager] �������� ��带 ã�� ���߽��ϴ�."));
        return;
    }

    // �ߺ� Id üũ
    {
        TSet<int32> Seen; bool bDup = false;
        for (AMapNode* NA : NodesInLevel)
        {
            if (Seen.Contains(NA->Nodetype.Id))
            {
                UE_LOG(LogTemp, Error, TEXT("[MapManager] �ߺ� Id: %d (%s)"),
                    NA->Nodetype.Id, *NA->GetName());
                bDup = true;
            }
            Seen.Add(NA->Nodetype.Id);
        }
        //if (bDup) { UE_LOG(LogTemp, Error, TEXT("[MapManager]: �ߺ� Id�� �ߴ�")); return; }
    }

    // �ʱ�ȭ
    TargetAsset->Nodes.Reset();
    TargetAsset->Edges.Reset();

    // ��� �ۼ�
    TMap<int32, int32> IdToIndex;
    TargetAsset->Nodes.Reserve(NodesInLevel.Num());

    for (AMapNode* NA : NodesInLevel)
    {
        FNodeDef Def;
        Def.Id = NA->Nodetype.Id;
        Def.Type = MapNodeType(NA->Nodetype.Type);
        Def.bLocked = IsLockedDesign(NA->Nodetype.Type) || (NA->Nodetype.State == EMapNodeState::Closed);
        Def.WorldPos = NA->GetActorLocation();

        int32 NewIdx = TargetAsset->Nodes.Add(Def);
        IdToIndex.Add(Def.Id, NewIdx);
    }

    // ���� ���� (Neighbors �� ������ ����)
    TSet<TPair<int32, int32>> Added;
    for (AMapNode* NA : NodesInLevel)
    {
        const int32 FromId = NA->Nodetype.Id;
        for (AMapNode* NB : NA->Neighbors)
        {
            if (!IsValid(NB)) continue;
            const int32 ToId = NB->Nodetype.Id;
            if (FromId == ToId) continue;

            const auto Key = UndirectedKey(FromId, ToId);
            if (!Added.Contains(Key))
            {
                Added.Add(Key);
                FNodeEdge E; E.FromId = FromId; E.ToId = ToId;
                TargetAsset->Edges.Add(E);
            }
        }
    }

    // �̿� ��� �ڵ� ����
    TargetAsset->BuildNeighborsFromEdges();

    if (bOverrideStartSettings)
    {
        TargetAsset->StartNodeId = StartNodeId;
        TargetAsset->StartTurns = StartTurns;
    }

    TargetAsset->MarkPackageDirty();
    ValidateGraphAfterBake();

    UE_LOG(LogTemp, Display, TEXT("[MapManager] Bake OK: Nodes=%d, Edges=%d, Start=%d, Turns=%d"),
        TargetAsset->Nodes.Num(), TargetAsset->Edges.Num(),
        TargetAsset->StartNodeId, TargetAsset->StartTurns);

    if (GIsEditor)
    {
        DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 100),
            FString::Printf(TEXT("Bake OK: %d nodes / %d edges"),
                TargetAsset->Nodes.Num(), TargetAsset->Edges.Num()),
            nullptr, FColor::Green, 3.f, true, 1.2f);
    }
#endif
}

void AMapManager::ValidateGraphAfterBake() const
{
    if (!TargetAsset) return;

    // Exit ��� ����
    bool bHasExit = false;
    for (const FNodeDef& N : TargetAsset->Nodes)
        if (N.Type == ENodeType::Exit) { bHasExit = true; break; }
    if (!bHasExit)
        UE_LOG(LogTemp, Warning, TEXT("[MapManager] Exit ��尡 �����ϴ�."));

    // �� ��� ���
    TSet<int32> HasEdge;
    for (const FNodeEdge& E : TargetAsset->Edges)
    {
        HasEdge.Add(E.FromId); HasEdge.Add(E.ToId);
    }

    for (const FNodeDef& N : TargetAsset->Nodes)
    {
        if (!HasEdge.Contains(N.Id))
            UE_LOG(LogTemp, Warning, TEXT("[MapManager] �� ���: Id=%d Pos=%s"),
                N.Id, *N.WorldPos.ToString());
    }
}

// ====== ��Ÿ�� �޸� ���� ======
void AMapManager::InitializeRunFromAsset()
{
    if (!TargetAsset)
    {
        UE_LOG(LogTemp, Error, TEXT("[MapManager] TargetAsset�� �����ϴ�."));
        return;
    }

    CurrentNodeId = TargetAsset->StartNodeId;
    TurnsLeft = TargetAsset->StartTurns;

    NodeStateById.Reset();
    NodeStateById.Reserve(TargetAsset->Nodes.Num());
    for (const FNodeDef& N : TargetAsset->Nodes)
    {
        // ���� ���(bLocked) �� ���� ���¸� Closed��, �ƴϸ� Open
        NodeStateById.Add(N.Id, N.bLocked ? EMapNodeState::Closed : EMapNodeState::Open);
    }
}

void AMapManager::SetNodeState(int32 NodeId, EMapNodeState NewState)
{
    if (NodeStateById.Contains(NodeId))
        NodeStateById[NodeId] = NewState;
}

EMapNodeState AMapManager::GetNodeState(int32 NodeId) const
{
    if (const EMapNodeState* Found = NodeStateById.Find(NodeId))
        return *Found;
    return EMapNodeState::Open; // �⺻
}

void AMapManager::ApplyMemoryStateToLevel()
{
    TArray<AMapNode*> Nodes;
    GatherLevelNodes(Nodes);
    for (AMapNode* N : Nodes)
    {
        if (const EMapNodeState* S = NodeStateById.Find(N->Nodetype.Id))
            N->SetState(*S);
    }
}

void AMapManager::GatherLevelNodes(TArray<AMapNode*>& Out) const
{
    Out.Reset();
    if (UWorld* W = GetWorld())
    {
        for (TActorIterator<AMapNode> It(W, NodeClass ? *NodeClass : AMapNode::StaticClass()); It; ++It)
        {
            if (IsValid(*It)) { Out.Add(*It); }
        }
    }
}

// ��ǥ(X��Y) ���� ����
struct FNodeSortXY
{
    bool operator()(const AMapNode& A, const AMapNode& B) const
    {
        const FVector& PA = A.GetActorLocation();
        const FVector& PB = B.GetActorLocation();
        if (FMath::IsNearlyEqual(PA.X, PB.X, 0.5f))
        {
            return PA.Y < PB.Y;
        }
        return PA.X < PB.X;
    }
};

void AMapManager::AutoAssignIdsInLevel()
{
#if WITH_EDITOR
    TArray<AMapNode*> Nodes;
    GatherLevelNodes(Nodes);

    if (Nodes.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("[MapMaker] �������� AMapNode�� ã�� ���߽��ϴ�."));
        return;
    }

    // ����: ��ǥ(X��Y) �Ǵ� �̸���
    if (bSortIdsByXThenY)
    {
        Nodes.Sort([](const AMapNode& A, const AMapNode& B) { return FNodeSortXY()(A, B); });
    }
    else
    {
        Nodes.Sort([](const AMapNode& A, const AMapNode& B) { return A.GetFName().LexicalLess(B.GetFName()); });
    }

    // 0..N-1 �ڵ� �ο�
    for (int32 i = 0; i < Nodes.Num(); ++i)
    {
        if (!IsValid(Nodes[i])) continue;
        Nodes[i]->Modify();                   // Undo ���
        Nodes[i]->Nodetype.Id = i;            // �� ID ����
        Nodes[i]->RerunConstructionScripts(); // �����/�� ����
    }

    UE_LOG(LogTemp, Display, TEXT("[MapMaker] ID �ڵ��Ҵ� �Ϸ�: %d�� (0..%d)"), Nodes.Num(), Nodes.Num() - 1);
#endif
}