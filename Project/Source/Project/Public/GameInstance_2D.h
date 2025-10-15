// GameInstance_2D.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Enemy.h"
#include "GameInstance_2D.generated.h"

class AMapNode;

// ��������Ʈ


UCLASS()
class PROJECT_API UGameInstance_2D : public UGameInstance
{
    GENERATED_BODY()

private:
    int32 PendingMoves = 0;
    void Compact(); // ��ȿ���� ���� ������ ����

public:
    virtual void Init() override;
    virtual void Shutdown() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    int32 MaxMovePoints = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    int32 CurrentMovePoints = 10;

    UFUNCTION(BlueprintCallable)
    bool ConsumeMovePoint();  

    UPROPERTY()
    TArray<TWeakObjectPtr<AEnemy>> Enemies;

public:
    // �� ���� �뵵
    // �� ���/����
    UFUNCTION(BlueprintCallable) void RegisterEnemy(AEnemy* Enemy);
    UFUNCTION(BlueprintCallable) void UnregisterEnemy(AEnemy* Enemy);

    // �÷��̾� �̵� ���� ���� ȣ��: �� �� ����
    UFUNCTION(BlueprintCallable) void RunEnemiesTurn();

    UFUNCTION(BlueprintCallable) void NotifyEnemyArrived(class ASpine_EntityBase* Enemy);

    // �Է� Ȱ��ȭ/��Ȱ��ȭ �Ϸ���
    UFUNCTION(BlueprintImplementableEvent) void OnEnemiesTurnEnded();
};

