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
    Cleared,   // 클리어
    Hidden     // 숨김(옵션)
};

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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node") int32        NodeId = -1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node") FVector2D    MapPos2D = FVector2D::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node") EMapNodeType Type = EMapNodeType::Normal;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node") EMapNodeState State = EMapNodeState::Closed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node") int32        MoveCost = 1;

    // 인접 노드 (에디터에서 드래그로 지정)
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Node")
    TArray<TObjectPtr<AMapNode>> Neighbors;

    // (선택) 기본 머티리얼 인스턴스(에셋)를 에디터에서 할당해두면 그걸 기반으로 동적 인스턴스 생성
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node|Visual")
    UMaterialInterface* BaseDiscMaterial = nullptr;

    // 동적 머티리얼
    UPROPERTY(Transient) 
    UMaterialInstanceDynamic* DiscMID = nullptr;

    // 클릭 이벤트 브로드캐스트(매니저에서 바인딩)
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMapNodeClicked OnNodeClicked;

    // 상태 변경
    UFUNCTION(BlueprintCallable) void SetState(EMapNodeState NewState);

    // BP에서 상태 바뀔 때 후처리하고 싶으면 여기 구현
    UFUNCTION(BlueprintImplementableEvent, Category = "Node")
    void BP_OnStateChanged(EMapNodeState NewState);

    // 클릭 콜백
    virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
    // 값/배치 변경 시(에디터) 호출 → 선(Neighbors) 디버그
    virtual void OnConstruction(const FTransform& Transform) override;

protected:
    void EnsureMID();        // DiscMID 생성 보장
    void RefreshVisuals();   // 색/외곽선 등 갱신
};
