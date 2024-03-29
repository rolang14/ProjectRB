// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/RBCharacterFieldAttackInterface.h"
#include "RBCharacterBase.generated.h"

// 전방선언
class ARBWeaponTypeBase;
class URBCharacterStatComponent;
class UAnimMontage;

UCLASS()
class PROJECTRB_API ARBCharacterBase : public ACharacter, public IRBCharacterFieldAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARBCharacterBase();

// Get Character's Status
public:
	// 캐릭터의 무기 정보를 취득
	FORCEINLINE const TObjectPtr<ARBWeaponTypeBase> GetCharacterWeapon() const { return Weapon; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

// Character Stat Section
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Character Stat", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URBCharacterStatComponent> Stat;

// Field Attack Section
protected:
	// 필드 어택 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "Character Motion")
	TObjectPtr<UAnimMontage> FieldAttackMontage;

	// 필드 어택 피격 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "Character Motion")
	TObjectPtr<UAnimMontage> FieldAttackHitMontage;

	// Perform Field Attack
	void FieldAttack();

	FORCEINLINE const bool& IsProcessingFieldAttack() const { return bIsProcessingFieldAttack; }
	FORCEINLINE const bool& CanInterruptFieldAttack() const { return bCanInterruptFieldAttack; }

private:
	// Weapon Blueprint 받기 용도
	UPROPERTY(EditDefaultsOnly, Category = "Character Weapon")
	TSubclassOf<ARBWeaponTypeBase> WeaponClass;

	// C++ Weapon System 사용을 위한 용도
	UPROPERTY()
	TObjectPtr<ARBWeaponTypeBase> Weapon;

	// BP로 설정한 무기를 캐릭터에 셋업
	void SetWeapon();

	// For Field Attack Interrupt and End Delegate
	virtual void IsFieldAttackHit() override;
	virtual void InterruptFieldAttack() override;
	void FieldAttackEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	bool bIsProcessingFieldAttack;
	bool bCanInterruptFieldAttack;
};
