// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Bonfire.h"
#include "Enemy.h"
#include "DEAGameInstance.generated.h"

class AMapNode;

// 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemiesTurnEnded);

UENUM(BlueprintType)
enum class ELevelNameEnum : uint8
{
	None UMETA(DisplayName = "None"),
	Tutorial UMETA(DisplayName = "Tutorial"),
	Chapter1 UMETA(DisplayName = "Chapter1"),
	Chapter2 UMETA(DisplayName = "Chapter2"),
	Chapter2_5 UMETA(DisplayName = "Chapter2_5"),
	Chapter3 UMETA(DisplayName = "Chapter3"),
	Chapter4 UMETA(DisplayName = "Chapter4")
};

UCLASS()
class DEUS_EX_ASH_API UDEAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<ELevelNameEnum, FName> LevelNameByEnum
	{
		{ ELevelNameEnum::None, FName("") },
		{ ELevelNameEnum::Tutorial, FName("Tutorial") },
		{ ELevelNameEnum::Chapter1, FName("Map1") },
		{ ELevelNameEnum::Chapter2, FName("Chapter2") },
		{ ELevelNameEnum::Chapter2_5, FName("Chapter2_5") },
		{ ELevelNameEnum::Chapter3, FName("Chapter3") },
		{ ELevelNameEnum::Chapter4, FName("MAP_4") }
	};

	UFUNCTION(BlueprintCallable)
	FName GetLevelName(ELevelNameEnum LevelNameEnum);

	UFUNCTION(BlueprintCallable)
	ELevelNameEnum GetLevelEnum(FName LevelName);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FBonfireInfo> BonfireInfoByLevelName;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELevelNameEnum MoveToBonfireTargetLevelEnum;


	// 2D --------------------------------------------

private:
	int32 PendingMoves = 0;
	void Compact(); // 유효하지 않은 포인터 정리

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	int32 MaxMovePoints = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	int32 CurrentMovePoints = 10;

	UFUNCTION(BlueprintCallable)
	bool ConsumeMovePoint();

	UPROPERTY(EditAnywhere)
	TArray<TWeakObjectPtr<AEnemy>> Enemies;

public:
	// 적 관리 용도
	// 적 등록/해제
	UFUNCTION(BlueprintCallable) void RegisterEnemy(AEnemy* Enemy);
	UFUNCTION(BlueprintCallable) void UnregisterEnemy(AEnemy* Enemy);

	// 플레이어 이동 성공 직후 호출: 적 턴 실행
	UFUNCTION(BlueprintCallable) void RunEnemiesTurn();

	UFUNCTION(BlueprintCallable) void NotifyEnemyArrived(class ASpine_EntityBase* Enemy);
	UFUNCTION(BlueprintCallable) void DeleteEnemy(class AEnemy* Enemy);

	// 입력 활성화/비활성화 하려고
	UPROPERTY(BlueprintAssignable)
	FOnEnemiesTurnEnded OnEnemiesTurnEnded;
};
