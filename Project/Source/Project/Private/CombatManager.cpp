#include "CombatManager.h"
#include "Kismet/KismetSystemLibrary.h"   // 선택: 로그용
#include "Kismet/GameplayStatics.h"       // 선택: 유틸


ACombatManager::ACombatManager()
{
    // 전투 로직은 이벤트 드리븐으로 갈 거라, 틱은 기본 꺼둠
    PrimaryActorTick.bCanEverTick = false;
    PlayerCards.SetNum(3);
    EnemyCards.SetNum(3);
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

// 액션 페이즈 동작
void ACombatManager::ActionPhase()
{
}

void ACombatManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // 현재는 사용 안 함(연출 딜레이/타이머 없이 즉시 처리 예정)
}

void ACombatManager::InitBoard()
{
    // 좌표기준 -> 좌하단(0, 0)
    PlayerPos = { 0, 1 };
    EnemyPos = { 3, 1 };
}

void ACombatManager::PushCard(EActionType cardnum, EDir4 dir)
{
    // 카드 추가
    PlayerCards[CurCardNum].Type = cardnum;
    PlayerCards[CurCardNum].Dir = dir;
    CurCardNum++;
}

bool ACombatManager::IsInsideBoard(const FIntPoint& P) const
{
    return (P.X >= 0 && P.X < BOARDWIDTH &&
        P.Y >= 0 && P.Y < BOARDHEIGHT);
}

void ACombatManager::ApplyDamage(bool IsPlayer)
{
	ASpine_EntityBase* Attacker = IsPlayer ? PlayerPawn : EnemyPawn;
	ASpine_EntityBase* Defender = IsPlayer ? EnemyPawn : PlayerPawn;

    if (true == Defender->Stat.DEF) // 방어
    {
        Defender->Stat.DEF = false;
        return;
	}

    Defender->Stat.HP -= Attacker->Stat.ATK; // 공격당함
    if (Defender->Stat.HP <= 0)
    {
        Defender->Stat.HP = 0;
        if (Defender == PlayerPawn)
            OnPhaseChanged.Broadcast(ECombatPhase::Defeat);
        else
            OnPhaseChanged.Broadcast(ECombatPhase::Victory);
    }

    OnHPChanged.Broadcast(true, PlayerPawn->Stat.MaxHP, PlayerPawn->Stat.HP);
    OnHPChanged.Broadcast(false, EnemyPawn->Stat.MaxHP, EnemyPawn->Stat.HP);
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

void ACombatManager::StepAction()
{
    if (Phase != ECombatPhase::Action) return;
    ActiveAction();   
    CurDeckIdx++;     // 다음 장으로
}

void ACombatManager::ActiveAction()
{
    // 덱을 모두 소모했으면 다음 턴 준비
    if (CurDeckIdx >= Deck.Num())
    {
        ResetTurn();
        return;
    }

    TPair<FActionCard, bool> Top = Deck[CurDeckIdx];
    FActionCard Card = Top.Key;
	bool bIsPlayer = Top.Value;
    
    switch (Card.Type)
    {
    case EActionType::Defend:
    {
		OnImageChange.Broadcast(bIsPlayer, EActionType::Defend);
        // 방어 켜기
        if (true == bIsPlayer) PlayerPawn->Stat.DEF = true;
        else EnemyPawn->Stat.DEF = true;
        break;
    }
    case EActionType::Attack:
    {
        OnImageChange.Broadcast(bIsPlayer, EActionType::Attack);
        if (true == bIsPlayer) // 플레이어의 턴
        {
            switch (Card.Dir)
            {
            case EDir4::Up:
                AttackedPos.Add({ PlayerPos.X, PlayerPos.Y + 1 });
                if (EnemyPos.Y == PlayerPos.Y + 1)
                    ApplyDamage(bIsPlayer);
                break;

            case EDir4::Down:
                AttackedPos.Add({ PlayerPos.X, PlayerPos.Y - 1 });
                if (EnemyPos.Y == PlayerPos.Y - 1)
                    ApplyDamage(bIsPlayer);
                break;

            case EDir4::Left:
                // 왼쪽 1칸
                AttackedPos.Add({ PlayerPos.X - 1, PlayerPos.Y });
                if (EnemyPos.X == PlayerPos.X - 1)
                    ApplyDamage(bIsPlayer);
                break;

            case EDir4::Right:
                AttackedPos.Add({ PlayerPos.X + 1, PlayerPos.Y });
                AttackedPos.Add({ PlayerPos.X + 2, PlayerPos.Y });
                if (EnemyPos.X == PlayerPos.X + 1 || EnemyPos.X == PlayerPos.X + 2)
                    ApplyDamage(bIsPlayer);
                break;
            }
        }
        else // 적의 턴
        {
            switch (EnemyPawn->GetAtkType())
            {
            case EAtkType::Tutorial:
                TutorialAttack(bIsPlayer);
                break;
            case EAtkType::Phase1:
                break;
            case EAtkType::Phase2:
                break;
            }
        }
        break;
    }
    case EActionType::Move:
    {
        OnImageChange.Broadcast(bIsPlayer, EActionType::Move);
        FPos* MyPos = nullptr;
        FPos OtherPos;

        if (bIsPlayer)
        {
            MyPos = &PlayerPos;
            OtherPos = EnemyPos;
        }
        else
        {
            MyPos = &EnemyPos;
            OtherPos = PlayerPos;
        }
		EntityMove(*MyPos, OtherPos, Card);
        break;
    }
    }


    if (false == AttackedPos.IsEmpty())
    {
        OnAttackedPos.Broadcast(AttackedPos);
        AttackedPos.Empty();
    }

    if (bIsPlayer)
    {
        EnemyPawn->Stat.DEF = false;
    }
    else
    {
        PlayerPawn->Stat.DEF = false;
    }
}

void ACombatManager::EntityMove(FPos& MyPos, FPos& OtherPos, FActionCard& Card)
{
    FPos NewPos = MyPos;

    if (Card.Dir == EDir4::Up)
        NewPos.Y += 1;
    else if (Card.Dir == EDir4::Down)
        NewPos.Y -= 1;
    else if (Card.Dir == EDir4::Left)
        NewPos.X -= 1;
    else if (Card.Dir == EDir4::Right)
        NewPos.X += 1;

    if (NewPos.X < 0 || NewPos.X > 3 || NewPos.Y < 0 || NewPos.Y > 2) return;

    // 상대 위치와 겹치면 이동 X
    if (NewPos.X == OtherPos.X && NewPos.Y == OtherPos.Y) return;

    // 최종 적용
    MyPos = NewPos;
    OnUnitMoved.Broadcast();
}

void ACombatManager::EnemySetup() // 적 카드넣는부분
{
    if (EnemyPawn->GetAtkType() == EAtkType::Tutorial)
    {
        // 1) 이동 카드
        EDir4 MoveDir = CalcTutorialMoveDir();
        if (MoveDir != EDir4::None)
        {
            FActionCard& MoveCard = EnemyCards[0];
            MoveCard.Type = EActionType::Move;
            MoveCard.Dir = MoveDir;
        }

        // 2) 공격 카드
        EDir4 AtkDir = CalcTutorialAttackDir();

        FActionCard& AtkCard = EnemyCards[1];
        AtkCard.Type = EActionType::Attack;
        AtkCard.Dir = AtkDir;

        // 3) 방어 카드
        FActionCard& DefCard = EnemyCards[2];
        DefCard.Type = EActionType::Defend;
        DefCard.Dir = EDir4::None;
    }

    // 덱으로 제작
    SetDeck();

    // 적 세팅 후 턴 넘김
    Phase = ECombatPhase::Action;
    OnPhaseChanged.Broadcast(Phase);
}

EDir4 ACombatManager::CalcTutorialMoveDir() const
{
    int32 DX = PlayerPos.X - EnemyPos.X;
    int32 DY = PlayerPos.Y - EnemyPos.Y;

    int32 AbsDX = DX;
    if (AbsDX < 0)
    {
        AbsDX = -AbsDX;
    }

    int32 AbsDY = DY;
    if (AbsDY < 0)
    {
        AbsDY = -AbsDY;
    }

    // 이미 인접(맨해튼 거리 1 이하)이면 이동 안 함
    if (AbsDX + AbsDY <= 1)
    {
        return EDir4::None; // 없으면 만들거나, 그냥 이동카드 안 넣도록 처리
    }

    // 대각선: X축(좌우) 먼저 접근
    if (DX != 0 && DY != 0)
    {
        if (DX > 0)
        {
            return EDir4::Right;
        }
        else
        {
            return EDir4::Left;
        }
    }

    // 수평 정렬(Y 같음) → 좌우로 접근
    if (DY == 0 && DX != 0)
    {
        if (DX > 0)
        {
            return EDir4::Right;
        }
        else
        {
            return EDir4::Left;
        }
    }

    // 수직 정렬(X 같음) → 상하로 접근
    if (DX == 0 && DY != 0)
    {
        if (DY > 0)
        {
            return EDir4::Down;
        }
        else
        {
            return EDir4::Up;
        }
    }

    return EDir4::None;
}

EDir4 ACombatManager::CalcTutorialAttackDir() const
{
    // 같은 줄이면 실제 좌/우 방향
    if (PlayerPos.Y == EnemyPos.Y)
    {
        if (PlayerPos.X < EnemyPos.X)
        {
            return EDir4::Left;
        }
        if (PlayerPos.X > EnemyPos.X)
        {
            return EDir4::Right;
        }

        // 같은 칸이면 튜토리얼용으로 Right 고정
        return EDir4::Right;
    }

    // 위/아래에 있으면 규칙상 무조건 Right
    return EDir4::Right;
}

// 튜토리얼 공격 처리
void ACombatManager::TutorialAttack(bool IsPlayer)
{
    AttackedPos.Add({ EnemyPos.X - 1, EnemyPos.Y });
    if (PlayerPos.Y == EnemyPos.Y && PlayerPos.X == EnemyPos.X - 1)
    {
        ApplyDamage(IsPlayer); 
    }
}

void ACombatManager::Setup(ASpine_EntityBase* Player, AEnemy* Enemy)
{
    PlayerPawn = Player;
    EnemyPawn = Enemy;
    InitBoard();
    ResetTurn();
}

void ACombatManager::ResetTurn()
{
    // 준비 페이즈로 전환 (카드/방향을 받는 상태)
    Phase = ECombatPhase::Prepare;
    OnPhaseChanged.Broadcast(Phase);
}
