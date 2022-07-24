// Fill out your copyright notice in the Description page of Project Settings.


#include "UIBagPanel.h"


void UUIBagPanel::UpdateBagUI(TArray<AItemBase*>& BagList, int32 BagSize)
{
	int32 i = 1;
	for (; i <= BagList.Num(); ++i)
	{
		UTexture2D* img = BagList[i - 1]->ItemInfo.Icon;
		int32 num = BagList[i - 1]->ItemInfo.Num;

		FString memberName = TEXT("Slot_");
		memberName.Append(FString::FromInt(i));

		auto findPro = CastFieldChecked<FObjectProperty>(GetClass()->FindPropertyByName(FName(memberName)));
		auto slot = Cast<UUIBagSlot>(findPro->GetObjectPropertyValue_InContainer(this));

		slot->SetSlotImg(img);
		slot->SetSlotText(num);
		slot->ItemRef = BagList[i - 1];
		slot->SetRenderOpacity(1.f);
	}
	for (; i <= BagSize; ++i)
	{
		FString memberName = TEXT("Slot_");
		memberName.Append(FString::FromInt(i));

		auto findPro = CastFieldChecked<FObjectProperty>(GetClass()->FindPropertyByName(FName(memberName)));
		auto slot = Cast<UUIBagSlot>(findPro->GetObjectPropertyValue_InContainer(this));

		slot->ClearSlotData();
		slot->SetRenderOpacity(1.f);
	}
	for (; i <= 16; ++i)
	{
		FString memberName = TEXT("Slot_");
		memberName.Append(FString::FromInt(i));

		auto findPro = CastFieldChecked<FObjectProperty>(GetClass()->FindPropertyByName(FName(memberName)));
		auto slot = Cast<UUIBagSlot>(findPro->GetObjectPropertyValue_InContainer(this));

		slot->ClearSlotData();
		slot->SetRenderOpacity(0.f);
	}
}