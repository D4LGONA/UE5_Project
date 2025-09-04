#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeGraphData.h"
#include "MapNode.h"         
#include "MapManager.generated.h"

UCLASS()
class PROJECT_API AMapManager : public AActor
{
    GENERATED_BODY()

public:
    AMapManager();

    // ====== 베이크 타깃 ======
    UPROPERTY(EditAnywhere, Category = "Graph|Bake")
    UNodeGraphData* TargetAsset = nullptr;

    UPROPERTY(EditAnywhere, Category = "Graph|Bake")
    TSubclassOf<AMapNode> NodeClass = AMapNode::StaticClass();

    UPROPERTY(EditAnywhere, Category = "Graph|Bake")
    bool bOverrideStartSettings = false;

    UPROPERTY(EditAnywhere, Category = "Graph|Bake", meta = (EditCondition = "bOverrideStartSettings"))
    int32 StartNodeId = 0;

    UPROPERTY(EditAnywhere, Category = "Graph|Bake", meta = (EditCondition = "bOverrideStartSettings"))
    int32 StartTurns = 10;

    UFUNCTION(CallInEditor, Category = "Graph|Bake")
    void BakeFromLevel();

    // ====== 런타임 상태(메모리 전용) ======
    UFUNCTION(BlueprintCallable, Category = "Run")
    void InitializeRunFromAsset();

    UFUNCTION(BlueprintCallable, Category = "Run")
    void ResetRunFromAsset() { InitializeRunFromAsset(); }

    UFUNCTION(BlueprintCallable, Category = "Run")
    void SetNodeState(int32 NodeId, EMapNodeState NewState);

    UFUNCTION(BlueprintCallable, Category = "Run")
    EMapNodeState GetNodeState(int32 NodeId) const;

    UFUNCTION(BlueprintCallable, Category = "Run")
    void SetCurrentNode(int32 NodeId) { CurrentNodeId = NodeId; }

    UFUNCTION(BlueprintCallable, Category = "Run")
    int32 GetCurrentNode() const { return CurrentNodeId; }

    UFUNCTION(BlueprintCallable, Category = "Run")
    void SetTurnsLeft(int32 NewTurns) { TurnsLeft = NewTurns; }

    UFUNCTION(BlueprintCallable, Category = "Run")
    int32 GetTurnsLeft() const { return TurnsLeft; }

    UFUNCTION(BlueprintCallable, Category = "Run|Debug")
    void ApplyMemoryStateToLevel();

    // ====== 맵 제작 보조: ID 자동 할당 ======
    // X→Y 좌표 정렬 기준으로 0..N-1 번호 매김 (끄면 이름순)
    UPROPERTY(EditAnywhere, Category = "MapMaker")
    bool bSortIdsByXThenY = true;

    // 에디터 버튼
    UFUNCTION(CallInEditor, Category = "MapMaker")
    void AutoAssignIdsInLevel();

private:
    // 메모리 전용 진행 상태
    UPROPERTY(Transient) int32 CurrentNodeId = -1;
    UPROPERTY(Transient) int32 TurnsLeft = 0;
    UPROPERTY(Transient) TMap<int32, EMapNodeState> NodeStateById;

    // 헬퍼
    static ENodeType     MapNodeType(EMapNodeType In);
    static bool          IsLockedDesign(EMapNodeType In);
    void                 GatherLevelNodes(TArray<AMapNode*>& Out) const;
    void                 ValidateGraphAfterBake() const;
};