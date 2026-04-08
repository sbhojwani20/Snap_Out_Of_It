// Fill out your copyright notice in the Description page of Project Settings.

#include "Shayna/Enemy/EnemyAI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../SnapOutOfItCharacter.h"
#include "Shayna/Enemy/EnemyAIManager.h"
#include "Tobias/Combat/CombatManager.h"
#include "Emil/Arm.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto enemyDataTable = ConstructorHelpers::FObjectFinder<UDataTable>(*ENEMY_DATA_TABLE_PATH);
	if (enemyDataTable.Succeeded()) EnemyDataTable = enemyDataTable.Object;
	if (!EnemyDataTable) return;

	GetMesh()->SetRelativeLocation(MESH_GROUND_OFFSET);
	GetMesh()->SetRelativeRotation(MESH_FACE_DIR_OFFSET);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	SphereColl = CreateDefaultSubobject<USphereComponent>(TEXT("Perception Trigger"));
	SphereColl->SetSphereRadius(500);
	SphereColl->SetRelativeLocation(FVector(0, 0, 90));
	SphereColl->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	//SphereColl->SetHiddenInGame(false);
	SphereColl->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::OnCollision);
	SphereColl->SetupAttachment(GetMesh());

	auto controllerClass = ConstructorHelpers::FClassFinder<AAIController>(*AI_MANAGER_PATH);
	if (controllerClass.Succeeded()) AIControllerClass = controllerClass.Class;
	GetCharacterMovement()->bOrientRotationToMovement = true;

#pragma region Code from Tobias
	static ConstructorHelpers::FObjectFinder<UStaticMesh> butcherKnifeObject(*BUTCHER_KNIFE);

	if (butcherKnifeObject.Succeeded()) ButcherKnife = butcherKnifeObject.Object;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Weapon Mesh")));
	MeshComponent->SetupAttachment(GetRootComponent());
#pragma endregion

}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	CheckIfEnemyIsAlive();
	
	Super::BeginPlay();

	AEnemyAI::SetUpEntity();
	
	if (auto aiManager = Cast<AEnemyAIManager>(GetController()); aiManager)
	{
		aiManager->SetEnemy(this);
		aiManager->SetWaypoint(Waypoints);
	}

	if(GetManager())GetManager()->SetEnemyDataTable(EnemyDataTable);
	else GEngine->AddOnScreenDebugMessage(-3, 5, FColor::Magenta, "No Enemy Manager found!");

	if (Waypoints.Num() != 2) GEngine->AddOnScreenDebugMessage(-3, 5, FColor::Magenta, "Enemy requires 2 Waypoints!");
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyAI::HandleDeath()
{
	// EnemyData.bEnemyAlive = false;
	//
	// EnemyDataTable->AddRow(FName(GetName()), EnemyData);

	GetManager()->SetEnemyDataTable(EnemyDataTable);

	Super::HandleDeath();
}

#if WITH_EDITOR
void AEnemyAI::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	SetUpEntity();
}
#endif WITH_EDITOR

void AEnemyAI::GiveEnemyDataToManager() const
{
	GetManager()->SetEnemyName(EnemyName);
}

void AEnemyAI::SetUpEntity()
{
	Super::SetUpEntity();

	if (GetType() == EEntityType::ET_BUTCHER)
	{
		MeshComponent->SetStaticMesh(ButcherKnife);

		MeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
		                                 TEXT("Weapon_Socket"));

		SetActorScale3D(FVector(1.5f, 1.5f, 1.5f));

		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -60.f));
		SphereColl->SetSphereRadius(200);
	}
	else
	{
		MeshComponent->SetStaticMesh(nullptr);

		SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));

		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	}
}

void AEnemyAI::AttackTarget()
{
	Super::AttackTarget();

	if (CheckParalyseEffect()) return;

	if (GetStats().AttackLeftAnimSequ) GetMesh()->PlayAnimation(GetStats().AttackLeftAnimSequ.Get(), false);
	else ApplyDamageToTarget();
}

void AEnemyAI::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ASnapOutOfItCharacter>()) CollidedWithPlayer = true;
}

void AEnemyAI::CheckIfEnemyIsAlive()
{
	if (auto data = EnemyDataTable->FindRow<FEnemyData>(FName(EnemyName), ""))
	{
		if (data->bEnemyAlive == false || GetType() == EEntityType::ET_DEFAULT) 
		{
			if (GetType() == EEntityType::ET_BUTCHER)
			{
				auto arm = Cast<AArm>(GetWorld()->SpawnActor(AArm::StaticClass(), &ArmSpawn1->GetActorTransform(), FActorSpawnParameters()));
				arm->SetType(EArmType::ET_PARALYZE);
				arm->SetArmDataFromTable();

				auto arm2 = Cast<AArm>(GetWorld()->SpawnActor(AArm::StaticClass(), &ArmSpawn2->GetActorTransform(), FActorSpawnParameters()));
				arm2->SetType(EArmType::ET_VENOM_ARM);
				arm2->SetArmDataFromTable();
			}
			Destroy();
		}
	}
	else
	{
		EnemyData.EnemyName = EnemyName;
		EnemyData.bEnemyAlive = true;

		EnemyDataTable->RemoveRow(FName(EnemyName));
		EnemyDataTable->AddRow(FName(EnemyName), EnemyData);
	}
}
