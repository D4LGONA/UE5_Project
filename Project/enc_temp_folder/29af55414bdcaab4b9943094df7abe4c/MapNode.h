#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapNode.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;
class AMapNode;

// 노드 종류
UENUM(BlueprintType)
enum class EMapNodeType : uint8
{
    Start,         // 시작 노드 -> 흰색(데이터만 존재)
    Normal,
    Locked,        // 잠금(빨강계열)
    Interaction,   // 상호작용/키(노랑계열)
    Event,         // 이벤트(주황/금색계열)
    Exit           // 출구(파랑계열)
};

// 노드 상태
UENUM(BlueprintType)
enum class EMapNodeState : uint8
{
    Closed,    // 잠김/비활성
    Open,      // 해금
};

USTRUCT(BlueprintType)
struct FMapNodeDef
{
    GENERATED_BODY()

    // 노드 아이디
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Id = -1;

    // 노드 좌표
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D Pos = FVector2D::ZeroVector;

    // 이웃 노드 Id들
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> NeighborIds;

    // 노드 타입
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMapNodeType  Type = EMapNodeType::Normal;

    // 노드 상태
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMapNodeState State = EMapNodeState::Open;
};

// 클릭 이벤트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapNodeClicked, AMapNode*, Node);

UCLASS(BlueprintType)
class PROJECT_API AMapNode : public AActor
{
    GENERATED_BODY()
public:
    AMapNode();

    // 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USceneComponent* Root;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UStaticMeshComponent* Disc;   // 원판 메쉬

    // 데이터
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Node", meta = (ShowOnlyInnerProperties))
    FMapNodeDef Nodetype;


    // 인접 노드 (에디터에서 드래그로 지정)
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Node")
    TArray<TObjectPtr<AMapNode>> Neighbors;

    // 동적 머티리얼
    UPROPERTY(Transient) 
    UMaterialInstanceDynamic* DiscMID = nullptr;

    // 상태 변경
    UFUNCTION(BlueprintCallable) void SetState(EMapNodeState NewState);

    virtual void OnConstruction(const FTransform& Transform) override;

    UFUNCTION(BlueprintCallable)
    const TArray<AMapNode*>& GetNeighbors() const { return Neighbors; }


protected:
    void EnsureMID();        // DiscMID 생성 보장
    void RefreshVisuals();   // 색/외곽선 등 갱신
};
