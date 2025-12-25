// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "WeaponBase.h"
#include "AS_CharacterAttributes.h"
#include "AS_WeaponAttributes.h"
#include "AbilitySystemUtility.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// ASC 추가
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Attribute Set 추가
	AS_CharacterAttributes = CreateDefaultSubobject<UAS_CharacterAttributes>(TEXT("AS_CharacterAttributes"));

	CharacterType = ECharacterType::None;

	EquippedWeapon = nullptr;

	MinStaminaRequired = 5.0f;

	//GroggyTime_PlayerNoStaminaAndEnemyGuarded = 3.0f;

	GroggyTime_PlayerJustGuardedAndNoStamina = 3.0f;

	StaminaAdd_JustGuard = -2.0f;
	StaminaAdd_PerfectGuard = -1.0f;

	WeaponDurabilityAdd_JustGuard = -1.0f;
	WeaponDurabilityAdd_PerfectGuard = 1.0f;

	AttackerStaminaAdd_PerfectGuard = -1.0f;
	AttackerWeaponDurabilityAdd_PerfectGuard = -1.0f;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every framec
//void ACharacterBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::ApplyDamage(AActor* AttackerCharacter, AActor* Projectile, float Damage, float StaggerDuration, bool IgnoreGuard)
{
	// 무적 태그 보유중이면 리턴
	if (AbilitySystemComponent->HasMatchingGameplayTag(FAbilitySystemUtility::InvincibleTag))
	{
		return;
	}

	AWeaponBase* EquippedWeaponBase = Cast<AWeaponBase>(EquippedWeapon);
	ACharacterBase*	AttackerCharacterBase = Cast<ACharacterBase>(AttackerCharacter);
	AWeaponBase* AttackerWeaponBase	= Cast<AWeaponBase>(AttackerCharacterBase->EquippedWeapon);
	
	if (EquippedWeaponBase && AttackerCharacterBase && AttackerWeaponBase)
	{
		//USkeletalMeshComponent* SkeletalMesh = GetMesh();
		//FTransform RootTransform = SkeletalMesh->GetSocketTransform(FName("root"), RTS_World);
		FTransform RootTransform = GetActorTransform();

		//FVector2D ForwardUnitVector2D = (FVector2D)RootTransform.GetUnitAxis(EAxis::Y); //캐릭터 루트 정면 벡터
		FVector2D ForwardUnitVector2D = (FVector2D)RootTransform.GetUnitAxis(EAxis::X);
		FVector2D TargetUnitVector2D;
		if (Projectile)
		{
			TargetUnitVector2D = ((FVector2D)(Projectile->GetActorLocation() - RootTransform.GetLocation())).GetSafeNormal();
		}
		else
		{
			TargetUnitVector2D = ((FVector2D)(AttackerCharacter->GetActorLocation() - RootTransform.GetLocation())).GetSafeNormal();
		}

		float Dot = FVector2D::DotProduct(ForwardUnitVector2D, TargetUnitVector2D);
		float CrossZ = FVector2D::CrossProduct(ForwardUnitVector2D, TargetUnitVector2D);

		ECharacterDirection HitDirection = ECharacterDirection::Forward;
		if (Dot >= FMath::Cos(PI / 4))
		{
			HitDirection = ECharacterDirection::Forward;
		}
		else if (Dot <= FMath::Cos(PI * 3 / 4))
		{
			HitDirection = ECharacterDirection::Backward;
		}
		else if (Dot < FMath::Cos(PI / 4) && Dot > FMath::Cos(PI * 3 / 4))
		{
			if (CrossZ < 0)
			{
				HitDirection = ECharacterDirection::Leftward;
			}
			else if (CrossZ > 0)
			{
				HitDirection = ECharacterDirection::Rightward;
			}
		}
		else
		{
			// ERROR
		}
		
		//float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(Dot));
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Format(TEXT("{0}도"), { AngleDegrees }));

		// 가드 중이고, 공격자 액터 위치가 전방 180도에 포함되고, 가드 무시공격이 아닌 경우
		if (AbilitySystemComponent->HasMatchingGameplayTag(FAbilitySystemUtility::GuardingTag) && Dot >= 0)
		{
			// 퍼펙트 가드
			if (AbilitySystemComponent->HasMatchingGameplayTag(FAbilitySystemUtility::PerfectGuardingTag))
			{
				// 퍼펙트 가드 성공 태그 전송
				FAbilitySystemUtility::Get().SendEventTag(FAbilitySystemUtility::PerfectGuardSuccessTag, this, this, AbilitySystemComponent);

				// 스태미나 감소
				FAbilitySystemUtility::Get().ApplyGameplayEffect(AttackerCharacterBase->AbilitySystemComponent, AbilitySystemComponent, GE_Stamina_Instant, 0.0f, FAbilitySystemUtility::StatStaminaInstantDataTag, StaminaAdd_PerfectGuard);

				// 무기 내구도 회복
				FAbilitySystemUtility::Get().ApplyGameplayEffect(AttackerCharacterBase->AbilitySystemComponent, EquippedWeaponBase->AbilitySystemComponent, EquippedWeaponBase->GE_WeaponDurability_Instant, 0.0f, FAbilitySystemUtility::WeaponDurabilityInstantDataTag, WeaponDurabilityAdd_PerfectGuard);

				if (!Projectile)
				{
					// 상대 ParryPushBack 태그 전송
					FAbilitySystemUtility::Get().SendEventTag(FAbilitySystemUtility::ParryPushBackTag, this, AttackerCharacter, AttackerCharacterBase->AbilitySystemComponent);

					// 상대 스태미나 감소
					FAbilitySystemUtility::Get().ApplyGameplayEffect(AbilitySystemComponent, AttackerCharacterBase->AbilitySystemComponent, AttackerCharacterBase->GE_Stamina_Instant, 0.0f, FAbilitySystemUtility::StatStaminaInstantDataTag, AttackerStaminaAdd_PerfectGuard);

					// 상대 무기 내구도 감소
					FAbilitySystemUtility::Get().ApplyGameplayEffect(AbilitySystemComponent, AttackerWeaponBase->AbilitySystemComponent, AttackerWeaponBase->GE_WeaponDurability_Instant, 0.0f, FAbilitySystemUtility::WeaponDurabilityInstantDataTag, AttackerWeaponDurabilityAdd_PerfectGuard);
				}
			}
			else
			{
				// 가드 무시 공격이면
				if (IgnoreGuard)
				{
					Hit(AttackerCharacterBase, HitDirection, Damage, StaggerDuration, true);
				}
				// 저스트 가드
				else
				{
					// 스태미나 있으면
					if (AS_CharacterAttributes->GetStamina() > 1.0E-4F)
					{
						// 저스트 가드 성공 태그 전송
						FAbilitySystemUtility::Get().SendEventTag(FAbilitySystemUtility::GuardSuccessTag, this, this, AbilitySystemComponent);

						// 무기 내구도 감소
						FAbilitySystemUtility::Get().ApplyGameplayEffect(AttackerCharacterBase->AbilitySystemComponent, EquippedWeaponBase->AbilitySystemComponent, EquippedWeaponBase->GE_WeaponDurability_Instant, 0.0f, FAbilitySystemUtility::WeaponDurabilityInstantDataTag, WeaponDurabilityAdd_JustGuard);

						// 스태미나 감소
						FAbilitySystemUtility::Get().ApplyGameplayEffect(AttackerCharacterBase->AbilitySystemComponent, AbilitySystemComponent, GE_Stamina_Instant, 0.0f, FAbilitySystemUtility::StatStaminaInstantDataTag, StaminaAdd_JustGuard);

						// 플레이어 저스트 가드 했는데 스태미나 0이면 그로기
						if (CharacterType == ECharacterType::Player)
						{
							if (FMath::IsNearlyZero(AS_CharacterAttributes->GetStamina(), 1.0E-4F))
							{
								FAbilitySystemUtility::Get().SendEventTag(FAbilitySystemUtility::GroggyTag, this, this, GroggyTime_PlayerJustGuardedAndNoStamina, AbilitySystemComponent);
							}
						}
					}
					else
					{
						// 피격
						Hit(AttackerCharacterBase, HitDirection, Damage, StaggerDuration, false);
					}
					float CurrentDurabilityRatio = EquippedWeaponBase->AS_WeaponAttributes->GetDurability() / EquippedWeaponBase->AS_WeaponAttributes->GetMaxDurability();

					// 무기 내구도 < 30%, 플레이어만 가드 리게인 사용
					if (CurrentDurabilityRatio < 0.3f && CharacterType == ECharacterType::Player)
					{
						// 받은 데미지 * 0.7만큼 체력 감소
						float ReducedDamage = Damage * 0.7f;
						FAbilitySystemUtility::Get().ApplyGameplayEffect(AttackerWeaponBase->AbilitySystemComponent, AbilitySystemComponent, GE_Health, 0.0f, FAbilitySystemUtility::StatHealthDataTag, -ReducedDamage);

						// 가드 리게인 태그 없으면
						if (!AbilitySystemComponent->HasMatchingGameplayTag(FAbilitySystemUtility::GuardRegainTag))
						{
							// 감소된 데미지 * 0.6 만큼 임시 체력
							float TargetTempHealth = AS_CharacterAttributes->GetHealth() + ReducedDamage * 0.6f;

							// 가드 리게인 활성화
							SetGuardRegainActive(true, TargetTempHealth);

							// 10초 후 가드 리게인 비활성화
							GetWorld()->GetTimerManager().SetTimer(
								RemoveGuardRegainTimerHandle,
								[this]() {
									if (IsValid(AbilitySystemComponent))
									{
										SetGuardRegainActive(false, 0.0f);
									}
								},
								10.0f,
								false
							);
						}
					}

					// 상대가 플레이어고 스태미나 0이면 그로기 -> 보류
					/*if (AttackerCharacterBase->CharacterType == ECharacterType::Player)
					{
						if (FMath::IsNearlyZero(AttackerCharacterBase->AS_CharacterAttributes->GetStamina(), 1.0E-4F))
						{
							FAbilitySystemUtility::Get().SendEventTag(FAbilitySystemUtility::GroggyTag, this, Attacker, GroggyTime_PlayerNoStaminaAndEnemyGuarded, AttackerCharacterBase->AbilitySystemComponent);
						}
					}*/
				}
			}
		}
		// 클린 히트 or 그로기 상태
		else
		{
			// 피격
			Hit(AttackerCharacterBase, HitDirection, Damage, StaggerDuration, false);
		}
	}
	else
	{
		FString str = FString::Format(
			TEXT(
				"[ EquippedWeaponBase ] : [ {0} ]\n"
				"[ AttackerCharacterBase ] : [ {1} ]\n"
				"[ AttackerWeaponBase ] : [ {2} ]"
			),
			{
				EquippedWeaponBase == nullptr ? TEXT("Null") : TEXT("Not Null"),
				AttackerCharacterBase == nullptr ? TEXT("Null") : TEXT("Not Null"),
				AttackerWeaponBase == nullptr ? TEXT("Null") : TEXT("Not Null")
			}
		);

		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Orange, str);
		UE_LOG(LogTemp, Log, TEXT("%s"), *str);
	}
}

void ACharacterBase::Hit(ACharacterBase* AttackerCharacterBase, ECharacterDirection HitDirectionEnum, float Damage, float StaggerDuration, bool IgnoreGuard)
{
	// 그로기 상태
	if (AbilitySystemComponent->HasMatchingGameplayTag(FAbilitySystemUtility::GroggyTag))
	{
		// 그로기 데미지 배율
		Damage *= 2.5f;

		// 넉다운 태그 전송
		FAbilitySystemUtility::Get().SendEventTag(FAbilitySystemUtility::KnockdownTag, AttackerCharacterBase, this, (float)HitDirectionEnum, AbilitySystemComponent);
	}
	// 가드 브레이크 공격 피격
	else if (IgnoreGuard)
	{
		FAbilitySystemUtility::Get().SendEventTag(FAbilitySystemUtility::GuardBreakHitTag, AttackerCharacterBase, this, AbilitySystemComponent);
	}
	else
	{
		// 히트 태그 전송
		FAbilitySystemUtility::Get().SendEventTag(FAbilitySystemUtility::HitTag, AttackerCharacterBase, this, (float)HitDirectionEnum, AbilitySystemComponent);
	}

	// 가드 리게인 태그 보유중인 경우 데미지만큼 임시체력 감소
	if (AbilitySystemComponent->HasMatchingGameplayTag(FAbilitySystemUtility::GuardRegainTag))
	{
		FAbilitySystemUtility::Get().ApplyGameplayEffect(AttackerCharacterBase->AbilitySystemComponent, AbilitySystemComponent, GE_TempHealth_Instant, 0.0f, FAbilitySystemUtility::StatTempHealthInstantDataTag, -Damage);
	}

	// 데미지만큼 체력 감소
	FAbilitySystemUtility::Get().ApplyGameplayEffect(AttackerCharacterBase->AbilitySystemComponent, AbilitySystemComponent, GE_Health, 0.0f, FAbilitySystemUtility::StatHealthDataTag, -Damage);

	// 경직 적용 시간이 있는 공격을 받은 경우
	if (StaggerDuration > 0.0f && !FMath::IsNearlyZero(StaggerDuration, 1.0E-4F))
	{
		// 경직 적용
		FAbilitySystemUtility::Get().SendEventTag(FAbilitySystemUtility::StaggerTag, AttackerCharacterBase, this, StaggerDuration, AbilitySystemComponent);
	}
}

void ACharacterBase::SetGuardRegainActive(bool Active, float TargetTempHealth)
{
	if (Active)
	{
		// 가드 리게인 태그 부여
		AbilitySystemComponent->AddLooseGameplayTag(FAbilitySystemUtility::GuardRegainTag);

		// TargetTempHealth로 임시 체력 설정
		FAbilitySystemUtility::Get().ApplyGameplayEffect(AbilitySystemComponent, AbilitySystemComponent, GE_TempHealth_Override, 0.0f, FAbilitySystemUtility::StatTempHealthOverrideDataTag, TargetTempHealth);
	}
	else
	{
		// 가드 리게인 타이머 제거
		if (RemoveGuardRegainTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(RemoveGuardRegainTimerHandle);
		}

		// 가드 리게인 태그 제거
		AbilitySystemComponent->RemoveLooseGameplayTag(FAbilitySystemUtility::GuardRegainTag);

		// 임시 체력 0.0으로 설정
		FAbilitySystemUtility::Get().ApplyGameplayEffect(AbilitySystemComponent, AbilitySystemComponent, GE_TempHealth_Override, 0.0f, FAbilitySystemUtility::StatTempHealthOverrideDataTag, 0.0f);
	}
}

void ACharacterBase::Die_Implementation() {  }
