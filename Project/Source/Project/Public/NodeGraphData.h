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
    // �����Ϳ��� ��� �迭�� ���� ����
    // TitleProperty�� NodeId�� �����ϸ� ��Ͽ��� �ĺ��� ������
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (TitleProperty = "NodeId", ShowOnlyInnerProperties))
    TArray<FMapNodeDef> Nodes;

#if WITH_EDITOR
    // ���� ����(�ߺ� ID, ���� �̿� ��)
    virtual void PostEditChangeProperty(FPropertyChangedEvent& E) override;
#endif
};
