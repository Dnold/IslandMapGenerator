#include "Inventory.h"
Inventory::Inventory(int _maxItems)
{
	maxItems = _maxItems;
	items = new ItemContainer(maxItems);
	equipmentSlots[0] = EquipmentSlot(BaseItem::ItemType::ARMOR);
	equipmentSlots[1] = EquipmentSlot(BaseItem::ItemType::WEAPON);
	equipmentSlots[2] = EquipmentSlot(BaseItem::ItemType::RING);

}

void Inventory::RenderItems(int startX, int startY, int lineHeight)
{
	int itemCount = items->GetSize();
	// Berechnen der Höhe des Hintergrunds basierend auf der Anzahl der Gegenstände und der Zeilenhöhe
	int backgroundHeight = itemCount * lineHeight;
	// Berechnen der Breite des Hintergrunds. Dies kann angepasst werden, um den Text zu passen.
	int backgroundWidth = 200; // Beispielbreite, passen Sie dies an Ihre Bedürfnisse an

	// Zeichnen des transparenten schwarzen Hintergrunds
	Color backgroundColor = { 0, 0, 0, 128 }; // RGBA für halbtransparentes Schwarz
	DrawRectangle(startX, startY, backgroundWidth, backgroundHeight, backgroundColor);

	// Render der Gegenstände wie zuvor
	for (int i = 0; i < itemCount; ++i)
	{
		auto item = items->GetItem<std::shared_ptr<BaseItem>>(i);
		DrawText(item->name.c_str(), startX, startY + i * lineHeight, 20, WHITE);
		DrawText(item->description.c_str(), startX, startY + 25 + i * lineHeight, 10, WHITE);
		std::string weightText = std::to_string(item->weight) + " kg";
		// Zeichnen des Gewichts
		DrawText(weightText.c_str(), startX, startY + 36 + i * lineHeight, 10, WHITE);
		//Zeichnen des Preises
		std::string priceText = std::to_string(item->price) + " $";
		DrawText(priceText.c_str(), startX, startY + 48 + i * lineHeight, 10, WHITE);
	}
}
bool Inventory::TryEquipItem(std::shared_ptr<BaseItem> item) {
	for (int i = 0; i < 3; ++i) { // Angenommen, es gibt 3 Ausrüstungsslots
		if (equipmentSlots[i].CanEquip(item)) {
			std::shared_ptr<BaseItem> itemToEquip = item;

			// Definieren der returnToInventory-Callback-Funktion als Lambda
			auto returnToInventory = [this](std::shared_ptr<BaseItem> item) {
				this->items->AddItem(item);
			};

			equipmentSlots[i].EquipItem(item, returnToInventory);
			return true;
		}
	}
	return false;
}
void Inventory::RemoveItem(int index) {
	items->RemoveItem(index);
}
void Inventory::RenderEquipmentSlots(int startX, int startY, int lineheight)
{
	// Starting Y position for rendering equipment slots
	int slotWidth = 100; // Width of each equipment slot
	int slotHeight = lineheight; // Height of each equipment slot

	// Render each equipment slot
	for (int i = 0; i < 3; ++i) // Assuming there are 3 equipment slots
	{
		// Calculate the position of the current equipment slot
		int slotX = startX + i * slotWidth;
		int slotY = startY;

		// Draw the equipment slot rectangle
		DrawRectangle(slotX, slotY, slotWidth, slotHeight, { BLACK.r,BLACK.g,BLACK.b,100 });

		// Get the item equipped in the current equipment slot
		std::shared_ptr<BaseItem> equippedItem = equipmentSlots[i].itemSlot;

		// If an item is equipped, render its name inside the equipment slot
		if (equippedItem != nullptr)
		{
			DrawText(equippedItem->name.c_str(), slotX + 10, slotY + 10 + i * 15, 20, WHITE);
		}
	}
}
void Inventory::SortInventoryWeight()
{
	for (int i = 0; i < items->GetSize(); i++)
	{
		for (int j = 0; j < items->GetSize(); j++)
		{
			if (items->GetItem<std::shared_ptr<BaseItem>>(i)->weight < items->GetItem<std::shared_ptr<BaseItem>>(j)->weight)
			{
				std::shared_ptr<BaseItem> temp = items->GetItem<std::shared_ptr<BaseItem>>(i);
				items->SetItem(i, items->GetItem<std::shared_ptr<BaseItem>>(j));
				items->SetItem(j, temp);
			}
		}
	}
}
void Inventory::SortInventoryName()
{
	for (int i = 0; i < items->GetSize(); i++)
	{
		for (int j = 0; j < items->GetSize(); j++)
		{
			if (items->GetItem<std::shared_ptr<BaseItem>>(i)->name < items->GetItem<std::shared_ptr<BaseItem>>(j)->name)
			{
				std::shared_ptr<BaseItem> temp = items->GetItem<std::shared_ptr<BaseItem>>(i);
				items->SetItem(i, items->GetItem<std::shared_ptr<BaseItem>>(j));
				items->SetItem(j, temp);
			}
		}
	}
}
void Inventory::SortInventoryPrice()
{
	for (int i = 0; i < items->GetSize(); i++)
	{
		for (int j = 0; j < items->GetSize(); j++)
		{
			if (items->GetItem<std::shared_ptr<BaseItem>>(i)->price < items->GetItem<std::shared_ptr<BaseItem>>(j)->price)
			{
				std::shared_ptr<BaseItem> temp = items->GetItem<std::shared_ptr<BaseItem>>(i);
				items->SetItem(i, items->GetItem<std::shared_ptr<BaseItem>>(j));
				items->SetItem(j, temp);
			}
		}
	}
}
