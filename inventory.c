
#include "inventory.h"
#include <stdlib.h>

static const char ItemStackableTable[] =
{
    0, /* none */
    0,
    1, /* dirt */
    1, /* stone */
    1,
    1,
    
};

void Inventory_Init(Inventory_t* inventory, short width, short height)
{
    inventory->items = malloc(sizeof(Item_t) * width * height);
    inventory->width = width;
    inventory->height = height;
    inventory->selectedItem = -1;
}

Item_t* Inventory_ItemAt(Inventory_t* inventory, short x, short y)
{
    return &inventory->items[x + y * inventory->width];
}

int Inventory_AddItem(Inventory_t* inventory, short item, int qty)
{
    int ex = -1, ey = -1;
    
    int x, y;
    for (y = 0; y < inventory->height; ++y)
    {
        for (x = 0; x < inventory->width; ++x)
        {
            if (ex == -1 && inventory->items[x + y * inventory->width].type == ITEM_NONE)
            {
                ex = x;
                ey = y;
                
                if (!ItemStackableTable[item])
                {
                    break;
                }
            }
            else if (ItemStackableTable[item])
            {
                if (inventory->items[x + y * inventory->width].type == item)
                {
                    ex = x;
                    ey = y;
                    break;
                }
            }
        }
    }
    
    if (ex >= 0 && ey >= 0)
    {
        inventory->items[ex + ey * inventory->width].type = item;
        inventory->items[ex + ey * inventory->width].qty += qty;
        return 1;
    }
    
    return 0;
}

void Inventory_RemoveItem(Inventory_t* inventory, int index, int qty)
{
    inventory->items[index].qty -= qty;
    
    if (inventory->items[index].qty <= 0)
    {
        inventory->items[index].qty = 0;
        inventory->items[index].type = ITEM_NONE;
    }
}


