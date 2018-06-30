

#ifndef ccraft_inventory_h
#define ccraft_inventory_h

enum
{
    ITEM_NONE = 0,
    ITEM_SHOVEL,
    ITEM_DIRT,
    ITEM_STONE,
    ITEM_TURF,
    ITEM_GIFT,
    ITEM_COUNT,
};

typedef struct
{
    int type;
    int qty;
} Item_t;

typedef struct
{
    short width;
    short height;
    Item_t* items;
    int itemCount;
    int selectedItem;
} Inventory_t;

extern void Inventory_Init(Inventory_t* inventory, short width, short height);

extern Item_t* Inventory_ItemAt(Inventory_t* inventory, short x, short y);
extern int Inventory_AddItem(Inventory_t* inventory, short item, int qty);
extern void Inventory_RemoveItem(Inventory_t* inventory, int index, int qty);

#endif
