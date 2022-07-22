// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBagComp.h"
#include "Bpex/Object/OutfitBase.h"

// Sets default values for this component's properties
UPlayerBagComp::UPlayerBagComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxSize = OriginSize;
}


// Called when the game starts
void UPlayerBagComp::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UPlayerBagComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/////////////////////////////////////////
// CustomEvent
void UPlayerBagComp::AttainBag(AOutfitBase* attainBag)
{
	MaxSize = OriginSize + attainBag->OutfitInfo.AdditiveValue;
	BagLevel = attainBag->OutfitInfo.Level;
	EquippedBag = attainBag;
}

void UPlayerBagComp::GetItem(AItemBase* Item)
{
	int32 currentSize = Bag.Num();

	FItemInfo& itemInfo = Item->ItemInfo;
	for (auto traverse : Bag)
	{
		if (traverse->ItemInfo.ItemType == itemInfo.ItemType)
		{
			int32 NumLeft = traverse->ItemInfo.MaxNum - traverse->ItemInfo.Num;
			if (NumLeft == 0)
				continue;
			if (NumLeft >= itemInfo.Num)
			{
				traverse->ItemInfo.Num += itemInfo.Num;
				itemInfo.Num = 0;
				Item->SetHideStatus(true);
			}
			else
			{
				traverse->ItemInfo.Num = itemInfo.MaxNum;
				itemInfo.Num -= NumLeft;
				if (currentSize < MaxSize)
				{
					Bag.Add(Item);
					Item->SetHideStatus(true);
				}
			}

			HUDItemDel.ExecuteIfBound(itemInfo.ItemType, CountItemInBag(itemInfo.ItemType));
			UIInventoryDel.Broadcast(Bag, MaxSize);
			return;
		}
	}

	if (currentSize < MaxSize)
	{
		Bag.Add(Item);
		Item->SetHideStatus(true);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("No more space!"));
	}

	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::FromInt(CountItemInBag(itemInfo.ItemType)));
	HUDItemDel.ExecuteIfBound(itemInfo.ItemType, CountItemInBag(itemInfo.ItemType));
	UIInventoryDel.Broadcast(Bag, MaxSize);
}

void UPlayerBagComp::DropItem(AItemBase* Item)
{
	Bag.Remove(Item);

	Item->SetHideStatus(false);
	Item->SetActorLocation(GetOwner()->GetActorLocation());
	Item->Mesh->AddImpulse(GetOwner()->GetActorRotation().Vector() * 400.f);
	HUDItemDel.ExecuteIfBound(Item->ItemInfo.ItemType, CountItemInBag(Item->ItemInfo.ItemType));
	//
}

bool UPlayerBagComp::UseItems(AItemBase* Item, UPARAM(ref) int32& UseNum)
{
	FItemInfo& itemInfo = Item->ItemInfo;
	EItemType ItemType = itemInfo.ItemType;

	if (itemInfo.Num > UseNum)
	{
		itemInfo.Num -= UseNum;
		Item->UseItem();
		UIInventoryDel.Broadcast(Bag, MaxSize);
		return true;
	}

	UseNum -= itemInfo.Num;
	itemInfo.Num = 0;
	Item->UseItem();
	Bag.Remove(Item);

	if (UseNum != 0)
	{
		AItemBase* item = SearchInBag(ItemType);
		if (item != nullptr)
		{
			UIInventoryDel.Broadcast(Bag, MaxSize);
			return UseItems(item, UseNum);
		}
		else
		{
			UIInventoryDel.Broadcast(Bag, MaxSize);
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("Out of items"));
			return false;
		}
	}
	else
	{
		UIInventoryDel.Broadcast(Bag, MaxSize);
		return true;
	}
}

AItemBase* UPlayerBagComp::SearchInBag(EItemType ItemType)
{
	int size = Bag.Num();
	int minItemNum = 100;
	int minIdx = -1;
	for (int i=0; i<size; ++i)
	{
		if (ItemType == Bag[i]->ItemInfo.ItemType && Bag[i]->ItemInfo.Num < minItemNum)
		{
			minItemNum = Bag[i]->ItemInfo.Num;
			minIdx = i;
		}
	}
	if (minIdx != -1)
		return Bag[minIdx];
	else
		return nullptr;
}

int32 UPlayerBagComp::CountItemInBag(EItemType ItemType)
{
	int32 count = 0;
	for (auto item : Bag)
	{
		if (item->ItemInfo.ItemType == ItemType)
		{
			count += item->ItemInfo.Num;
		}
	}

	return count;
}