// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/RBWeaponFieldAttackInterface.h"
#include "RBWeaponTypeBase.generated.h"

// 무기 타입 열거 클래스
UENUM()
enum class WEAPON_TYPE : uint8
{
	NONE = 0,
	OHSWORD,
	THSWORD,
	GUN,
	MAGIC
};

/**
 * 무기 타입에 대한 기초 클래스. 모든 무기는 이 클래스의 하위 클래스를 상속받아야 한다.
 */
UCLASS()
class PROJECTRB_API ARBWeaponTypeBase : public AActor, public IRBWeaponFieldAttackInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARBWeaponTypeBase();

protected:
	UPROPERTY(VisibleInstanceOnly, Category = "Weapon Mesh")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Effect")
	TSoftObjectPtr<UParticleSystem> WeaponAttackEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Effect")
	TSoftObjectPtr<USoundBase> WeaponAttackSound;

	// 해당 무기가 적중했을 때의 피격 이펙트. 상대 캐릭터가 아닌 무기의 멤버로 존재하는것이 낫다고 판단함.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Effect")
	TSoftObjectPtr<UParticleSystem> WeaponHitEffect;

public:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Type")
	WEAPON_TYPE WeaponType;
	
	// 각각의 무기에 맞는 공격 방식 수행
	virtual void PerformWeaponFieldAttack() override;
};
