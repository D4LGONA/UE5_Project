#include "CombatManager.h"
#include "Kismet/KismetSystemLibrary.h"   // 선택: 로그용
#include "Kismet/GameplayStatics.h"       // 선택: 유틸

ACombatManager::ACombatManager()
{
    // 전투 로직은 이벤트 드리븐으로 갈 거라, 틱은 기본 꺼둠
    PrimaryActorTick.bCanEverTick = false;
}

void ACombatManager::BeginPlay()
{
    Super::BeginPlay();

    // 안전하게 시작 페이즈를 Prepare로 보장
    if (Phase != ECombatPhase::Prepare)
    {
        Phase = ECombatPhase::Prepare;
        OnPhaseChanged.Broadcast(Phase);
    }

    // 필요하면 여기서 InitBoard(...) / Setup() 호출 가능
    // (일반적으로는 외부에서 전투 진입 시점에 Setup을 호출하는 걸 권장)
}

void ACombatManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // 현재는 사용 안 함(연출 딜레이/타이머 없이 즉시 처리 예정)
}

void ACombatManager::InitBoard(int32 InSizeX, int32 InSizeY)
{
    BoardSizeX = FMath::Max(2, InSizeX);
    BoardSizeY = FMath::Max(1, InSizeY);

    const int32 MidY = BoardSizeY / 2;

    // 좌표기준 -> 좌하단(0, 0)
    PlayerPos = { 0, 1 };
    EnemyPos = { 3, 1 };

    // 초기 위치 브로드캐스트 -> 이걸로 이미지 띄우기.
    OnUnitMoved.Broadcast(true, PlayerPos);
    OnUnitMoved.Broadcast(false, EnemyPos);
}

void ACombatManager::PushCard(uint8 cardnum, uint8 dir)
{
    // 카드 추가
    PlayerCards[CurCardNum].Type = static_cast<EActionType>(cardnum);
    PlayerCards[CurCardNum].Dir = static_cast<EDir4>(dir);
    CurCardNum++;

    // 카드 넘어갔으면 
    if (CurCardNum >= MAX_CARDS)
    {   
        // 전투 페이즈로 전환
		Phase = ECombatPhase::Action;
    }
}

void ACombatManager::SetDeck() // 플레이어 - 적 사이의 카드들 보고 순서 하나로 합치는 것
{
    Deck.Reset();
    CurDeckIdx = 0;
    CurCardNum = 0;

    auto Priority = [](EActionType T)
        {
            switch (T)
            {
            case EActionType::Defend: return 3;
            case EActionType::Attack: return 2;
            case EActionType::Move:   return 1;
            default: return 0;
            }
        };

    // 슬롯(0..MAX_CARDS-1)을 순서대로 확인하면서 덱 구성
    for (int32 i = 0; i < MAX_CARDS; ++i)
    {
		auto PlayerCard = PlayerCards[i];
		auto EnemyCard = EnemyCards[i];

		const int32 PP = Priority(PlayerCard.Type);
		const int32 EP = Priority(EnemyCard.Type);

        if (PP >= EP) // 플레이어 우선
        {
            Deck.Add({ PlayerCard, true });
            Deck.Add({ EnemyCard, false });
        }
        else if (PP < EP) // 적 우선
        {
            Deck.Add({ EnemyCard, false });
            Deck.Add({ PlayerCard, true });
        }
    }
}

void ACombatManager::ActiveAction()
{
    // 실행 가능한 카드가 없으면 종료
    if (CurDeckIdx >= Deck.Num())
        return;

    TPair<FActionCard, bool> Top = Deck[CurDeckIdx];
    FActionCard Card = Top.Key;
	bool bIsPlayer = Top.Value;

    
    switch (Card.Type)
    {
    case EActionType::Defend:
    {
        // 방어 켜기
        if (true == bIsPlayer) bPlayerBlocking = true;
        else bEnemyBlocking = true;
        break;
    }
    case EActionType::Attack:
    {
        // 만약x축으로 한칸 앞에 적이 있다면 공격
        if (true == bIsPlayer) // 플레이어 턴
        {
            if (true != bEnemyBlocking) // 적이 방어중이 아님
            {
                if (EnemyPos.X == PlayerPos.X + 1 && EnemyPos.Y == PlayerPos.Y) // 공격 성공
                {
                    
                }
            }
        }
        else
        {

        }


        break;
    }
    case EActionType::Move:
    {
        // 상하좌우 확인해서 이동...
    }
    }

    


    // 덱을 모두 소모했으면 다음 턴 준비
    if (CurDeckIdx >= Deck.Num())
    {
        ResetTurn();
    }
}

void ACombatManager::Setup()
{
    // 전투 진입 시 호출: 보통 InitBoard 이후에 불림
    // 여기서는 최소로, 턴 초기화만 수행
    ResetTurn();
}

void ACombatManager::ResetTurn()
{
    // 준비 페이즈로 전환 (카드/방향을 받는 상태)
    Phase = ECombatPhase::Prepare;
    OnPhaseChanged.Broadcast(Phase);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
    UKismetSystemLibrary::PrintString(this, TEXT("[Combat] Phase -> Prepare"), true, true, FLinearColor::Green, 1.5f);
#endif

    // 다음 단계에서:
    // - 가드 초기화
    // - 슬롯 인덱스 0으로
    // - 적 로드아웃(또는 외부 제공) 세팅
    // 등을 추가할 예정
}
