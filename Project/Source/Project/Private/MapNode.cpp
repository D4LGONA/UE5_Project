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

static FLinearColor ColorOf(EMapNodeType Type)
{
	switch (Type)
	{
	case EMapNodeType::Start:       return FLinearColor::White;
	case EMapNodeType::Normal:      return FLinearColor(0.85f, 0.85f, 0.85f);
	case EMapNodeType::Locked:      return FLinearColor::Red;
	case EMapNodeType::Interaction: return FLinearColor::Yellow;
	case EMapNodeType::Event:       return FLinearColor(1.0f, 0.6f, 0.0f); // 주황/골드
	case EMapNodeType::Exit:        return FLinearColor::Blue;
	default:                        return FLinearColor::Black;
	}
}

void AMapNode::RefreshVisuals()
{
	EnsureMID();

	// 머티리얼 파라미터명은 프로젝트에 맞춰 변경
	if (DiscMID)
	{
		const FLinearColor C = ColorOf(Nodetype.Type);
		DiscMID->SetVectorParameterValue(FName("BaseColor"), C);

		// 잠김 표시를 따로 주고 싶다면 Emissive/Outline 계열 파라미터에 가중치 주기
		const bool bLocked = (Nodetype.State == EMapNodeState::Closed) || (Nodetype.Type == EMapNodeType::Locked);
		DiscMID->SetScalarParameterValue(FName("LockedIntensity"), bLocked ? 1.0f : 0.0f);
	}

#if WITH_EDITOR
	// 에디터 뷰에서 타입 라벨
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

	// (선택) 2D 편집 좌표를 Actor 위치와 동기화하고 싶으면 여기에 반영
	// Nodetype.Pos = FVector2D(GetActorLocation().X, GetActorLocation().Y);

	// 시각 갱신
	RefreshVisuals();

#if WITH_EDITOR
	// 에디터에서 이웃 간 디버그 라인
	const FVector From = GetActorLocation();
	for (const TObjectPtr<AMapNode>& N : Neighbors)
	{
		if (!IsValid(N) || N == this) continue;

		const FVector To = N->GetActorLocation();
		// 편도 중복 라인 방지: Id가 더 큰 쪽에서만 그림
		if (N->Nodetype.Id < Nodetype.Id) continue;

		DrawDebugLine(GetWorld(), From, To, FColor::Silver, false, 0.f, 0, 2.f);
	}

	// (선택) 구조체 내부 NeighborIds 자동 동기화
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
