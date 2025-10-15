// GameInstance_2D.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Enemy.h"
#include "GameInstance_2D.generated.h"

class AMapNode;

// 델리게이트


UCLASS()
class PROJECT_API UGameInstance_2D : public UGameInstance
{
    GENERATED_BODY()

private:
    int32 PendingMoves = 0;
    void Compact(); // 유효하지 않은 포인터 정리

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
    // 적 관리 용도
    // 적 등록/해제
    UFUNCTION(BlueprintCallable) void RegisterEnemy(AEnemy* Enemy);
    UFUNCTION(BlueprintCallable) void UnregisterEnemy(AEnemy* Enemy);

    // 플레이어 이동 성공 직후 호출: 적 턴 실행
    UFUNCTION(BlueprintCallable) void RunEnemiesTurn();

    UFUNCTION(BlueprintCallable) void NotifyEnemyArrived(class ASpine_EntityBase* Enemy);

    // 입력 활성화/비활성화 하려고
    UFUNCTION(BlueprintImplementableEvent) void OnEnemiesTurnEnded();
};

