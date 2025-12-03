// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomVolume.h"

#include "RoomInterface.h"
#include "Components/BoxComponent.h"

// Sets default values
ARoomVolume::ARoomVolume()
{
	RoomZone = CreateDefaultSubobject<UBoxComponent>("RoomZone");
	RootComponent = RoomZone;
	
	RoomZone->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARoomVolume::OnOverlapBegin);
}

ARoomVolume* ARoomVolume::WhichRoomNoiseIs(const FVector& NoiseLocation)
{
	/* Take all the RoomVolume of the World and 
	 * with its collider detect which collider is overlapping with the noise and then return the RoomVolume */
	for (TObjectIterator<ARoomVolume> It; It; ++It)
	{
		ARoomVolume* Room = *It;
		
		if (!IsValid(Room) || !Room->RoomZone)
			continue;
		
		FBoxSphereBounds Bounds = Room->RoomZone->CalcBounds(Room->RoomZone->GetComponentTransform());
		FBox BoxBound = Bounds.GetBox();
		
		FVector BoxCenter = BoxBound.GetCenter();
		FVector BoxExtent = BoxBound.GetExtent();
		
		if(BoxBound.IsInside(NoiseLocation))
		{
			return Room;
		}
	}
	
	return nullptr;
}

void ARoomVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OtherActor == this)
		return;
	
	if (OtherActor->Implements<URoomInterface>())
	{
		IRoomInterface::Execute_SetCurrentRoom(OtherActor, RoomName);
	}
}
