#include "NodeGraphData.h"

#if WITH_EDITOR
void UNodeGraphData::PostEditChangeProperty(FPropertyChangedEvent& E) {
    Super::PostEditChangeProperty(E);

    TSet<int32> Seen;
    for (const auto& n : Nodes) {
        if (Seen.Contains(n.Id)) {
            UE_LOG(LogTemp, Warning, TEXT("[GraphData] Duplicate NodeId: %d"), n.Id);
        }
        Seen.Add(n.Id);
    }
    // 존재하지 않는 NeighborId 경고
    for (const auto& n : Nodes) {
        for (int32 nb : n.NeighborIds) {
            if (!Seen.Contains(nb)) {
                UE_LOG(LogTemp, Warning, TEXT("[GraphData] Node %d has unknown neighbor %d"), n.Id, nb);
            }
        }
    }
}
#endif
