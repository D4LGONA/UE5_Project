//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "NodeTypes.h"
//#include "MapManager.generated.h"
//
//class AMapNode;   // �װ� �̹� ���� ��� C++ Ŭ����
//class AActor;     // ��ũ�� BP�� �� ������ AActor�� �޾Ƽ� ����
//
//UCLASS()
//class PROJECT_API AMapManager : public AActor
//{
//    GENERATED_BODY()
//
//public:
//    AMapManager();
//
//    UPROPERTY(EditAnywhere, Category = "Graph")
//    TSubclassOf<AMapNode> NodeClass;
//
//    UPROPERTY(EditAnywhere, Category = "Graph")
//    TSubclassOf<AActor> LinkClass;
//
//    UPROPERTY(EditAnywhere, Category = "Graph")
//    float ZHeight = 0.f;      // ��� ��� Z
//
//    UPROPERTY(EditAnywhere, Category = "Graph")
//    TArray<FMapNodeDef> NodeDefs;
//
//    UPROPERTY(VisibleAnywhere, Category = "Runtime")
//    TMap<int32, TObjectPtr<AMapNode>> NodeById;
//
//    UPROPERTY(VisibleAnywhere, Category = "Runtime")
//    TArray<TObjectPtr<AActor>> SpawnedLinks;
//
//    UFUNCTION(BlueprintCallable, Category = "Graph") void BuildGraph();
//    UFUNCTION(BlueprintCallable, Category = "Graph") void ClearGraph();
//
//protected:
//    virtual void BeginPlay() override;
//    virtual void OnConstruction(const FTransform& Transform) override;
//
//private:
//    void BuildNodes();
//    void BuildLinks();
//};
