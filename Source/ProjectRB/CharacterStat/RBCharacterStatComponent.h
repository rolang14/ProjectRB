// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/RBCharacterStat.h"
#include "RBCharacterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRB_API URBCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URBCharacterStatComponent();

	// Stat Getter & Setter
	FORCEINLINE const FRBCharacterStat GetTotalStat() const { return BaseStat + AdditionalStat; }

	FORCEINLINE const float& GetFieldAttackRadius() const { return FieldAttackRadius; }
	FORCEINLINE const float& GetFieldAttackRange() const { return FieldAttackRange; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Character Stat")
	float CurrentHp;

	UPROPERTY(EditDefaultsOnly, Category = "Character Stat")
	float FieldAttackRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Character Stat")
	float FieldAttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "Character Stat")
	float FeverAttackRange;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Character Stat")
	FRBCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Character Stat")
	FRBCharacterStat AdditionalStat;
};
