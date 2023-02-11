// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	UWorld* World = GetWorld();
	if (MuzzleFlashSocket && World)
	 {
		 FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

		 //From muzzle flash socket to hit location from TracceUnderCrosshairs
		 FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		 FRotator TargetRotation = ToTarget.Rotation();

		 FActorSpawnParameters SpawnParams;
		 SpawnParams.Owner = GetOwner();
		 SpawnParams.Instigator = InstigatorPawn;
		 
		 AProjectile* SpawnedProjectile = nullptr;
		 if (bUseServerSideRewind)
		 {
			 if (InstigatorPawn->HasAuthority()) // Server
			 {
				 if (InstigatorPawn->IsLocallyControlled()) // Server, host - use replicated projectile
				 {
					 SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					 SpawnedProjectile->bUseServerSideRewind = false;
					 SpawnedProjectile->Damage = DamageAmount;
					 SpawnedProjectile->HeadShotDamage = HeadShotDamage;
				 }
				 else // server, not locally controlled - spawn non-replicated projectie, no ssr
				 {
					 SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					 SpawnedProjectile->bUseServerSideRewind = true;
				 }
			 }
			 else //client, using ssr
			 {
				 if (InstigatorPawn->IsLocallyControlled()) // client, locally controlled - spawn non-replicated projectile, use ssr
				 {
					 SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					 SpawnedProjectile->bUseServerSideRewind = true;
					 SpawnedProjectile->TraceStart = SocketTransform.GetLocation();
					 SpawnedProjectile->InitialVelocity = SpawnedProjectile->GetActorForwardVector() * SpawnedProjectile->InitialSpeed;
				 }
				 else //client, not localy controller - spawn non-replicated projectile, no ssr
				 {
					 SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					 SpawnedProjectile->bUseServerSideRewind = false;
				 }
			 }
		 }
		 else //weapon not using ssr
		 {
			 if (InstigatorPawn->HasAuthority())
			 {
				 SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
				 SpawnedProjectile->bUseServerSideRewind = false;
				 SpawnedProjectile->Damage = DamageAmount;
				 SpawnedProjectile->HeadShotDamage = HeadShotDamage;
			 }

		 }

		// World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
	 }
}