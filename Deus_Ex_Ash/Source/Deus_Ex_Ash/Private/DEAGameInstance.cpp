// Fill out your copyright notice in the Description page of Project Settings.


#include "DEAGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Bonfire.h"


void UDEAGameInstance::Init()
{
	Super::Init();

	FBonfireInfo Bonfire_Tutorial
	{
		FVector(1263.0f, -711.0f, 88.0f),
		FRotator(0.0f, 90.0f, 0.0f)
	};

	FBonfireInfo Bonfire_Chapter1
	{
		FVector(0.0f, 0.0f, 10.0f),
		FRotator(0.0f, 0.0f, 0.0f)
	};

	FBonfireInfo Bonfire_Chapter4
	{
		FVector(3772.0f, -3912.0f, 505.5f),
		FRotator(0.0f, -90.0f, 0.0f)
	};

	BonfireInfoByLevelName.Add(GetLevelName(ELevelNameEnum::Tutorial), Bonfire_Tutorial);
	BonfireInfoByLevelName.Add(GetLevelName(ELevelNameEnum::Chapter1), Bonfire_Chapter1);
	BonfireInfoByLevelName.Add(GetLevelName(ELevelNameEnum::Chapter4), Bonfire_Chapter4);
}

void UDEAGameInstance::Shutdown()
{
	Super::Shutdown();


}

FName UDEAGameInstance::GetLevelName(ELevelNameEnum LevelNameEnum)
{
	return LevelNameByEnum[LevelNameEnum];
}

ELevelNameEnum UDEAGameInstance::GetLevelEnum(FName LevelName)
{
	TArray<ELevelNameEnum> LevelNameEnumKeyArray;
	LevelNameByEnum.GenerateKeyArray(LevelNameEnumKeyArray);

	for (int32 i = 0; i < LevelNameByEnum.Num(); i++)
	{
		if (LevelNameByEnum[LevelNameEnumKeyArray[i]] == LevelName)
		{
			return LevelNameEnumKeyArray[i];
		}
	}

	return ELevelNameEnum::None;
}

// 2D --------------------------------------------

bool UDEAGameInstance::ConsumeMovePoint()
{
    if (CurrentMovePoints > 0)
    {
        CurrentMovePoints--;
        return true;
    }
    else
    {
        return false;
    }
}

void UDEAGameInstance::RegisterEnemy(AEnemy* Enemy)
{
    if (!IsValid(Enemy)) return;
    Enemies.AddUnique(Enemy);
}

void UDEAGameInstance::UnregisterEnemy(AEnemy* Enemy)
{
    if (!Enemy) return;
    Enemies.RemoveAll([Enemy](const TWeakObjectPtr<AEnemy>& W) { return W.Get() == Enemy; });
}

void UDEAGameInstance::RunEnemiesTurn()
{
    PendingMoves = 0;

    for (auto& W : Enemies)
    {
        auto* Next = W->NextNode();
        ++PendingMoves;
        W->SetDest(Next);

    }

    if (PendingMoves == 0)
    {
        OnEnemiesTurnEnded.Broadcast();
    }
}

void UDEAGameInstance::NotifyEnemyArrived(ASpine_EntityBase* /*Enemy*/)
{
    if (PendingMoves > 0 && --PendingMoves == 0)
    {
        OnEnemiesTurnEnded.Broadcast();
    }
}

void UDEAGameInstance::DeleteEnemy(AEnemy* Enemy)
{
    Enemies.RemoveSingle(Enemy);
}

void UDEAGameInstance::Compact()
{
    Enemies.RemoveAll([](const TWeakObjectPtr<AEnemy>& W) { return !W.IsValid(); });
}