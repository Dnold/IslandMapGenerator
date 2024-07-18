#include "EquipmentSlot.h"
void EquipmentSlot::SetSlot(std::shared_ptr<BaseItem> item)
{
	if (item->type == slotType)
	{
		itemSlot = item;
	}
	else
	{
		std::cout << "Item type does not match slot type" << std::endl;
	}
}
bool EquipmentSlot::CanEquip(std::shared_ptr<BaseItem> item)
{

	return item->type == slotType;
}
void EquipmentSlot::UnequipItem(std::function<void(std::shared_ptr<BaseItem>)> returnToInventory)
{
	if (itemSlot != nullptr)
	{
		returnToInventory(itemSlot);
		itemSlot = nullptr;
	}
}
void EquipmentSlot::EquipItem(std::shared_ptr<BaseItem> item, std::function<void(std::shared_ptr<BaseItem>)> returnToInventory)
{
	if (CanEquip(item))
	{
		if (itemSlot != nullptr)
		{
			// Wenn bereits ein Gegenstand im Slot ist, legen Sie diesen zurück ins Inventar
			returnToInventory(itemSlot);
		}
		// Den neuen Gegenstand im Slot platzieren
		itemSlot = item;
	}
	else
	{
		std::cout << "Item type does not match slot type" << std::endl;
	}
}