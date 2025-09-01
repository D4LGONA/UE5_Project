#include "MapNode.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "DrawDebugHelpers.h"

AMapNode::AMapNode()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Disc = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Disc"));
    Disc->SetupAttachment(Root);
    // 클릭 잡히게 Visibility 채널만 Block
    Disc->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Disc->SetCollisionResponseToAllChannels(ECR_Ignore);
    Disc->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

#if WITH_EDITORONLY_DATA
    SetActorScale3D(FVector(0.6f));
#endif

    // 머티리얼 지정
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> NodeMat(
        TEXT("/Game/Materials/M_NodeDisc.M_NodeDisc")); // ← 네 실제 경로로 교체
    if (NodeMat.Succeeded() && !BaseDiscMaterial)
    {
        BaseDiscMaterial = NodeMat.Object;
        if (Disc && !Disc->GetMaterial(0))
        {
            Disc->SetMaterial(0, BaseDiscMaterial);
        }
    }
}

void AMapNode::EnsureMID()
{
    if (!Disc) return;

    // 1) BaseDiscMaterial이 설정돼 있으면 무조건 0번 슬롯을 우리 머티로 바꿔 끼움
    if (BaseDiscMaterial && Disc->GetMaterial(0) != BaseDiscMaterial)
    {
        Disc->SetMaterial(0, BaseDiscMaterial);
    }
    // 2) 그 위에서 동적 MID 생성
    if (!DiscMID)
    {
        DiscMID = Disc->CreateAndSetMaterialInstanceDynamic(0);
    }
}


void AMapNode::SetState(EMapNodeState NewState)
{
    if (State != NewState)
    {
        State = NewState;
        RefreshVisuals();
        //BP_OnStateChanged(State);
    }
}

void AMapNode::NotifyActorOnClicked(FKey /*ButtonPressed*/)
{
    OnNodeClicked.Broadcast(this);
}

void AMapNode::OnConstruction(const FTransform& Transform)
{
    EnsureMID();
    RefreshVisuals();
}

void AMapNode::RefreshVisuals()
{
    EnsureMID();
    if (!DiscMID) return;

    // 타입별 기본 색
    FLinearColor Tint = FLinearColor::White;
    switch (Type)
    {
    case EMapNodeType::Normal:      Tint = FLinearColor(0.95f, 0.95f, 0.95f); break;
    case EMapNodeType::Locked:      Tint = FLinearColor(1.f, 0.2f, 0.2f);     break; // 빨강
    case EMapNodeType::Interaction: Tint = FLinearColor(1.f, 0.9f, 0.2f);     break; // 노랑
    case EMapNodeType::Event:       Tint = FLinearColor(1.f, 0.7f, 0.2f);     break; // 주황
    case EMapNodeType::Exit:        Tint = FLinearColor(0.2f, 0.6f, 1.f);     break; // 파랑
    }

    // 상태에 따른 밝기(알파 대신 밝기로)
    float Brightness = 1.0f;
    if (State == EMapNodeState::Closed)  Brightness = 0.6f;

    const FLinearColor FinalTint = Tint * Brightness;
    DiscMID->SetVectorParameterValue(TEXT("Tint"), FinalTint);
}
