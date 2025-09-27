// MapManager.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeGraphData.h"
#include "MapNode.h"
#include "MapLink.h"
#include "MapManager.generated.h"

UCLASS()
class PROJECT_API AMapManager : public AActor {
    GENERATED_BODY()
public:
    AMapManager();

    // �� ����
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Graph|Bake")
	void BakeGraph() { BakePlacedNodesToAsset(BakeTargetAsset); }

    void BakePlacedNodesToAsset(UNodeGraphData* OutAsset);

    UPROPERTY(EditAnywhere, Category = "Graph|Bake")
	UNodeGraphData* BakeTargetAsset = nullptr;

    // ------------------------------------------------------------------

    // �� �ε�
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Graph|Build")
    void BuildGraph();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Graph|Build")
    UNodeGraphData* TargetAsset = nullptr;

    UPROPERTY(EditAnywhere, Category = "Graph|Build")
    TSubclassOf<AMapNode> NodeClass = AMapNode::StaticClass();

    // ��ũ Ŭ����
    UPROPERTY(EditAnywhere, Category = "Graph|Build")
    TSubclassOf<AActor> LinkClass;

    // ------------------------------------------------------------------

    // �÷��̾� Ŭ����
    UPROPERTY(EditAnywhere, Category = "Graph|Construct")
    TSubclassOf<AActor> PlayerClass;

protected:
    virtual void OnConstruction(const FTransform& Transform) override;

private:
    // ���� ��� ����
    UPROPERTY(Transient) TMap<int32, AMapNode*> NodeById;
    UPROPERTY(Transient) TArray<AActor*> SpawnedLinks;
    
    void ClearGraph();
    void SpawnLinkOnce(int32 AId, int32 BId);

    // �ߺ� ��ũ ������ Ű
    TSet<FIntPoint> EdgeSet; // (min,max)

    // ���� ���
	AMapNode* StartNode = nullptr;
};
