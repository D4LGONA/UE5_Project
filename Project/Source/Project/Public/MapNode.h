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
};

USTRUCT(BlueprintType)
struct FMapNodeDef
{
    GENERATED_BODY()

    // ��� ���̵�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Id = -1;

    // ��� ��ǥ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D Pos = FVector2D::ZeroVector;

    // �̿� ��� Id��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> NeighborIds;

    // ��� Ÿ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMapNodeType  Type = EMapNodeType::Normal;

    // ��� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMapNodeState State = EMapNodeState::Open;
};

// Ŭ�� �̺�Ʈ ��������Ʈ
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
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Node", meta = (ShowOnlyInnerProperties))
    FMapNodeDef Nodetype;


    // ���� ��� (�����Ϳ��� �巡�׷� ����)
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Node")
    TArray<TObjectPtr<AMapNode>> Neighbors;

    // ���� ��Ƽ����
    UPROPERTY(Transient) 
    UMaterialInstanceDynamic* DiscMID = nullptr;

    // ���� ����
    UFUNCTION(BlueprintCallable) void SetState(EMapNodeState NewState);

    virtual void OnConstruction(const FTransform& Transform) override;

    UFUNCTION(BlueprintCallable)
    const TArray<AMapNode*>& GetNeighbors() const { return Neighbors; }


protected:
    void EnsureMID();        // DiscMID ���� ����
    void RefreshVisuals();   // ��/�ܰ��� �� ����
};
