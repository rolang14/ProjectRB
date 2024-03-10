// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RBWeaponTypeBase.h"

// Sets default values
ARBWeaponTypeBase::ARBWeaponTypeBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponType = WEAPON_TYPE::NONE;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
}

void ARBWeaponTypeBase::PerformWeaponFieldAttack()
{
}
