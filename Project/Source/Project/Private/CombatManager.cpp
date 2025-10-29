#include "CombatManager.h"
#include "Kismet/KismetSystemLibrary.h"   // ����: �α׿�
#include "Kismet/GameplayStatics.h"       // ����: ��ƿ

ACombatManager::ACombatManager()
{
    // ���� ������ �̺�Ʈ �帮������ �� �Ŷ�, ƽ�� �⺻ ����
    PrimaryActorTick.bCanEverTick = false;
}

void ACombatManager::BeginPlay()
{
    Super::BeginPlay();

    // �����ϰ� ���� ����� Prepare�� ����
    if (Phase != ECombatPhase::Prepare)
    {
        Phase = ECombatPhase::Prepare;
        OnPhaseChanged.Broadcast(Phase);
    }

    // �ʿ��ϸ� ���⼭ InitBoard(...) / Setup() ȣ�� ����
    // (�Ϲ������δ� �ܺο��� ���� ���� ������ Setup�� ȣ���ϴ� �� ����)
}

void ACombatManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // ����� ��� �� ��(���� ������/Ÿ�̸� ���� ��� ó�� ����)
}

void ACombatManager::InitBoard(int32 InSizeX, int32 InSizeY)
{
    BoardSizeX = FMath::Max(1, InSizeX);
    BoardSizeY = FMath::Max(1, InSizeY);
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
    UKismetSystemLibrary::PrintString(this,
        FString::Printf(TEXT("[Combat] InitBoard: %d x %d"), BoardSizeX, BoardSizeY),
        true, true, FLinearColor::Yellow, 2.0f);
#endif
}

void ACombatManager::PushCard(uint8 cardnum, uint8 dir)
{
    // ī�� �߰�
    PlayerCards[CurCardNum].Type = static_cast<EActionType>(cardnum);
    PlayerCards[CurCardNum].Dir = static_cast<EDir4>(dir);
    CurCardNum++;

    // ī�� �Ѿ���� 
    if (CurCardNum >= MAX_CARDS)
    {   
        // ���� ������� ��ȯ
		Phase = ECombatPhase::Action;
    }
}

void ACombatManager::SetDeck() // �÷��̾� - �� ������ ī��� ���� ���� �ϳ��� ��ġ�� ��
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

    // ����(0..MAX_CARDS-1)�� ������� Ȯ���ϸ鼭 �� ����
    for (int32 i = 0; i < MAX_CARDS; ++i)
    {
		auto PlayerCard = PlayerCards[i];
		auto EnemyCard = EnemyCards[i];

		const int32 PP = Priority(PlayerCard.Type);
		const int32 EP = Priority(EnemyCard.Type);

        if (PP >= EP)
        {
            Deck.Add(PlayerCard);
			Deck.Add(EnemyCard);
        }
        else if (PP < EP)
        {
            Deck.Add(EnemyCard);
            Deck.Add(PlayerCard);
        }
    }
}

void ACombatManager::ActiveAction()
{
    // ���� ������ ī�尡 ������ ����
    if (CurDeckIdx >= Deck.Num())
        return;

    const FActionCard& Top = Deck[CurDeckIdx];


    // ���� ��� �Ҹ������� ���� �� �غ�
    if (CurDeckIdx >= Deck.Num())
    {
        ResetTurn();
    }
}

void ACombatManager::Setup()
{
    // ���� ���� �� ȣ��: ���� InitBoard ���Ŀ� �Ҹ�
    // ���⼭�� �ּҷ�, �� �ʱ�ȭ�� ����
    ResetTurn();
}

void ACombatManager::ResetTurn()
{
    // �غ� ������� ��ȯ (ī��/������ �޴� ����)
    Phase = ECombatPhase::Prepare;
    OnPhaseChanged.Broadcast(Phase);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
    UKismetSystemLibrary::PrintString(this, TEXT("[Combat] Phase -> Prepare"), true, true, FLinearColor::Green, 1.5f);
#endif

    // ���� �ܰ迡��:
    // - ���� �ʱ�ȭ
    // - ���� �ε��� 0����
    // - �� �ε�ƿ�(�Ǵ� �ܺ� ����) ����
    // ���� �߰��� ����
}
