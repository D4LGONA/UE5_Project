// CombatManager.h
#pragma once

#include "CoreMinimal.h"
#include "Spine_EntityBase.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "CombatManager.generated.h"

constexpr short MAX_CARDS = 3;
constexpr int32 BOARDWIDTH = 4;
constexpr int32 BOARDHEIGHT = 3;

class AActor; // 플레이어/적 액터 타입 자유롭게

// 선택페이즈 -> 액션페이즈 -> 승리/패배
UENUM(BlueprintType)
enum class ECombatPhase : uint8 { Prepare, Action, Victory, Defeat };

UENUM(BlueprintType) // 행동 타입
enum class EActionType : uint8 { Defend = 0, Attack = 1, Move = 2 };

UENUM(BlueprintType) // 이동 방향(None: Attack/Defend에서 사용)
enum class EDir4 : uint8 { None = 0, Up, Down, Left, Right };

USTRUCT(BlueprintType)
struct FPos { // 위치 저장
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 X = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Y = 0;
    bool operator==(const FPos& O) const { return X == O.X && Y == O.Y; }
};

USTRUCT(BlueprintType)
struct FActionCard { // 카드정보
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EActionType Type = EActionType::Defend;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EDir4 Dir = EDir4::None; // Attack/Move에서 사용
};

// --- delegates ---
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnitMoved);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHPChanged, bool, bIsPlayer, int32, OldHP, int32, NewHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnImageChange, bool, bIsPlayer, EActionType, Action);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, ECombatPhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackedPos, const TArray<FPos>&, Positions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatEnded, bool, bPlayerWin);



UCLASS()
class DEUS_EX_ASH_API ACombatManager : public AActor
{
    GENERATED_BODY()

public:
    ACombatManager();

protected:
    virtual void BeginPlay() override;

public:
    // --- events (BlueprintAssignable) ---
    UPROPERTY(BlueprintAssignable) FOnUnitMoved    OnUnitMoved;
    UPROPERTY(BlueprintAssignable) FOnHPChanged    OnHPChanged;
    UPROPERTY(BlueprintAssignable) FOnPhaseChanged OnPhaseChanged;
    UPROPERTY(BlueprintAssignable) FOnCombatEnded  OnCombatEnded;
    UPROPERTY(BlueprintAssignable) FOnImageChange  OnImageChange;
    UPROPERTY(BlueprintAssignable) FOnAttackedPos  OnAttackedPos;


    // --- 각 페이즈 별 동작 ---
    void ActionPhase(); // 액션 페이즈 동작


    virtual void Tick(float DeltaTime) override;

    UPROPERTY() // 굳이 이렇게 무겁게 들고 있어야 할까 싶네..
    ASpine_EntityBase* PlayerPawn;

    UPROPERTY()
    AEnemy* EnemyPawn;


    // === 설정 ===
    UFUNCTION(BlueprintCallable)
    void Setup(ASpine_EntityBase* Player, AEnemy* Enemy); // 초기화 함수, 전투 진입시에 호출
    void ResetTurn(); // 턴 초기화.
    void InitBoard();

    // === 읽기 ===
    UFUNCTION(BlueprintPure) ECombatPhase GetPhase() const { return Phase; }

    // 카드 선택 페이즈 ----------------------------------------
    UFUNCTION(BlueprintCallable)
    void PushCard(EActionType cardnum, EDir4 dir);

    UFUNCTION(BlueprintCallable)
    TArray<FActionCard>& GetEnemyCard()  { return EnemyCards; };

    UFUNCTION(BlueprintCallable)
    void EnemySetup();

    UFUNCTION(BlueprintCallable)
    void StepAction();

private:

    TArray<FPos> AttackedPos;

    // 데이터관리 -> 턴마다 초기화해줘야 함
    TArray<FActionCard> PlayerCards;
    TArray<FActionCard> EnemyCards;
    TArray<TPair<FActionCard, bool>> Deck; // 이번 발동순서대로
    int CurCardNum = 0; // 등록한 카드 개수
    int CurDeckIdx = 0; // 발동할 것

	FPos PlayerPos;
	FPos EnemyPos;

    bool IsInsideBoard(const FIntPoint& P) const;
    void ApplyDamage(bool IsPlayer); // 딜넣는함수

    void SetDeck();

    void ActiveAction();

    void EntityMove(FPos& MyPos, FPos& OtherPos, FActionCard& Card);

    // 적 AI
 
    EDir4 CalcTutorialMoveDir() const;
    EDir4 CalcTutorialAttackDir() const;
    void TutorialAttack(bool IsPlayer);


    UPROPERTY(VisibleAnywhere, Category = "State") 
    ECombatPhase Phase = ECombatPhase::Prepare;

public: // getter - setter
    UFUNCTION(BlueprintCallable)
    FPos& GetPlayerPos() { return PlayerPos; }
    UFUNCTION(BlueprintCallable)
    FPos& GetEnemyPos() { return EnemyPos; }
};

