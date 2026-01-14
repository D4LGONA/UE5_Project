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
        IsHitGuard = true;
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

    IsHitGuard = false;
    AttackedPos.Empty();
    OnAttackedPos.Broadcast(AttackedPos);

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
        OnImageChange.Broadcast(bIsPlayer, EActionType::Attack);
        }
        else // 적의 턴
        {
            switch (EnemyPawn->GetAtkType())
            {
            case EAtkType::Tutorial:
                TutorialAttack(bIsPlayer, Card);
                break;
            case EAtkType::Phase1:
                Chapter1Attack(bIsPlayer, Card);
                break;
            case EAtkType::Phase2:
                Chapter2Attack(bIsPlayer, Card);
                break;
            }
        OnImageChange.Broadcast(bIsPlayer, EActionType::Attack);
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
        FActionCard& MoveCard = EnemyCards[0];
        MoveCard.Type = EActionType::Move;
        MoveCard.Dir = MoveDir;

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
    if (EnemyPawn->GetAtkType() == EAtkType::Phase1)
    {
        // 1. 방어
        FActionCard& DefCard = EnemyCards[0];
        DefCard.Type = EActionType::Defend;
        DefCard.Dir = EDir4::None;

        // 2. 이동 카드
        EDir4 MoveDir = CalcCh1MoveDir();
        FActionCard& MoveCard = EnemyCards[1];
        MoveCard.Type = EActionType::Move;
        MoveCard.Dir = MoveDir;

        // 3. 공격 카드
        EDir4 AtkDir = CalcCh1AttackDir();
        FActionCard& AtkCard = EnemyCards[2];
        AtkCard.Type = EActionType::Attack;
        AtkCard.Dir = AtkDir;
    }
    if (EnemyPawn->GetAtkType() == EAtkType::Phase2)
    {
        const bool bInRange = IsPlayerInAttackRange_Phase2();

        if (bInRange)
        {
            // 공격 > 방어 > 이동
            EnemyCards[0].Type = EActionType::Attack;
            EnemyCards[0].Dir = EDir4::None;

            EnemyCards[1].Type = EActionType::Defend;
            EnemyCards[1].Dir = EDir4::None;

            EnemyCards[2].Type = EActionType::Move;
            EnemyCards[2].Dir = CalcCh2MoveDir();
        }
        else
        {
            // 이동 > 공격 > 방어
            EnemyCards[0].Type = EActionType::Move;
            EnemyCards[0].Dir = CalcCh2MoveDir();

            EnemyCards[1].Type = EActionType::Attack;
            EnemyCards[1].Dir = EDir4::None;

            EnemyCards[2].Type = EActionType::Defend;
            EnemyCards[2].Dir = EDir4::None;
        }
    }

    // 덱으로 제작
    SetDeck();

    // 적 세팅 후 턴 넘김
    Phase = ECombatPhase::Action;
    OnPhaseChanged.Broadcast(Phase);
}

EDir4 ACombatManager::CalcTutorialMoveDir() const
{
    const int32 DX = PlayerPos.X - EnemyPos.X; // +면 플레이어가 오른쪽
    const int32 DY = PlayerPos.Y - EnemyPos.Y; // +면 플레이어가 위쪽

    const int32 AbsDX = (DX < 0) ? -DX : DX;
    const int32 AbsDY = (DY < 0) ? -DY : DY;

    // 이미 인접(상하좌우 1칸)이면 이동 안 함
    if (AbsDX + AbsDY <= 1)
        return EDir4::None;

    // 대각선이면 좌우(X축) 먼저 접근
    if (DX != 0 && DY != 0)
        return (DX > 0) ? EDir4::Right : EDir4::Left;

    // 수평 정렬(Y 같음) → 좌우로 접근
    if (DY == 0 && DX != 0)
        return (DX > 0) ? EDir4::Right : EDir4::Left;

    // 수직 정렬(X 같음) → 상하로 접근
    // 좌하단(0,0): DY > 0이면 플레이어가 위쪽이므로 Up으로 접근해야 함
    if (DX == 0 && DY != 0)
        return (DY > 0) ? EDir4::Up : EDir4::Down;

    return EDir4::None;
}

EDir4 ACombatManager::CalcCh1MoveDir() const
{
    // 좌하단 (0,0): X+ 오른쪽, Y+ 위
    const int32 DX = PlayerPos.X - EnemyPos.X; // +면 플레이어가 오른쪽
    const int32 DY = PlayerPos.Y - EnemyPos.Y; // +면 플레이어가 위쪽

    const int32 AbsDX = (DX < 0) ? -DX : DX;
    const int32 AbsDY = (DY < 0) ? -DY : DY;

    // 이미 인접(상하좌우 1칸)이면 이동할 필요 없음
    if (AbsDX + AbsDY <= 1)
        return EDir4::None;

    // 대각선이면 좌/우 먼저 접근
    if (DX != 0 && DY != 0)
        return (DX > 0) ? EDir4::Right : EDir4::Left;

    // 수평 정렬(Y 같음) -> 좌우 접근
    if (DY == 0 && DX != 0)
        return (DX > 0) ? EDir4::Right : EDir4::Left;

    // 수직 정렬(X 같음) -> 상하 접근
    if (DX == 0 && DY != 0)
        return (DY > 0) ? EDir4::Up : EDir4::Down;

    return EDir4::None;
}

EDir4 ACombatManager::CalcCh2MoveDir()
{
    const FPos Enemy = EnemyPos;
    const FPos Player = PlayerPos;

    auto DirToDelta = [](EDir4 Dir) -> FPos
        {
            switch (Dir)
            {
            case EDir4::Up:    return FPos(0, 1);
            case EDir4::Down:  return FPos(0, -1);
            case EDir4::Left:  return FPos(-1, 0);
            case EDir4::Right: return FPos(1, 0);
            default:           return FPos(0, 0);
            }
        };

    auto IsInMap = [](const FPos& P) -> bool
        {
            return (P.X >= 0 && P.X <= 3 &&
                P.Y >= 0 && P.Y <= 2);
        };

    auto Score = [&](const FPos& NewEnemy) -> TPair<int, int>
        {
            const int dx = Player.X - NewEnemy.X;
            const int dy = Player.Y - NewEnemy.Y;

            const int RowScore = FMath::Abs(dy);                 
            const int DistScore = FMath::Abs(FMath::Abs(dx) - 1);
            return { RowScore, DistScore };
        };

    // 이미 이상적인 위치면 이동 안 함
    {
        const int dx = Player.X - Enemy.X;
        const int dy = Player.Y - Enemy.Y;
        if (dy == 0 && FMath::Abs(dx) == 1)
            return EDir4::None;
    }

    const EDir4 Candidates[4] = {
        EDir4::Up, EDir4::Down, EDir4::Left, EDir4::Right
    };

    EDir4 BestDir = EDir4::None;
    TPair<int, int> BestScore = Score(Enemy);

    for (EDir4 Dir : Candidates)
    {
        const FPos NewPos = { Enemy.X + DirToDelta(Dir).X, Enemy.Y + DirToDelta(Dir).Y};

        if (!IsInMap(NewPos))
            continue;

        const TPair<int, int> S = Score(NewPos);

        const bool bBetter =
            (S.Key < BestScore.Key) ||
            (S.Key == BestScore.Key && S.Value < BestScore.Value);

        if (bBetter)
        {
            BestScore = S;
            BestDir = Dir;
        }
    }
    return BestDir;
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
    if (EnemyPos.X == 3) return EDir4::Left;
    return EDir4::Right;
}

EDir4 ACombatManager::CalcCh1AttackDir() const
{
    const int32 DX = PlayerPos.X - EnemyPos.X; // +면 플레이어가 오른쪽
    const int32 DY = PlayerPos.Y - EnemyPos.Y; // +면 플레이어가 위쪽

    const int32 AbsDX = (DX < 0) ? -DX : DX;
    const int32 AbsDY = (DY < 0) ? -DY : DY;

    // 대각선이면 좌/우 먼저 접근
    if (DX != 0 && DY != 0)
        return (DX > 0) ? EDir4::Right : EDir4::Left;

    // 수평 정렬(Y 같음) -> 좌우 접근
    if (DY == 0 && DX != 0)
        return (DX > 0) ? EDir4::Right : EDir4::Left;

    // 수직 정렬(X 같음) -> 상하 접근
    if (DX == 0 && DY != 0)
        return (DY > 0) ? EDir4::Up : EDir4::Down;

    return EDir4::None;
}

// 튜토리얼 공격 처리
void ACombatManager::TutorialAttack(bool IsPlayer, FActionCard& card)
{
    switch (card.Dir)
    {
    case EDir4::Left:
        // 왼쪽 1칸
        AttackedPos.Add({ EnemyPos.X - 1, EnemyPos.Y });
        if (EnemyPos.X - 1 == PlayerPos.X && EnemyPos.Y == PlayerPos.Y)
            ApplyDamage(IsPlayer);
        break;

    case EDir4::Right:
        AttackedPos.Add({ EnemyPos.X + 1, EnemyPos.Y });
        if (EnemyPos.X + 1 == PlayerPos.X && EnemyPos.Y == PlayerPos.Y)
            ApplyDamage(IsPlayer);
        break;
    default:
        break;
    }
}

void ACombatManager::Chapter1Attack(bool IsPlayer, FActionCard& Card)
{
    // 상하좌우 1칸 전체 공격
    static const FPos Offsets[] = {
        { -1,  0 }, // Left
        {  1,  0 }, // Right
        {  0,  1 }, // Up
        {  0, -1 }  // Down
    };

    AttackedPos.Reset();

    for (const FPos& Offset : Offsets)
    {
        const FPos TargetPos = EnemyPos + Offset;

        // 맵 범위 체크 (0~3, 0~2)
        if (TargetPos.X < 0 || TargetPos.X > 3 ||
            TargetPos.Y < 0 || TargetPos.Y > 2)
        {
            continue;
        }

        // 공격 표시용 위치는 무조건 기록
        AttackedPos.Add(TargetPos);

        // 플레이어가 있으면 데미지
        if (TargetPos == PlayerPos)
        {
            ApplyDamage(IsPlayer);
        }
    }
}


void ACombatManager::Chapter2Attack(bool IsPlayer, FActionCard& card)
{
    const FPos Origin = EnemyPos;

    AttackedPos.Reset();

    for (const FPos& Offset : HOffsets3x3)
    {
        const FPos TargetPos = Origin + Offset;

        // 맵 범위 체크
        if (TargetPos.X < 0 || TargetPos.X > 3 ||
            TargetPos.Y < 0 || TargetPos.Y > 2)
        {
            continue;
        }

        // 공격 표시용 위치는 무조건 기록
        AttackedPos.Add(TargetPos);

        // 플레이어가 해당 위치에 있으면 데미지 적용
        if (TargetPos == PlayerPos)
        {
            ApplyDamage(IsPlayer);
        }
    }
}

bool ACombatManager::IsPlayerInAttackRange_Phase2() const
{
    const int dx = PlayerPos.X - EnemyPos.X;
    const int dy = PlayerPos.Y - EnemyPos.Y;

    const FPos Delta(dx, dy);

    for (const FPos& Offset : HOffsets3x3)
    {
        if (Delta == Offset)
            return true;
    }
    return false;
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
    PlayerCards.Empty();
    EnemyCards.Empty();
    Deck.Empty();
    PlayerCards.SetNum(3);
    EnemyCards.SetNum(3);
    CurCardNum = 0; // 등록한 카드 개수
    CurDeckIdx = 0; // 발동할 것
    if (EnemyPawn != nullptr && PlayerPawn != nullptr)
    {
        EnemyPawn->Stat.DEF = false;
        PlayerPawn->Stat.DEF = false;
    }
    OnPhaseChanged.Broadcast(Phase);
}
