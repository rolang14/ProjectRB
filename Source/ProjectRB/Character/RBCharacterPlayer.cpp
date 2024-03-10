// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RBCharacterPlayer.h"
#include "Controller/RBPlayerController.h"

#include "InputActionValue.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Data/RBCharacterControlData.h"

ARBCharacterPlayer::ARBCharacterPlayer()
{
	// 스프링 암 및 카메라 컴포넌트 생성
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Character Camera Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Character Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ARBCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 캐릭터의 각종 컨트롤 데이터 세팅
	SetCharacterControlData();

	NextZoom = SpringArmComponent->TargetArmLength;
}

void ARBCharacterPlayer::Tick(float DeltaSeconds)
{
	// Smooth Zoom 수행
	if (SpringArmComponent->TargetArmLength != NextZoom)
	{
		PerformSmoothZoom();
	}
}

// 상세 캐릭터 컨트롤 설정
void ARBCharacterPlayer::SetCharacterControlData()
{
	if (CharacterControlData)
	{
		// Spring Arm이 Pawn의 컨트롤을 따라 회전한다. (False 인 경우 카메라는 Pawn의 위치를 고수하며, 컨트롤에 따라 회전하지 않음)
		SpringArmComponent->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
		SpringArmComponent->bInheritPitch = CharacterControlData->bInheritPitch;
		SpringArmComponent->bInheritYaw = CharacterControlData->bInheritYaw;
		SpringArmComponent->bInheritRoll = CharacterControlData->bInheritRoll;
		SpringArmComponent->TargetArmLength = CharacterControlData->TargetArmLength;

		// 캐릭터 키에 맞춰 스프링암 높이 세팅
		SpringArmComponent->SetRelativeLocation(FVector(0.f, 0.f, CameraHeight));

		// Pawn의 Yaw 회전이 Player Controller 의 Yaw 회전을 따라가게 한다.
		bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

		// bOrientRotationToMovement = 캐릭터를 가속도 방향으로 회전시킬 것인지에 대한 옵션. 추가로 RotationRate 를 사용하여 회전 속도를 정한다.
		GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
		GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
		GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
		GetCharacterMovement()->MaxWalkSpeed = CharacterControlData->MaxWalkSpeed;
	}
}

#pragma region Input Binding Functions

// 카메라 회전
void ARBCharacterPlayer::InputActionLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	// 마우스 입력은 프레임레이트와 관계 없이 해당 프레임에서의 가속도가 들어오기 때문에 DT 고려하지 않아도 된다. 
	AddControllerPitchInput(LookAxisVector.Y/* * LookSpeed*/);
	AddControllerYawInput(LookAxisVector.X/* * LookSpeed*/);
}

// 캐릭터 움직임
void ARBCharacterPlayer::InputActionMove(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	ProcessCharacterMove(MovementVector.X, MovementVector.Y);
}

// 카메라 -> 캐릭터 줌 인/아웃
void ARBCharacterPlayer::InputActionZoom(const FInputActionValue& Value)
{
	SetNextZoom(Value.Get<float>());
}

// 캐릭터 점프
void ARBCharacterPlayer::InputActionJump()
{
	// 필드 공격 수행 중에는 점프를 할 수 없음.
	if (!IsProcessingFieldAttack())
	{
		UE_LOG(LogTemp, Display, TEXT("Jump"));
		Jump();
	}
}

// 캐릭터 점프 종료
void ARBCharacterPlayer::InputActionStopJumping()
{
	StopJumping();
}

// 캐릭터 걷기/뛰기 모드 변경
void ARBCharacterPlayer::InputActionChangeRunMode()
{
	if (IsRunMode)
	{
		GetCharacterMovement()->MaxWalkSpeed = CharacterControlData->MinWalkSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = CharacterControlData->MaxWalkSpeed;
	}

	IsRunMode = !IsRunMode;
}

// 캐릭터의 필드 공격 (전투 진입 공격)
void ARBCharacterPlayer::InputActionFieldAttack()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		// 공중에서는 필드 공격 수행 불가
		if (!Movement->IsFalling())
		{
			ProcessFieldAttack();
		}
	}
}

#pragma endregion

void ARBCharacterPlayer::ProcessCharacterMove(const float& MoveForwardValue, const float& MoveRightValue)
{
	// 필드 공격 모션 캔슬
	if (CanInterruptFieldAttack() && IsProcessingFieldAttack())
	{
		UE_LOG(LogTemp, Display, TEXT("Interrupted Field Attack"));
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Stop(0.1f, FieldAttackMontage);
		}
	}

	// 캐릭터가 카메라는 고정된 채로, WASD 컨트롤에 따라 회전하며 움직이게 하고 싶으므로
	// 아래와 같이 액터의 방향으로 시도하면 회전하면서 계속 Right Vector 가 변경되므로
	// 컨트롤러의 Yaw 방향을 가져와야 한다.
	//AddMovementInput(GetActorForwardVector() * MovementVector.X);
	//AddMovementInput(GetActorRightVector() * MovementVector.Y);

	// 컨트롤러의 Rotation 을 가져온다.
	const FRotator ControllerYawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ControllerForwardDirection = FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::X);
	const FVector ControllerRightDirection = FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ControllerForwardDirection, MoveForwardValue);
	AddMovementInput(ControllerRightDirection, MoveRightValue);
}

void ARBCharacterPlayer::SetNextZoom(const float ZoomValue)
{
	const float NextZoomValue = SpringArmComponent->TargetArmLength + ZoomValue * CharacterControlData->ZoomSpeed;
	NextZoom = FMath::Clamp(NextZoomValue, TargetArmMinLength, TargetArmMaxLength);
}

void ARBCharacterPlayer::PerformSmoothZoom()
{
	// FInterpTo 이나 EaseOut 혹은 Lerp 을 사용해도 좋다. 한번 비교해보자.
	switch (CharacterControlData->SmoothType)
	{
	case SmoothType::Default :
		SpringArmComponent->TargetArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, NextZoom, GetWorld()->GetDeltaSeconds(), CharacterControlData->SmoothZoomSpeed);
		break;
	case SmoothType::EaseOut:
		SpringArmComponent->TargetArmLength = FMath::InterpEaseOut(SpringArmComponent->TargetArmLength, NextZoom, GetWorld()->GetDeltaSeconds(), CharacterControlData->SmoothZoomSpeed);
		break;
	case SmoothType::Lerp:
		SpringArmComponent->TargetArmLength = FMath::Lerp(SpringArmComponent->TargetArmLength, NextZoom, GetWorld()->GetDeltaSeconds() * CharacterControlData->SmoothZoomSpeed);
		break;
	}
}

#pragma region Play Animation

void ARBCharacterPlayer::ProcessFieldAttack()
{
	if (IsProcessingFieldAttack())
	{
		UE_LOG(LogTemp, Display, TEXT("Already Attacking Now"));
		return;
	}
	else
	{
		FieldAttack();
	}
}

#pragma endregion