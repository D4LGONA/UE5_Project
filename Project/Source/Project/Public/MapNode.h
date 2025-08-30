#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapNode.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;
class AMapNode;

// ��� ����
UENUM(BlueprintType)
enum class EMapNodeType : uint8
{
    Start,         // ���� ��� -> ���(�����͸� ����)
    Normal,
    Locked,        // ���(�����迭)
    Interaction,   // ��ȣ�ۿ�/Ű(����迭)
    Event,         // �̺�Ʈ(��Ȳ/�ݻ��迭)
    Exit           // �ⱸ(�Ķ��迭)
};

// ��� ����
UENUM(BlueprintType)
enum class EMapNodeState : uint8
{
    Closed,    // ���/��Ȱ��
    Open,      // �ر�
    Cleared,   // Ŭ����
    Hidden     // ����(�ɼ�)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapNodeClicked, AMapNode*, Node);

UCLASS(BlueprintType)
class PROJECT_API AMapNode : public AActor
{
    GENERATED_BODY()
public:
    AMapNode();

    // ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) USceneComponent* Root;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UStaticMeshComponent* Disc;   // ���� �޽�

    // ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node") int32        NodeId = -1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node") FVector2D    MapPos2D = FVector2D::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node") EMapNodeType Type = EMapNodeType::Normal;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node") EMapNodeState State = EMapNodeState::Closed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node") int32        MoveCost = 1;

    // ���� ��� (�����Ϳ��� �巡�׷� ����)
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Node")
    TArray<TObjectPtr<AMapNode>> Neighbors;

    // (����) �⺻ ��Ƽ���� �ν��Ͻ�(����)�� �����Ϳ��� �Ҵ��صθ� �װ� ������� ���� �ν��Ͻ� ����
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node|Visual")
    UMaterialInterface* BaseDiscMaterial = nullptr;

    // ���� ��Ƽ����
    UPROPERTY(Transient) 
    UMaterialInstanceDynamic* DiscMID = nullptr;

    // Ŭ�� �̺�Ʈ ��ε�ĳ��Ʈ(�Ŵ������� ���ε�)
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMapNodeClicked OnNodeClicked;

    // ���� ����
    UFUNCTION(BlueprintCallable) void SetState(EMapNodeState NewState);

    // BP���� ���� �ٲ� �� ��ó���ϰ� ������ ���� ����
    UFUNCTION(BlueprintImplementableEvent, Category = "Node")
    void BP_OnStateChanged(EMapNodeState NewState);

    // Ŭ�� �ݹ�
    virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
    // ��/��ġ ���� ��(������) ȣ�� �� ��(Neighbors) �����
    virtual void OnConstruction(const FTransform& Transform) override;

protected:
    void EnsureMID();        // DiscMID ���� ����
    void RefreshVisuals();   // ��/�ܰ��� �� ����
};
