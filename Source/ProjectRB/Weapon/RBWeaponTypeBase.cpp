// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RBWeaponTypeBase.h"

#include "NiagaraComponent.h"
//#include "NiagaraFunctionLibrary.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Sets default values
ARBWeaponTypeBase::ARBWeaponTypeBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	// 나이아가라 컴포넌트 SET UP
	WeaponSummonEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Summon Effect"));
	WeaponSummonEffect->SetupAttachment(WeaponMesh);

	UE_LOG(LogTemp, Display, TEXT("[%s] MPC Loading ..."), *GetActorNameOrLabel());
	// MPC 불러오기
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> SummonEffectParameterRef(TEXT("/Game/ProjectRB/Weapon/Pipe/Material/MPC_Pipe_Edge.MPC_Pipe_Edge"));
	if (SummonEffectParameterRef.Object)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] MPC Loaded !!!"), *GetActorNameOrLabel());
		SummonEffectParameter = SummonEffectParameterRef.Object;
		UE_LOG(LogTemp, Display, TEXT("[%s] MPC Fully Loaded : %s !!!"), *GetActorNameOrLabel(), *SummonEffectParameter->GetFName().ToString());
	}

	PrimaryActorTick.bCanEverTick = true;
}

void ARBWeaponTypeBase::BeginPlay()
{
	UE_LOG(LogTemp, Display, TEXT("[%s] BeginPlay !!!"), *GetActorNameOrLabel());
	if (SummonEffectParameter)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] MPC Trying to Set !!!"), *GetActorNameOrLabel());
		UMaterialParameterCollectionInstance* SummonEffectPci = GetWorld()->GetParameterCollectionInstance(SummonEffectParameter);
		if (SummonEffectPci)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s] MPC Set !!!"), *GetActorNameOrLabel());
			SummonEffectPci->SetScalarParameterValue(TEXT("Mask"), 0.f);
		}

		if (WeaponSummonEffect)
		{
			UE_LOG(LogTemp, Display, TEXT("[%s] Set Niagara Mask User Exposed Parameter Set !!!"), *GetActorNameOrLabel());
			WeaponSummonEffect->SetNiagaraVariableFloat(TEXT("User.ParticleMask"), 1.f);
			WeaponSummonEffect->SetNiagaraVariableFloat(TEXT("User.MeshMask"), 1.f);
		}
	}
}

void ARBWeaponTypeBase::Tick(float DeltaSeconds)
{

}

void ARBWeaponTypeBase::PerformWeaponFieldAttack()
{

}

void ARBWeaponTypeBase::PerformWeaponSummonEffect()
{
	UE_LOG(LogTemp, Display, TEXT("Summon"));

	UMaterialParameterCollectionInstance* SummonEffectPci = GetWorld()->GetParameterCollectionInstance(SummonEffectParameter);
	if (SummonEffectPci)
	{
		//SummonEffectPci->SetScalarParameterValue(TEXT("Mask"), FMath::FInterpTo(0.f, 1.f, GetWorld()->GetDeltaSeconds(), WeaponSummonSpeed));
		SummonEffectPci->SetScalarParameterValue(TEXT("Mask"), 1.f);
	}

	if (WeaponSummonEffect)
	{
		WeaponSummonEffect->SetNiagaraVariableFloat(TEXT("User.ParticleMask"), FMath::FInterpTo(1.f, 0.f, GetWorld()->GetDeltaSeconds(), WeaponSummonSpeed));
		WeaponSummonEffect->SetNiagaraVariableFloat(TEXT("User.MeshMask"), FMath::FInterpTo(0.f, 1.f, GetWorld()->GetDeltaSeconds(), WeaponSummonSpeed));
	}
}

void ARBWeaponTypeBase::PerformWeaponDisappearEffect()
{
	UE_LOG(LogTemp, Display, TEXT("Disappear"));

	UMaterialParameterCollectionInstance* SummonEffectPci = GetWorld()->GetParameterCollectionInstance(SummonEffectParameter);
	if (SummonEffectPci)
	{
		//SummonEffectPci->SetScalarParameterValue(TEXT("Mask"), FMath::FInterpTo(1.f, 0.f, GetWorld()->GetDeltaSeconds(), WeaponSummonSpeed));
		SummonEffectPci->SetScalarParameterValue(TEXT("Mask"), 0.f);
	}

	if (WeaponSummonEffect)
	{
		WeaponSummonEffect->SetNiagaraVariableFloat(TEXT("User.ParticleMask"), FMath::FInterpTo(1.f, 0.f, GetWorld()->GetDeltaSeconds(), WeaponSummonSpeed));
		WeaponSummonEffect->SetNiagaraVariableFloat(TEXT("User.MeshMask"), FMath::FInterpTo(1.f, 0.f, GetWorld()->GetDeltaSeconds(), WeaponSummonSpeed));
	}
}
