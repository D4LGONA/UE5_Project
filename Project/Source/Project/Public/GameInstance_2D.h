// GameInstance_2D.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstance_2D.generated.h"

class AMapNode;

UCLASS()
class PROJECT_API UGameInstance_2D : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void Shutdown() override;

    // 플레이어 클래스
    UPROPERTY(EditAnywhere)
    TSubclassOf<APawn> PlayerPawnClass;

    UFUNCTION(BlueprintCallable, Category = "Startup")
    APawn* SpawnPlayer();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    int32 MaxMovePoints = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    int32 CurrentMovePoints = 10;

    UFUNCTION(BlueprintCallable)
    bool ConsumeMovePoint();  // 1 줄이고 성공/실패 반환
};
