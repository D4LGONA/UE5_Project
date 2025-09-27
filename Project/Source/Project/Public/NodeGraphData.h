// NodeGraphData.h
#pragma once
#include "CoreMinimal.h"
#include "Engine/PrimaryAssetLabel.h"
#include "MapNode.h"
#include "NodeGraphData.generated.h"

UCLASS(BlueprintType)
class PROJECT_API UNodeGraphData : public UPrimaryDataAsset {
    GENERATED_BODY()
public:
    // 에디터에서 노드 배열을 직접 편집
    // TitleProperty를 NodeId로 지정하면 목록에서 식별이 편해짐
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (TitleProperty = "NodeId", ShowOnlyInnerProperties))
    TArray<FMapNodeDef> Nodes;

#if WITH_EDITOR
    // 간단 검증(중복 ID, 끊긴 이웃 등)
    virtual void PostEditChangeProperty(FPropertyChangedEvent& E) override;
#endif
};
