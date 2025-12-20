//// DEAGameInstance.cpp
//#include "DEAGameInstance.h"
//#include "Kismet/GameplayStatics.h"
//#include "GameFramework/PlayerController.h"
//#include "Engine/World.h"
//
//#include "MapNode.h"        
//#include "NodeGraphData.h"  
//
//void UDEAGameInstance::Init()
//{
//    Super::Init();
//
//}
//
//void UDEAGameInstance::Shutdown()
//{
//
//    Super::Shutdown();
//}
//
//bool UDEAGameInstance::ConsumeMovePoint()
//{
//    if (CurrentMovePoints > 0)
//    {
//        CurrentMovePoints--;
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
//
//void UDEAGameInstance::RegisterEnemy(AEnemy* Enemy)
//{
//    if (!IsValid(Enemy)) return;
//    Enemies.AddUnique(Enemy);
//}
//
//void UDEAGameInstance::UnregisterEnemy(AEnemy* Enemy)
//{
//    if (!Enemy) return;
//    Enemies.RemoveAll([Enemy](const TWeakObjectPtr<AEnemy>& W) { return W.Get() == Enemy; });
//}
//
//void UDEAGameInstance::RunEnemiesTurn()
//{
//    PendingMoves = 0;
//
//    for (auto& W : Enemies)
//    {
//        // todo: 가만히 서있는 애 제외해야 함
//        auto* Next = W->NextNode();      
//        ++PendingMoves;              
//        W->SetDest(Next);
//        
//    }
//
//    if (PendingMoves == 0)
//    {
//        OnEnemiesTurnEnded.Broadcast();
//    }
//}
//
//void UDEAGameInstance::NotifyEnemyArrived(ASpine_EntityBase* /*Enemy*/)
//{
//    if (PendingMoves > 0 && --PendingMoves == 0)
//    {
//        OnEnemiesTurnEnded.Broadcast();
//    }
//}
//
//void UDEAGameInstance::DeleteEnemy(AEnemy* Enemy)
//{
//    Enemies.RemoveSingle(Enemy);
//}
//
//void UDEAGameInstance::Compact()
//{
//    Enemies.RemoveAll([](const TWeakObjectPtr<AEnemy>& W) { return !W.IsValid(); });
//}