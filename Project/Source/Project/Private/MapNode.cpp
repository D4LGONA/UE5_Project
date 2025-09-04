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

	// ����: �����Ϳ����� �浹/��������
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

static FLinearColor ColorOf(EMapNodeType Type)
{
	switch (Type)
	{
	case EMapNodeType::Start:       return FLinearColor::White;
	case EMapNodeType::Normal:      return FLinearColor(0.85f, 0.85f, 0.85f);
	case EMapNodeType::Locked:      return FLinearColor::Red;
	case EMapNodeType::Interaction: return FLinearColor::Yellow;
	case EMapNodeType::Event:       return FLinearColor(1.0f, 0.6f, 0.0f); // ��Ȳ/���
	case EMapNodeType::Exit:        return FLinearColor::Blue;
	default:                        return FLinearColor::Black;
	}
}

void AMapNode::RefreshVisuals()
{
	EnsureMID();

	// ��Ƽ���� �Ķ���͸��� ������Ʈ�� ���� ����
	if (DiscMID)
	{
		const FLinearColor C = ColorOf(Nodetype.Type);
		DiscMID->SetVectorParameterValue(FName("BaseColor"), C);

		// ��� ǥ�ø� ���� �ְ� �ʹٸ� Emissive/Outline �迭 �Ķ���Ϳ� ����ġ �ֱ�
		const bool bLocked = (Nodetype.State == EMapNodeState::Closed) || (Nodetype.Type == EMapNodeType::Locked);
		DiscMID->SetScalarParameterValue(FName("LockedIntensity"), bLocked ? 1.0f : 0.0f);
	}

#if WITH_EDITOR
	// ������ �信�� Ÿ�� ��
	FVector TextPos = GetActorLocation() + FVector(0, 0, 60);
	const FString Label = FString::Printf(TEXT("[%d] %s"), Nodetype.Id, *UEnum::GetValueAsString(Nodetype.Type));
	DrawDebugString(GetWorld(), TextPos, Label, nullptr, FColor::White, 0.f, true, 1.0f);
#endif
}

void AMapNode::SetState(EMapNodeState NewState)
{
	Nodetype.State = NewState;
	RefreshVisuals();
}

void AMapNode::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// (����) 2D ���� ��ǥ�� Actor ��ġ�� ����ȭ�ϰ� ������ ���⿡ �ݿ�
	// Nodetype.Pos = FVector2D(GetActorLocation().X, GetActorLocation().Y);

	// �ð� ����
	RefreshVisuals();

#if WITH_EDITOR
	// �����Ϳ��� �̿� �� ����� ����
	const FVector From = GetActorLocation();
	for (const TObjectPtr<AMapNode>& N : Neighbors)
	{
		if (!IsValid(N) || N == this) continue;

		const FVector To = N->GetActorLocation();
		// �� �ߺ� ���� ����: Id�� �� ū �ʿ����� �׸�
		if (N->Nodetype.Id < Nodetype.Id) continue;

		DrawDebugLine(GetWorld(), From, To, FColor::Silver, false, 0.f, 0, 2.f);
	}

	// (����) ����ü ���� NeighborIds �ڵ� ����ȭ
	Nodetype.NeighborIds.Reset();
	for (const TObjectPtr<AMapNode>& N : Neighbors)
	{
		if (IsValid(N))
		{
			Nodetype.NeighborIds.AddUnique(N->Nodetype.Id);
		}
	}
#endif
}
