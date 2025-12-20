// MapManager.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeGraphData.h"
#include "MapNode.h"
#include "MapLink.h"
#include "MapManager.generated.h"

UCLASS()
class DEUS_EX_ASH_API AMapManager : public AActor {
    GENERATED_BODY()
public:
    AMapManager();

    // 맵 저장
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Graph|Bake")
	void BakeGraph() { BakePlacedNodesToAsset(BakeTargetAsset); }

    void BakePlacedNodesToAsset(UNodeGraphData* OutAsset);

    UPROPERTY(EditAnywhere, Category = "Graph|Bake")
	UNodeGraphData* BakeTargetAsset = nullptr;

    // ------------------------------------------------------------------

    // 맵 로드
    UFUNCTION(CallInEditor, BlueprintCallable, Category = "Graph|Build")
    void BuildGraph();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Graph|Build")
    UNodeGraphData* TargetAsset = nullptr;

    UPROPERTY(EditAnywhere, Category = "Graph|Build")
    TSubclassOf<AMapNode> NodeClass = AMapNode::StaticClass();

    // 링크 클래스
    UPROPERTY(EditAnywhere, Category = "Graph|Build")
    TSubclassOf<AActor> LinkClass;

    // ------------------------------------------------------------------

    // 플레이어 클래스
    UPROPERTY(EditAnywhere, Category = "Graph|Construct")
    TSubclassOf<AActor> PlayerClass;

    UFUNCTION(BlueprintCallable, Category = "Startup")
    APawn* SpawnPlayer();

    // ------------------------------------------------------------------

    // 데이터 읽어서 저장
    virtual void BeginPlay() override;
    void LoadData();

protected:

    // 스폰 결과 보관
    UPROPERTY(Transient, BlueprintReadWrite) TMap<int32, AMapNode*> NodeById;
    UPROPERTY(Transient, BlueprintReadWrite) TArray<AActor*> SpawnedLinks;


private:

    void ClearGraph();
    void SpawnLinkOnce(int32 AId, int32 BId);

    // 중복 링크 방지용 키
    TSet<FIntPoint> EdgeSet; // (min,max)

    // 시작 노드
	AMapNode* StartNode = nullptr;
};
