#include "NodeGraphData.h"

void UNodeGraphData::BuildNeighborsFromEdges()
{
	TMap<int32, int32> IdToIndex;

	// Id → 인덱스 맵 작성, 기존 Neighbors 비우기
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		IdToIndex.Add(Nodes[i].Id, i);
		Nodes[i].Neighbors.Reset();
	}

	// 엣지 돌면서 Neighbors 채우기
	for (const auto& E : Edges)
	{
		const int32* FromIdx = IdToIndex.Find(E.FromId);
		const int32* ToIdx = IdToIndex.Find(E.ToId);

		if (FromIdx && ToIdx)
		{
			Nodes[*FromIdx].Neighbors.AddUnique(Nodes[*ToIdx].Id);
			Nodes[*ToIdx].Neighbors.AddUnique(Nodes[*FromIdx].Id);
		}
	}

#if WITH_EDITOR
	MarkPackageDirty();
#endif
}
