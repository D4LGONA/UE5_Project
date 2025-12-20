// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "AbilitySystemUtility.h"


UEnemyBase::UEnemyBase()
{

}

void UEnemyBase::BeginPlay()
{
	Super::BeginPlay();

}

//void UEnemyBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//}

void UEnemyBase::Die()
{
	GetOwner()->Destroy();
}