// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RBCharacterBase.h"
#include "Interface/RBCharacterMovementInterface.h"
#include "RBCharacterPlayer.generated.h"

/**
 * 플레이어가 조작할 수 있는 캐릭터의 기본 클래스
 */
UCLASS()
class PROJECTRB_API ARBCharacterPlayer : public ARBCharacterBase, public IRBCharacterMovementInterface
{
	GENERATED_BODY()
	
public:
	ARBCharacterPlayer();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Character Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Character Camera")
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Character Control Data")
	TObjectPtr<class URBCharacterControlData> CharacterControlData;

	// Settings for Camera Movement
	UPROPERTY(EditAnywhere, Category = "Camera")
	float RotateSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float TargetArmMaxLength = 600.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float TargetArmMinLength = 100.f;

	// 줌할때 얼굴이 잘 안보여서 하는 설정
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraHeight = 30.f;

	bool IsRunMode = true;

private:
	void SetCharacterControlData();

	virtual void InputActionLook(const struct FInputActionValue& Value) override;
	virtual void InputActionMove(const struct FInputActionValue& Value) override;
	virtual void InputActionZoom(const struct FInputActionValue& Value) override;
	virtual void InputActionJump() override;
	virtual void InputActionStopJumping() override;
	virtual void InputActionChangeRunMode() override;
	virtual void InputActionFieldAttack() override;

	// Character Move Functions
	void ProcessCharacterMove(const float& MoveForwardValue, const float& MoveRightValue);

	// Camera Zoom Functions
	void SetNextZoom(const float ZoomValue);
	void PerformSmoothZoom();

	float NextZoom;

	// Field Attack Functions
	void ProcessFieldAttack();
};
