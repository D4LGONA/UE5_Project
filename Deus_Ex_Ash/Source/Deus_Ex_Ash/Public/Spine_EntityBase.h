// 2D Spine Object Base Class

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapNode.h"
#include "Spine_EntityBase.generated.h"


USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MaxHP = 100; // 100 고정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 HP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 ATK = 25; // 고정값 -> 나 25, 적 40
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool DEF = false; // 방어를 했는지에 대한 변수인 듯
};


UCLASS()
class DEUS_EX_ASH_API ASpine_EntityBase : public APawn
{
	GENERATED_BODY()
	
public:
	/** 현재 위치한 노드 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	AMapNode* curNode = nullptr;

	/** 이동 목적지 노드(있으면 이동 중) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	AMapNode* destNode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterStats Stat;

public:	
	// Sets default values for this actor's properties
	ASpine_EntityBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** 목적지 설정 (한 칸 이동 시작) */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetDest(AMapNode* node);

	/** 지금 이동 중인지(BP에서 쓰기 편하게) */
	UFUNCTION(BlueprintPure, Category = "Movement")
	bool IsMoving() const { return destNode != nullptr; }

	

	/** 도착 알림 이벤트 (자식 BP에서 구현해서 받기) */
	UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
	void OnArrivedAtNode(AMapNode* FromNode, AMapNode* ArrivedNode);

	// 이동 시작 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category="Movement")
	void OnStartMove(AMapNode* FromNode, AMapNode* ToNode);
	
public:
};
