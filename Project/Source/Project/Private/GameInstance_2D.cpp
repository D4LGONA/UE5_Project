// GameInstance_2D.cpp
#include "GameInstance_2D.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#include "MapNode.h"        
#include "NodeGraphData.h"  

void UGameInstance_2D::Init()
{
    Super::Init();

}

void UGameInstance_2D::Shutdown()
{

    Super::Shutdown();
}

bool UGameInstance_2D::ConsumeMovePoint()
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

void UGameInstance_2D::RegisterEnemy(AEnemy* Enemy)
{
    if (!IsValid(Enemy)) return;
    Enemies.AddUnique(Enemy);
}

void UGameInstance_2D::UnregisterEnemy(AEnemy* Enemy)
{
    if (!Enemy) return;
    Enemies.RemoveAll([Enemy](const TWeakObjectPtr<AEnemy>& W) { return W.Get() == Enemy; });
}

void UGameInstance_2D::RunEnemiesTurn()
{
    PendingMoves = 0;

    for (auto& W : Enemies)
    {
        // todo: 가만히 서있는 애 제외해야 함
        auto* Next = W->NextNode();      
        ++PendingMoves;              
        W->SetDest(Next);
        
    }

    if (PendingMoves == 0)
    {
        OnEnemiesTurnEnded(); 
    }
}

void UGameInstance_2D::NotifyEnemyArrived(ASpine_EntityBase* /*Enemy*/)
{
    if (PendingMoves > 0 && --PendingMoves == 0)
    {
        OnEnemiesTurnEnded(); 
    }
}

void UGameInstance_2D::Compact()
{
    Enemies.RemoveAll([](const TWeakObjectPtr<AEnemy>& W) { return !W.IsValid(); });
}