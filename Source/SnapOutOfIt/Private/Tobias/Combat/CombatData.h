#pragma once

#include "CoreMinimal.h"
#include "CombatData.generated.h"

UENUM(Blueprinttype, DisplayName = "Entity Type")
enum class EEntityType : uint8
{
	ET_DEFAULT = 0		UMETA(DisplayName = "Default"),
	ET_PLAYER			UMETA(DisplayName = "Player"),
	ET_ENEMY_A			UMETA(DisplayName = "Enemy_A"),
	ET_ENEMY_B			UMETA(DisplayName = "Enemy_B"),
	ET_ENEMY_C			UMETA(DisplayName = "Enemy_C"),
	ET_BUTCHER			UMETA(DisplayName = "Butcher"),
	ET_FINAL_BOSS		UMETA(DisplayName = "FinalBoss"),
	ET_COUNT			UMETA(DisplayName = "Count")
};

UENUM(Blueprinttype, DisplayName = "Status Effects")
enum class EStatusEffectType : uint8
{
	SE_DEFAULT = 0		UMETA(DisplayName = "Default"),
	SE_BURNED			UMETA(DisplayName = "Burned"),
	SE_POISONED			UMETA(DisplayName = "Poisoned"),
	SE_PARALIZED		UMETA(DisplayName = "Paralized"),
	SE_BLEEDING			UMETA(DisplayName = "Bleeding"),
	SE_COUNT			UMETA(DisplayName = "Count")
};

UENUM(Blueprinttype, DisplayName = "Heal Type")
enum class EHealType : uint8
{
	HT_DEFAULT = 0			UMETA(DisplayName = "Default"),
	HT_HEAL_FLASK			UMETA(DisplayName = "Heal_Flask"),
	HT_LIFE_STEAL			UMETA(DisplayName = "Life_Steal"),
	HT_COUNT				UMETA(DisplayName = "Count")
};

USTRUCT(BlueprintType)
struct FCombatData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	TObjectPtr<USkeletalMesh> EntityMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> IdleAnimSequ = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> IdleCombatAnimSequ = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> AttackLeftAnimSequ = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> AttackRightAnimSequ = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> SpecialLeftAnimSequ = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> SpecialRightAnimSequ = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> DeathAnimSequ = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> WalkAnimSequ = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> HitAnimSequ = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> HealAnimSequ = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> EffectedAnimSequ = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> ArmSwitchLeftAnimSequ = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimSequence> ArmSwitchRightAnimSequ = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Info")
	EStatusEffectType StatusEffect{ EStatusEffectType::SE_DEFAULT };

	UPROPERTY(EditAnywhere, Category = "Info")
	FText Name = FText();

	UPROPERTY(EditAnywhere, Category = "Info")
	int32 MaxHealth = 0;

	UPROPERTY(EditAnywhere, Category = "Info")
	int32 Health = 0;

	UPROPERTY(EditAnywhere, Category = "Info")
	int32 Attack = 0;

	UPROPERTY(EditAnywhere, Category = "Info")
	int32 Defense = 0;

	UPROPERTY(EditAnywhere, Category = "Info")
	int32 Initiative = 0;
};