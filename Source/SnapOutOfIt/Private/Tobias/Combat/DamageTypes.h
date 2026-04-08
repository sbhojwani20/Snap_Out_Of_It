#pragma once

#include "CoreMinimal.h"
#include "DamageTypes.generated.h"

UENUM(Blueprinttype, DisplayName = "Damage Types")
enum class E_DAMAGE_TYPE : uint8
{
	DT_DEFAULT = 0		UMETA(DisplayName = "Default"),
	DT_FIRE				UMETA(DisplayName = "Fire"),
	DT_POISON			UMETA(DisplayName = "Poison"),
	DT_PARALYSIS		UMETA(DisplayName = "Paraliysis"),
	DT_BLEED			UMETA(DisplayName = "Bleed"),
	DT_LIFE_STEAL		UMETA(DisplayName = "LifeSteal"),
	DT_COUNT			UMETA(DisplayName = "Count")
};
