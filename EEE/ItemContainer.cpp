#include "ItemContainer.h"


ItemContainer::ItemContainer(int maxItems)
{
	this->maxItems = maxItems;
	itemSlots = std::vector<std::shared_ptr<BaseItem>>();
}


int ItemContainer::GetSize()
{
	return itemSlots.size();
}
void ItemContainer::RemoveItem(int index) {
	if (index >= 0 && index < itemSlots.size()) {
		itemSlots.erase(itemSlots.begin() + index);
	}
}