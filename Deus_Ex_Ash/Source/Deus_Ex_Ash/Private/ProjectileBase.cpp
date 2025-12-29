// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "CharacterBase.h"
#include "WeaponBase.h"
#include "AS_CharacterAttributes.h"
#include "AbilitySystemUtility.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 600.0f;
	ProjectileMovementComponent->MaxSpeed = 600.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void AProjectileBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AProjectileBase::OnHit(AActor* HitActor, AActor* ProjectileActor, float Damage, float DurabilityDecrease, float HitDuration, float StaggerDuration, bool IgnoreGuard)
{
	AActor* ProjectileOwner = GetOwner();
	AWeaponBase* ProjectileOwnerWeaponBase = Cast<AWeaponBase>(ProjectileOwner); // 투사체 소유자 = 무기
	ACharacterBase* ProjectileOwnerCharacterBase = Cast<ACharacterBase>(ProjectileOwner); // 투사체 소유자 = 캐릭터

	ACharacterBase* HitCharacterBase = Cast<ACharacterBase>(HitActor);

	ACharacterBase* OwnerCharacterBase = nullptr;
	// 투사체 소유자가 무기인 경우
	if (ProjectileOwnerWeaponBase)
	{
		AActor* WeaponOwner = ProjectileOwner->GetOwner();
		OwnerCharacterBase = Cast<ACharacterBase>(WeaponOwner);
	}
	// 투사체 소유자가 캐릭터인 경우
	else if (ProjectileOwnerCharacterBase)
	{
		OwnerCharacterBase = Cast<ACharacterBase>(ProjectileOwner);
	}

	if (ProjectileOwner && OwnerCharacterBase && HitCharacterBase)
	{
		// 맞은 캐릭터 데미지 적용
		HitCharacterBase->ApplyDamage(OwnerCharacterBase, ProjectileActor, Damage, HitDuration, StaggerDuration, IgnoreGuard);

		// 가드 리게인 태그 보유중이면
		if (OwnerCharacterBase->AbilitySystemComponent->HasMatchingGameplayTag(FAbilitySystemUtility::GuardRegainTag))
		{
			const float CurrentHealth = OwnerCharacterBase->AS_CharacterAttributes->GetHealth();
			const float TempHealth = OwnerCharacterBase->AS_CharacterAttributes->GetTempHealth();

			// 현재 체력이 임시 체력보다 작으면
			if (CurrentHealth < TempHealth && !FMath::IsNearlyEqual(CurrentHealth, TempHealth, 1.0E-4F))
			{
				// 체력 추가
				AWeaponBase* EquippedWeaponBase = Cast<AWeaponBase>(OwnerCharacterBase->EquippedWeapon);
				float HealthAddAmount = FMath::Clamp(TempHealth - CurrentHealth, 0.0f, EquippedWeaponBase->GuardRegainHealAmount);
				FAbilitySystemUtility::Get().ApplyGameplayEffect(OwnerCharacterBase->AbilitySystemComponent, OwnerCharacterBase->AbilitySystemComponent, OwnerCharacterBase->GE_Health, 0.0f, FAbilitySystemUtility::StatHealthDataTag, HealthAddAmount);
			}
		}
	}
	else
	{
		FString str = FString::Format(
			TEXT(
				"[ WeaponOwner ] : [ {0} ]\n"
				"[ OwnerCharacterBase ] : [ {1} ]\n"
				"[ HitCharacterBase ] : [ {2} ]"
			),
			{
				ProjectileOwner == nullptr ? TEXT("Null") : TEXT("Not Null"),
				OwnerCharacterBase == nullptr ? TEXT("Null") : TEXT("Not Null"),
				HitCharacterBase == nullptr ? TEXT("Null") : TEXT("Not Null")
			}
		);

		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Orange, str);
		UE_LOG(LogTemp, Log, TEXT("%s"), *str);
	}
}

void AProjectileBase::SetFireDirection(const FVector& Direction)
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}