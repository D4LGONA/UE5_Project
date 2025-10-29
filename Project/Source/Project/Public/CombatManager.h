// CombatManager.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatManager.generated.h"

constexpr short MAX_CARDS = 3;

class AActor; // �÷��̾�/�� ���� Ÿ�� �����Ӱ�

// ���������� -> �׼������� -> �¸�/�й�
UENUM(BlueprintType)
enum class ECombatPhase : uint8 { Prepare, Action, Victory, Defeat };

UENUM(BlueprintType) // �ൿ Ÿ��
enum class EActionType : uint8 { Defend = 0, Attack = 1, Move = 2 };

UENUM(BlueprintType) // �̵� ����(None: Attack/Defend���� ���)
enum class EDir4 : uint8 { None = 0, Up, Down, Left, Right };

USTRUCT(BlueprintType)
struct FPos { // ��ġ ����
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 X = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Y = 0;
    bool operator==(const FPos& O) const { return X == O.X && Y == O.Y; }
};

USTRUCT(BlueprintType)
struct FActionCard { // ī������
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EActionType Type = EActionType::Defend;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EDir4 Dir = EDir4::None; // Attack/Move���� ���
};

// ===== ��������Ʈ (UI��) =====
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, ECombatPhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitMoved, bool, bIsPlayer, FPos, NewPos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHit, bool, bAttackerIsPlayer, bool, bDefenderIsPlayer, int32, DefenderNewHP);

UCLASS()
class PROJECT_API ACombatManager : public AActor
{
    GENERATED_BODY()

public:
    ACombatManager();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // === ���� ===
    UFUNCTION(BlueprintCallable)
    void Setup(); // �ʱ�ȭ �Լ�, ���� ���Խÿ� ȣ��

    void ResetTurn(); // �� �ʱ�ȭ.

    void InitBoard(int32 InSizeX = 4, int32 InSizeY = 3);
    // === �б� ===
    UFUNCTION(BlueprintPure) ECombatPhase GetPhase() const { return Phase; }
    UFUNCTION(BlueprintPure) int32 GetBoardSizeX() const { return BoardSizeX; }
    UFUNCTION(BlueprintPure) int32 GetBoardSizeY() const { return BoardSizeY; }

    // === �̺�Ʈ(���߿� UI���� ���ε�) ===
    UPROPERTY(BlueprintAssignable) FOnPhaseChanged OnPhaseChanged;
    UPROPERTY(BlueprintAssignable) FOnUnitMoved   OnUnitMoved;
    UPROPERTY(BlueprintAssignable) FOnHit         OnHit;

    // ī�� ���� ������ ----------------------------------------
    UFUNCTION(BlueprintCallable)
    void PushCard(uint8 cardnum, uint8 dir);

private:
    // �����Ͱ��� -> �ϸ��� �ʱ�ȭ����� ��
    FActionCard PlayerCards[MAX_CARDS];
    FActionCard EnemyCards[MAX_CARDS];
    TArray<FActionCard> Deck; // �̹� �ߵ��������
    int CurCardNum = 0; // ����� ī�� ����
    int CurDeckIdx = 0; // �ߵ��� ��

    void SetDeck();

    void ActiveAction();

    // �ּ� ����(���� ũ��/�����). ����/�ε�ƿ��� ���� �ܰ迡�� �߰�.
    UPROPERTY(EditAnywhere, Category = "Board") int32 BoardSizeX = 4;
    UPROPERTY(EditAnywhere, Category = "Board") int32 BoardSizeY = 3;

    UPROPERTY(VisibleAnywhere, Category = "State") ECombatPhase Phase = ECombatPhase::Prepare;
};

