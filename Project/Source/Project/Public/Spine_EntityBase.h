// 2D Spine Object Base Class

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapNode.h"
#include "Spine_EntityBase.generated.h"




UCLASS()
class PROJECT_API ASpine_EntityBase : public APawn
{
	GENERATED_BODY()
	
protected:
	/** ���� ��ġ�� ��� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	AMapNode* curNode = nullptr;

	/** �̵� ������ ���(������ �̵� ��) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	AMapNode* destNode = nullptr;

public:	
	// Sets default values for this actor's properties
	ASpine_EntityBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** ������ ���� (�� ĭ �̵� ����) */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetDest(AMapNode* node);

	/** ���� �̵� ������(BP���� ���� ���ϰ�) */
	UFUNCTION(BlueprintPure, Category = "Movement")
	bool IsMoving() const { return destNode != nullptr; }

	/** ���� �˸� �̺�Ʈ (�ڽ� BP���� �����ؼ� �ޱ�) */
	UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
	void OnArrivedAtNode(AMapNode* FromNode, AMapNode* ArrivedNode);

	// �̵� ���� �̺�Ʈ
	UFUNCTION(BlueprintImplementableEvent, Category="Movement")
	void OnStartMove(AMapNode* FromNode, AMapNode* ToNode);
	
public:
};
