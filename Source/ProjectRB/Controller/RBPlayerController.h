// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RBPlayerController.generated.h"

// 전방선언
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 캐릭터를 조작하는 컨트롤러의 기본 클래스
 * 초기 기획 :
 * 조작해야 할 캐릭터가 총 4개까지이기 때문에
 * Input Binding 을 컨트롤러에서 진행한다.
 */
UCLASS()
class PROJECTRB_API ARBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void OnPossess(APawn* aPawn) override;

	virtual void SetupInputComponent() override;
	
private:
	// Input Mapping
	UPROPERTY(EditDefaultsOnly, Category = "InputMappingContext")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> ZoomAction;

	// 걷기/달리기 변경 키 (향후 이동/스프린트 로 변경 가능성 있음)
	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> ChangeRunModeAction;

	UPROPERTY(EditDefaultsOnly, Category = "InputAction")
	TObjectPtr<UInputAction> FieldAttackAction;

private:
	void SetCharacterEnhancedInput();

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
	void Jump();
	void StopJumping();
	void ChangeRunMode();
	void FieldAttack();
};
