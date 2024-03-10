// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/RBPlayerController.h"
#include "Interface/RBCharacterMovementInterface.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ARBPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// Set Enhanced Input Context
	SetCharacterEnhancedInput();
}

// Enhanced Input Subsystem Setting
void ARBPlayerController::SetCharacterEnhancedInput()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMappingContext, 0);
		// PlayerController->SetPlayerControllerEnabled(true);
	}
}

// Enhanced Input Binding
void ARBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Move);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Zoom);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARBPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ARBPlayerController::StopJumping);

		EnhancedInputComponent->BindAction(ChangeRunModeAction, ETriggerEvent::Triggered, this, &ARBPlayerController::ChangeRunMode);

		EnhancedInputComponent->BindAction(FieldAttackAction, ETriggerEvent::Triggered, this, &ARBPlayerController::FieldAttack);
	}
}

#pragma region Input Actions

void ARBPlayerController::Look(const FInputActionValue& Value)
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionLook(Value);
	}
}

void ARBPlayerController::Move(const FInputActionValue& Value)
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionMove(Value);
	}
}

void ARBPlayerController::Zoom(const FInputActionValue& Value)
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionZoom(Value);
	}
}

void ARBPlayerController::Jump()
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionJump();
	}
}

void ARBPlayerController::StopJumping()
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionStopJumping();
	}
}

void ARBPlayerController::ChangeRunMode()
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionChangeRunMode();
	}
}

void ARBPlayerController::FieldAttack()
{
	APawn* PossessingPawn = GetPawn();
	if (PossessingPawn == nullptr)
	{
		return;
	}

	if (IRBCharacterMovementInterface* PawnMovement = Cast<IRBCharacterMovementInterface>(PossessingPawn))
	{
		PawnMovement->InputActionFieldAttack();
	}
}

#pragma endregion