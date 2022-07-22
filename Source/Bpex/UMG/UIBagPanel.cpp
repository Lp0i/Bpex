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
		slot->SetRenderOpacity(1.f);

		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, slot->GetName());
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, img->GetName());
	}
	for (; i <= BagSize; ++i)
	{
		FString memberName = TEXT("Slot_");
		memberName.Append(FString::FromInt(i));

		auto findPro = CastFieldChecked<FObjectProperty>(GetClass()->FindPropertyByName(FName(memberName)));
		auto slot = Cast<UUIBagSlot>(findPro->GetObjectPropertyValue_InContainer(this));

		slot->SetSlotImg(nullptr);
		slot->SetSlotText(0);
		slot->SetRenderOpacity(1.f);
	}
	for (; i <= 16; ++i)
	{
		FString memberName = TEXT("Slot_");
		memberName.Append(FString::FromInt(i));

		auto findPro = CastFieldChecked<FObjectProperty>(GetClass()->FindPropertyByName(FName(memberName)));
		auto slot = Cast<UUIBagSlot>(findPro->GetObjectPropertyValue_InContainer(this));
		slot->SetRenderOpacity(0.f);
	}
}

void UUIBagPanel::PrintProperty()
{
	auto slot = Cast<FObjectProperty>(GetClass()->FindPropertyByName(TEXT("Slot_1")));
	auto a = Cast<UUIBagSlot>(slot->GetObjectPropertyValue_InContainer(this));
}