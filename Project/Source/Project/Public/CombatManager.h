// CombatManager.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatManager.generated.h"

constexpr short MAX_CARDS = 3;

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

// ===== 델리게이트 (UI용) =====
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, ECombatPhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDoAction, EActionType, Action, bool, bIsPlayer);
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

    // === 설정 ===
    UFUNCTION(BlueprintCallable)
    void Setup(); // 초기화 함수, 전투 진입시에 호출
    void ResetTurn(); // 턴 초기화.
    void InitBoard(int32 InSizeX = 4, int32 InSizeY = 3);

    // === 읽기 ===
    UFUNCTION(BlueprintPure) ECombatPhase GetPhase() const { return Phase; }
    UFUNCTION(BlueprintPure) int32 GetBoardSizeX() const { return BoardSizeX; }
    UFUNCTION(BlueprintPure) int32 GetBoardSizeY() const { return BoardSizeY; }

    // === 이벤트(나중에 UI에서 바인딩) ===
    UPROPERTY(BlueprintAssignable) FOnPhaseChanged OnPhaseChanged;
    UPROPERTY(BlueprintAssignable) FOnUnitMoved   OnUnitMoved;
    UPROPERTY(BlueprintAssignable) FOnHit         OnHit;

    // 카드 선택 페이즈 ----------------------------------------
    UFUNCTION(BlueprintCallable)
    void PushCard(uint8 cardnum, uint8 dir);

private:
    // 데이터관리 -> 턴마다 초기화해줘야 함
    FActionCard PlayerCards[MAX_CARDS];
    FActionCard EnemyCards[MAX_CARDS];
    TArray<TPair<FActionCard, bool>> Deck; // 이번 발동순서대로
    int CurCardNum = 0; // 등록한 카드 개수
    int CurDeckIdx = 0; // 발동할 것

	FPos PlayerPos;
	FPos EnemyPos;

    // 방어 상태 & 소비 여부
    bool bPlayerBlocking = false;
    bool bEnemyBlocking = false;
    bool bPlayerBlockConsumed = false; // 해당 턴에 방어로 공격을 막았는지
    bool bEnemyBlockConsumed = false;


    void SetDeck();

    void ActiveAction();

    // 최소 상태(보드 크기/페이즈만). 유닛/로드아웃은 다음 단계에서 추가.
    UPROPERTY(EditAnywhere, Category = "Board") int32 BoardSizeX = 4;
    UPROPERTY(EditAnywhere, Category = "Board") int32 BoardSizeY = 3;

    UPROPERTY(VisibleAnywhere, Category = "State") ECombatPhase Phase = ECombatPhase::Prepare;
};

