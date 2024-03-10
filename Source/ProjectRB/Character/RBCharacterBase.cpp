// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RBCharacterBase.h"
#include "Components/CapsuleComponent.h"

#include "CharacterStat/RBCharacterStatComponent.h"
#include "Weapon/RBWeaponTypeBase.h"

#include "Engine/DamageEvents.h"
#include "Action/DamageType_FieldAttack.h"

#include "Physics/RBCollision.h"


ARBCharacterBase::ARBCharacterBase()
{
	// Collision Profile Setting For Field Attack Hit Check
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_RBCharacterCapsule);
	GetMesh()->SetCollisionProfileName(CPROFILE_NOCOLLISION);

	Stat = CreateDefaultSubobject<URBCharacterStatComponent>(TEXT("Character Stat Component"));

	bIsProcessingFieldAttack = false;
	bCanInterruptFieldAttack = false;
}

// 공격에 대한 대미지 처리 (Field Attack, In Battle Attack)
float ARBCharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Display, TEXT("Take Damage"));
	if (UDamageType_FieldAttack* FieldAttackDamageType = Cast<UDamageType_FieldAttack>(DamageEvent.DamageTypeClass->GetDefaultObject()))
	{
		UE_LOG(LogTemp, Display, TEXT("Take Field Attack Damage"));
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && FieldAttackHitMontage)
		{	
			UE_LOG(LogTemp, Display, TEXT("Play Field Hit Montage"));
			// 피격 몽타주를 플레이하고, 특정 시점(혹은 종료 시점)에 전투가 진입되도록 Notify 를 사용해서 구현하면 될듯
			AnimInstance->Montage_Play(FieldAttackHitMontage);
		}
	}

	return 0.0f;
}

void ARBCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetWeapon();
}

void ARBCharacterBase::SetWeapon()
{
	if (WeaponClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("There is no Weapon Class at %s"), *GetActorNameOrLabel());
		return;
	}

	Weapon = GetWorld()->SpawnActor<ARBWeaponTypeBase>(WeaponClass);
	Weapon->SetOwner(this);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
}

void ARBCharacterBase::FieldAttack()
{
	if (FieldAttackMontage == nullptr)
	{
		return;
	}

	bCanInterruptFieldAttack = false;
	bIsProcessingFieldAttack = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 몽타주 재생
		AnimInstance->Montage_Play(FieldAttackMontage, 1.0f);

		// 몽타주 종료 콜백 델리게이트
		FOnMontageEnded FieldAttackEndDelegate;
		FieldAttackEndDelegate.BindUObject(this, &ARBCharacterBase::FieldAttackEnded);
		AnimInstance->Montage_SetEndDelegate(FieldAttackEndDelegate, FieldAttackMontage);
	}
}

void ARBCharacterBase::IsFieldAttackHit()
{
	UE_LOG(LogTemp, Display, TEXT("Field Attack Hit Check"));

	// Trace 쿼리 세팅을 Field Attack 태그로, 또한 자기 자신을 제외하는 것으로 지정
	FCollisionQueryParams FieldAttackCollisionQueryParams(SCENE_QUERY_STAT(FieldAttack), false, this);

	// 공격 범위 계산
	const FVector StartLocation = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	DrawDebugSphere(GetWorld(), StartLocation, 10.f, 24, FColor::Blue, true);
	const FVector EndLocation = StartLocation + GetActorForwardVector() * Stat->GetFieldAttackRange();
	DrawDebugSphere(GetWorld(), EndLocation, 10.f, 24, FColor::Green, true);

	// 공격 판정 Sweep Trace
	// Destructable Objects 추가하게 되면 나중에 Multi 로 바꿔야 할듯?
	FHitResult FieldAttackHitResult;
	bool IsHit = GetWorld()->SweepSingleByChannel(FieldAttackHitResult, StartLocation, EndLocation, FQuat::Identity, CCHANNEL_RBCharacterCapsule, FCollisionShape::MakeSphere(Stat->GetFieldAttackRadius()), FieldAttackCollisionQueryParams);
	if (!IsHit)
	{
		return;
	}

	if (AActor* HitActor = FieldAttackHitResult.GetActor())
	{
		UE_LOG(LogTemp, Display, TEXT("Field Attack Hit Someone : %s"), *HitActor->GetActorNameOrLabel());
		DrawDebugSphere(GetWorld(), FieldAttackHitResult.ImpactPoint, 10.f, 24, FColor::Red, true);

		// 필드에서 피격 시의 이벤트
		FDamageEvent FieldDamageEvent(UDamageType_FieldAttack::StaticClass());
		HitActor->TakeDamage(0.f, FieldDamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector DebugCapsuleCenter = StartLocation + (EndLocation - StartLocation) * 0.5f;
	DrawDebugCapsule(GetWorld(), DebugCapsuleCenter, Stat->GetFieldAttackRange() * 0.5f, Stat->GetFieldAttackRadius(), FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), IsHit ? FColor::Green : FColor::Red, true);

#endif
}

// 몽타주 재생 중 특정 타이밍부터는 모션을 끊고 이동 가능하도록 구현
void ARBCharacterBase::InterruptFieldAttack()
{
	UE_LOG(LogTemp, Display, TEXT("Can Interrupt Field Attack"));
	bCanInterruptFieldAttack = true;
}

// 캐릭터 필드 공격 몽타주 종료 콜백
// 지금으로써는 딱히 필요 있나 싶음
void ARBCharacterBase::FieldAttackEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UE_LOG(LogTemp, Display, TEXT("End Field Attack"));
	bIsProcessingFieldAttack = false;
}
