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

private:
    UFUNCTION(BlueprintCallable, Category = "Startup")
    APawn* SpawnPlayer();
};
