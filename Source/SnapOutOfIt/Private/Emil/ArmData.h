// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tobias/Combat/DamageTypes.h"
#include "ArmData.generated.h"

/**
 * 
 */
UENUM(Blueprinttype, DisplayName = "Arm Type")
enum class EArmType : uint8
{
	ET_DEFAULT = 0			UMETA(DisplayName = "Default"),
	ET_MELTED_ARM			UMETA(DisplayName = "Melted"),
	ET_NECRO_ARM			UMETA(DisplayName = "Necro"),
	ET_FLAMETHROWER			UMETA(DisplayName = "Flamethrower"),
	ET_BLEED_ARM			UMETA(DisplayName = "Bleed"),
	ET_VENOM_ARM			UMETA(DisplayName = "Venom"),
	ET_PARALYZE				UMETA(DisplayName = "Paralyze"),
	ET_BRUTE_ARM			UMETA(DisplayName = "Brute")
};

USTRUCT(BlueprintType)
struct FArmData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Arm Info")
	FText ArmName = FText();

	UPROPERTY(EditAnywhere, Category = "Arm Info")
	int32 ArmCharges = 0;

	UPROPERTY(EditAnywhere, Category = "Arm Info")
	int32 SpecialAttackDamage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arm Info")
	EArmType ArmType = EArmType::ET_DEFAULT;

	UPROPERTY(EditAnywhere, Category = "Arm Info")
	E_DAMAGE_TYPE DamageType = E_DAMAGE_TYPE::DT_DEFAULT;

	UPROPERTY(EditAnywhere, Category = "Texture")
	TObjectPtr<UTexture2D> ArmIcon = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMesh> LeftArmSkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMesh> RightArmSkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMesh> ArmWroldMesh = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimInstance> ABPInstance = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmIdleLeftAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmIdleRightAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmAttackActiveLeftAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmAttackActiveRightAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmAttackPassiveLeftAnim = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmAttackPassiveRightAnim = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmSpecialAttackActiveLeftAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmSpecialAttackActiveRightAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmSpecialAttackPassiveLeftAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmSpecialAttackPassiveRightAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmHitLeftAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmHitRightAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmHealLeftAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmHealRightAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmDeathLeftAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmDeathRightAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmSwitchLeftLeftAnim1 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmSwitchLeftLeftAnim2 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmSwitchLeftRightAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmSwitchRightLeftAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmSwitchRightRightAnim1 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimSequence> ArmSwitchRightRightAnim2 = nullptr;
};