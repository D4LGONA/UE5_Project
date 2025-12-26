#include "MapNode.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

AMapNode::AMapNode()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Disc = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Disc"));
	Disc->SetupAttachment(Root);

	// 선택: 에디터에서만 충돌/선택편의
	Disc->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Disc->SetGenerateOverlapEvents(false);
}

void AMapNode::EnsureMID()
{
	if (!Disc) return;
	if (!DiscMID)
	{
		UMaterialInterface* BaseMat = Disc->GetMaterial(0);
		if (BaseMat)
		{
			DiscMID = UMaterialInstanceDynamic::Create(BaseMat, this);
			Disc->SetMaterial(0, DiscMID);
		}
	}
}

void AMapNode::SetState(EMapNodeState NewState)
{
	Nodetype.State = NewState;
}

void AMapNode::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// (선택) 구조체 내부 NeighborIds 자동 동기화
	Nodetype.NeighborIds.Reset();
	for (const TObjectPtr<AMapNode>& N : Neighbors)
	{
		if (IsValid(N))
		{
			Nodetype.NeighborIds.AddUnique(N->Nodetype.Id);
		}
	}
}
