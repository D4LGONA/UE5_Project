// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "CharacterBase.h"
#include "ProjectileBase.h"
#include "Components/CapsuleComponent.h"
#include "AS_WeaponAttributes.h"
#include "AS_CharacterAttributes.h"
#include "AbilitySystemUtility.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(GetRootComponent());

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AS_WeaponAttributes = CreateDefaultSubobject<UAS_WeaponAttributes>(TEXT("AS_WeaponAttributes"));

	WeaponType = EWeaponType::None;
	ComboIndex = 0;
	bIsBroken = false;
	GuardRegainHealAmount = 1.0f;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
//void AWeaponBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

UAbilitySystemComponent* AWeaponBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AWeaponBase::AddHitActor(AActor* OtherActor)
{
	if (OtherActor && !HitActors.Contains(OtherActor))
	{
		HitActors.Add(OtherActor);
	} 
}

bool AWeaponBase::ContainsHitActor(AActor* OtherActor)
{
	return HitActors.Contains(OtherActor);
}

void AWeaponBase::ResetHitActors()
{
	HitActors.Empty();
}


void AWeaponBase::OnAttackHit(AActor* HitActor, FVector ImpactPoint, float Damage, float DurabilityAddAmount, float StaggerDuration, bool IgnoreGuard)
{
	AActor* WeaponOwner = GetOwner();
	ACharacterBase* WeaponOwnerCharacterBase = Cast<ACharacterBase>(WeaponOwner);
	ACharacterBase* HitCharacterBase = Cast<ACharacterBase>(HitActor);

	if (WeaponOwner && WeaponOwnerCharacterBase && HitCharacterBase)
	{
		// 맞은 캐릭터 데미지 적용
		HitCharacterBase->ApplyDamage(WeaponOwner, nullptr, ImpactPoint, Damage, StaggerDuration, IgnoreGuard);

		// 무기 내구도 감소
		FAbilitySystemUtility::Get().ApplyGameplayEffect(AbilitySystemComponent, AbilitySystemComponent, GE_WeaponDurability_Instant, 0.0f, FAbilitySystemUtility::WeaponDurabilityInstantDataTag, DurabilityAddAmount);

		// 가드 리게인 태그 보유중이면
		if (WeaponOwnerCharacterBase->AbilitySystemComponent->HasMatchingGameplayTag(FAbilitySystemUtility::GuardRegainTag))
		{
			const float CurrentHealth = WeaponOwnerCharacterBase->AS_CharacterAttributes->GetHealth();
			const float TempHealth = WeaponOwnerCharacterBase->AS_CharacterAttributes->GetTempHealth();

			// 현재 체력이 임시 체력보다 작으면
			if (CurrentHealth < TempHealth && !FMath::IsNearlyEqual(CurrentHealth, TempHealth, 1.0E-4F))
			{
				// 체력 추가
				float HealthAddAmount = FMath::Clamp(TempHealth - CurrentHealth, 0.0f, GuardRegainHealAmount);
				FAbilitySystemUtility::Get().ApplyGameplayEffect(AbilitySystemComponent, WeaponOwnerCharacterBase->AbilitySystemComponent, WeaponOwnerCharacterBase->GE_Health, 0.0f, FAbilitySystemUtility::StatHealthDataTag, HealthAddAmount);
			}
		}
	}
	else
	{
		FString str = FString::Format(
			TEXT(
				"[ WeaponOwner ] : [ {0} ]\n"
				"[ WeaponOwnerCharacterBase ] : [ {1} ]\n"
				"[ HitCharacterBase ] : [ {2} ]"
			),
			{
				WeaponOwner == nullptr ? TEXT("Null") : TEXT("Not Null"),
				WeaponOwnerCharacterBase == nullptr ? TEXT("Null") : TEXT("Not Null"),
				HitCharacterBase == nullptr ? TEXT("Null") : TEXT("Not Null")
			}
		);

		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Orange, str);
		UE_LOG(LogTemp, Log, TEXT("%s"), *str);
	}
}