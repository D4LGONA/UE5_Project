// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "AbilitySystemUtility.h"
#include "CharacterBase.h"


UPlayerBase::UPlayerBase()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UPlayerBase::BeginPlay()
{
	Super::BeginPlay();

}

//void UPlayerBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//}

void UPlayerBase::Die()
{
	AActor* PlayerActor = GetOwner();
	ACharacterBase* PlayerCharacterBase = Cast<ACharacterBase>(PlayerActor);

	if (PlayerActor && PlayerCharacterBase && PlayerCharacterBase->AbilitySystemComponent)
	{
		FAbilitySystemUtility::Get().SendEventTag(FAbilitySystemUtility::DieTag, PlayerActor, PlayerActor, 0.0f, PlayerCharacterBase->AbilitySystemComponent);
	}
}