#include "Menu.h"
#include "Color.h"
#include "Display.h"
#include "common.h"
#include "Log.h"

static const int    MENU_SUBITEM_HEIGHT_OPENED = 20,

                    MENU_ITEM_WIDTH_TITLE = 100,
                    MENU_ITEM_HEIGHT_TITLE = 25,
                    MENU_ITEM_HEIGHT = 35,
                    MENU_ITEM_WIDTH = 75,
                    MENU_ITEM_HEIGHT_RECT_FOR_VALUE = 15,
                    MENU_ITEM_WIDTH_RECT_FOR_VALUE = 69,

                    MENU_PAGE_ITEMS_ON_PAGE = 5,

                    MENU_HEIGHT_TITLE = 25;

enum    TypeMenuSubItem {
    // Channel 1
    MSI_Chan1enterEnable,   MSI_Chan1enterDisable,  MSI_Chan1enterInverse,  MSI_Chan1couplingAC,    MSI_Chan1couplingDC,    MSI_Chan1couplingGND,
    MSI_Chan1factor1x,      MSI_Chan1factor10x,
    // Channel 2
    MSI_Chan2enterEnable,   MSI_Chan2enterDisable,  MSI_Chan2enterInverse,  MSI_Chan2couplingAC,    MSI_Chan2couplingDC,    MSI_Chan2couplingGND,   
    MSI_Chan2factor1x,      MSI_Chan2factor10x,
    // Cursors
    MSI_CursChoiceVert,     MSI_CursChoiceHoriz,    MSI_CursChoiceDisable,  MSI_CursXcursor1,       MSI_CursXcursor2,       MSI_CursXcursor12, 
    MSI_CursMeasTT1,        MSI_CursMeasTT2,        MSI_CursMeasTdT,        MSI_CursYcursor1,       MSI_CursYcursor2,       MSI_CursYcursor12, 
    MSI_CursMeasVV1,        MSI_CursMeasVV2,        MSI_CursMeasVdV,
    // Display
    MSI_DisplMappingVector, MSI_DisplMappingPoints, MSI_DisplGridStandard,  MSI_DisplGridCoord,     MSI_DisplGridDisable, 
    MSI_DisplAccumEnable,   MSI_DisplAccumDisable,  MSI_DisplUpdate,        MSI_DisplScreen1,       MSI_DisplScreen2,
    // Memory
    MSI_MemOperExtract,     MSI_MemOperSaving,      MSI_MemSourceChan1,     MSI_MemSoucreChan2,     MSI_MemSoucreSetting,
    MSI_MemStorageInt,      MSI_MemStorageExt,      MSI_MemMemoryM1,        MSI_MemExecutionEXE,
    // Synchronization
    MSI_TrigModeAuto,       MSI_TrigModeWaiting,    MSI_TrigModeSingle,     MSI_TrigSourceChan1,    MSI_TrigSourceChan2,    MSI_TrigSourceExt,
    MSI_TrigPolarityFront,  MSI_TrigPolarityFall,   MSI_TrigFiltrDisable,   MSI_TrigFiltrLPF,       MSI_TrigFiltrHPF,       MSI_TrigCouplingDC,     
    MSI_TrigCouplingAC,
    // Measures
    MSI_MeasFreq1,          MSI_MeasFreq2,          MSI_MeasT1,             MSI_MeasT2,             MSI_MeasDurationPOS1,   MSI_MeasDurationPOS2,   
    MSI_MeasDurationNEG1,   MSI_MeasDurationNEG2,   MSI_MeasDuty1,          MSI_MeasDuty2,          MSI_MeasDurationFront1, MSI_MeasDurationFront2, 
    MSI_MeasDurationFall1,  MSI_MeasDurationFall2,  MSI_MeasVampl1,         MSI_MeasVampl2,         MSI_MeasVpp1,           MSI_MeasVpp2,   
    MSI_MeasVrms1,          MSI_MeasVrms2,          MSI_MeasVaverage1,      MSI_MeasVaverage2,      MSI_MeasVmax1,          MSI_MeasVmax2,
    MSI_MeasVmin1,          MSI_MeasVmin2
};

static char *subItemNames[] = {
    "ÂÊË.",                 "ÂÛÊË.",            "ÈÍÂÅÐÑÈß",         "ÏÅÐÅÌ.",           "ÏÎÑÒ.",            "ÇÅÌËß",
    "1õ",                   "10õ",

    "ÂÊË.",                 "ÂÛÊË.",            "ÈÍÂÅÐÑÈß",         "ÏÅÐÅÌ.",           "ÏÎÑÒ.",            "ÇÅÌËß",
    "1õ",                   "10õ",

    "ÂÅÐÒÈÊ.",              "ÃÎÐÈÇÎÍÒ.",        "ÂÛÊË.",            "ÊÓÐÑÎÐ 1",         "ÊÓÐÑÎÐ 2",         "ÊÓÐÑÎÐÛ 1+2",
    "Ò1 = ",                "Ò2 = ",            "dT = ",            "ÊÓÐÑÎÐ 1",         "ÊÓÐÑÎÐ 2",         "ÊÓÐÑÎÐÛ 1+2",
    "V1 = ",                "V2 = ",            "dV = ",

    "ÂÅÊÒÎÐ",               "ÒÎ×ÊÈ",            "ÑÒÀÍÄÀÐÒ",         "ÊÎÎÐÄÈÍÀÒ.",       "ÂÛÊË.",
    "ÂÊË.",                 "ÂÛÊË.",            "16",               "1",                "2",

    "ÈÇÂËÅ×.",              "ÑÎÕÐÀÍ.",          "ÊÀÍÀË 1",          "ÊÀÍÀË 2",          "ÓÑÒÀÍ.",
    "ÂÍÓÒÐ.",               "ÂÍÅØÍ.",           "Ì1",               "ÂÛÏÎËÍ.",

    "ÀÂÒ.",                 "ÆÄÓÙÈÉ",           "ÎÄÍÎÊÐ.",          "ÊÀÍÀË 1",          "ÊÀÍÀË 2",          "ÂÍÅØÍ.",
    "ÔÐÎÍÒ",                "ÑÐÅÇ",             "ÂÛÊË.",            "ÔÍ×",              "ÔÂ×",              "ÏÎÑÒ.",
    "ÏÅÐÅÌ.",

    "1:            ",       "2:            ",   "1:            ",   "2:            ",   "1:            ",   "2:            ",
    "1:            ",       "2:            ",   "1:            ",   "2:            ",   "1:            ",   "2:            ",
    "1:            ",       "2:            ",   "1:            ",   "2:            ",   "1:            ",   "2:            ",
    "1:            ",       "2:            ",   "1:            ",   "2:            ",   "1:            ",   "2:            ",
    "1:            ",       "2:            "
};

enum    TypeMenuItem {
    MI_Chan1enter,      MI_Chan1coupling,   MI_Chan1factor, 
    MI_Chan2enter,      MI_Chan2coupling,   MI_Chan2factor,
    MI_CursChoice,      MI_CursCursorsX,    MI_CursMeasT,   MI_CursCursorxY,    MI_CursMeasV,
    MI_DisplMapping,    MI_DisplGrid,       MI_DisplAccum,  MI_DisplUpdate,     MI_DisplScreen,
    MI_MemOperation,    MI_MemSource,       MI_MemStorage,  MI_MemMemory,       MI_MemExecution,
    MI_TrigMode,        MI_TrigSource,      MI_TrigPolarity,MI_TrigCoupling,    MI_TrigFiltr,

    MI_MeasFreq,        MI_MeasT,
    MI_MeasDurationPOS,         // äëèò. +
    MI_MeasDurationNEG,         // äëèòþ -
    MI_MeasDuty,                // ñêâàæíîñòü
    MI_MeasDurationFront,       // äëèò. ôðîíòà
    MI_MeasDurationFall,        // äëèò. ñðåçà
    MI_MeasVampl,       MI_MeasVpp,         MI_MeasVrms,    MI_MeasVaverage,    MI_MeasVmax,    MI_MeasVmin
};

enum    TypeMenuPage {
    MP_Chan1,   MP_Chan2,   MP_Cursors, MP_Display, MP_Memory,  MP_Trig,    MP_Measures
};
//*****************************************************************************************************
struct  MenuSubItem {
    enum TypeMenuSubItem type;
    bool selected;
};


struct  MenuItem {
    struct MenuSubItem subItems[3];
    bool selected;
    enum TypeMenuItem type;
    int position;
    struct MenuPage *page;
    char *name;
    int numberSubItems;
};

struct  MenuPage {
    int firstItemOnScreen;
    struct MenuItem *items;
    char *name;
    bool enabled;
    int numberItems;
    struct MenuItem *openedItem;
};

static struct MenuItem itemsChan1[3];
static struct MenuItem itemsChan2[3];
static struct MenuItem itemsCursors[5];
static struct MenuItem itemsDisplay[5];
static struct MenuItem itemsMemory[5];
static struct MenuItem itemsTrig[5];
static struct MenuItem itemsMeasures[13];

static struct MenuPage pages[7];
static struct MenuPage *curPage;
static int numberCurPage;
static int buttonTimer;

//******************************************************************************************************
void    MenuSubItem_Init(struct MenuSubItem *_subItem, enum TypeMenuSubItem _type) {
    _subItem->type = _type;
    _subItem->selected = false;
};

//*******************************************************************************************************
void    MenuSubItem_DrawOpened(struct MenuSubItem *_subItem, int _x, int _y) {
    Display_SetDrawStyle(ColorR128, ColorWHITE);
    Display_DrawRect(_x, _y, MENU_ITEM_WIDTH_TITLE, MENU_SUBITEM_HEIGHT_OPENED);
    if(_subItem->selected) {
        Display_SetDrawStyle(ColorR154, ColorR128);
        int delta = 3;
        Display_DrawRect(_x + delta, _y + delta, MENU_ITEM_WIDTH_TITLE - 2 * delta, MENU_SUBITEM_HEIGHT_OPENED - 2 * delta);
    }
    Display_DrawText(subItemNames[_subItem->type], _x, _y + 3);
}

//*********************************************************************************************************
void    MenuItem_Init(struct MenuItem *_item, enum TypeMenuItem _type, struct MenuPage *_page) {
    _item->selected = false;
    _item->page = _page;
    _item->position = -1;
    _item->numberSubItems = 0;

    if(_type == MI_Chan1enter) {
        _item->name = "ÂÕÎÄ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_Chan1enterEnable);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_Chan1enterDisable);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_Chan1enterInverse);
        _item->numberSubItems = 3;
    } else if(_type == MI_Chan1coupling) {
        _item->name = "ÑÂßÇÜ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_Chan1couplingDC);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_Chan1couplingAC);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_Chan1couplingGND);
        _item->numberSubItems = 3;
    } else if(_type == MI_Chan2enter) {
        _item->name = "ÂÕÎÄ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_Chan2enterEnable);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_Chan2enterDisable);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_Chan2enterInverse);
        _item->numberSubItems = 3;
    } else if(_type == MI_Chan2coupling) {
        _item->name = "ÑÂßÇÜ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_Chan2couplingDC);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_Chan2couplingAC);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_Chan2couplingGND);
        _item->numberSubItems = 3;
    } else if(_type == MI_Chan1factor) {
        _item->name = "ÌÍÎÆÈÒÅËÜ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_Chan1factor1x);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_Chan1factor10x);
        _item->numberSubItems = 2;
    } else if(_type == MI_Chan2factor) {
        _item->name = "ÌÍÎÆÈÒÅËÜ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_Chan2factor1x);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_Chan2factor10x);
        _item->numberSubItems = 2;
    } else if(_type == MI_CursChoice) {
        _item->name = "ÂÛÁÎÐ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_CursChoiceVert);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_CursChoiceHoriz);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_CursChoiceDisable);
        _item->numberSubItems = 3;
    } else if(_type == MI_CursCursorsX) {
        _item->name = "ÊÓÐÑÎÐÛ Õ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_CursXcursor1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_CursXcursor2);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_CursXcursor12);
        _item->numberSubItems = 3;
    } else if(_type == MI_CursMeasT) {
        _item->name = "ÈÇÌÅÐ.T";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_CursMeasTT1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_CursMeasTT2);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_CursMeasTdT);
        _item->numberSubItems = 3;
    } else if(_type == MI_CursCursorxY) {
        _item->name = "ÊÓÐÑÎÐÛ Y";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_CursYcursor1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_CursYcursor2);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_CursYcursor12);
        _item->numberSubItems = 3;
    } else if(_type == MI_CursMeasV) {
        _item->name = "ÈÇÌÅÐ.V";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_CursMeasVV1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_CursMeasVV2);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_CursMeasVdV);
        _item->numberSubItems = 3;
    } else if(_type == MI_DisplMapping) {
        _item->name = "ÎÒÎÁÐÀÆ.";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_DisplMappingVector);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_DisplMappingPoints);
        _item->numberSubItems = 2;
    } else if(_type == MI_DisplGrid) {
        _item->name = "ÑÅÒÊÀ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_DisplGridStandard);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_DisplGridCoord);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_DisplGridDisable);
        _item->numberSubItems = 3;
    } else if(_type == MI_DisplAccum) {
        _item->name = "ÍÀÊÎÏË.";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_DisplAccumEnable);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_DisplAccumDisable);
        _item->numberSubItems = 2;
    } else if(_type == MI_DisplUpdate) {
        _item->name = "ÎÁÍÎÂË.";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_DisplUpdate);
        _item->numberSubItems = 1;
    } else if(_type == MI_DisplScreen) {
        _item->name = "ÝÊÐÀÍ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_DisplScreen1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_DisplScreen2);
        _item->numberSubItems = 2;
    } else if(_type == MI_MemOperation) {
        _item->name = "ÎÏÅÐÀÖÈß";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MemOperExtract);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MemOperSaving);
        _item->numberSubItems = 2;
    } else if(_type == MI_MemSource) {
        _item->name = "ÈÑÒÎ×ÍÈÊ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MemSourceChan1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MemSoucreChan2);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_MemSoucreSetting);
        _item->numberSubItems = 3;
    } else if(_type == MI_MemStorage) {
        _item->name = "ÕÐÀÍÈËÈÙÅ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MemStorageInt);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MemStorageExt);
        _item->numberSubItems = 2;
    } else if(_type == MI_MemMemory) {
        _item->name = "ÏÀÌßÒÜ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MemMemoryM1);
        _item->numberSubItems = 1;
    } else if(_type == MI_MemExecution) {
        _item->name = "ÂÛÏÎËÍ.";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MemExecutionEXE);
        _item->numberSubItems = 1;
    } else if(_type == MI_TrigMode) {
        _item->name = "ÐÅÆÈÌ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_TrigModeAuto);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_TrigModeWaiting);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_TrigModeSingle);
        _item->numberSubItems = 3;
    } else if(_type == MI_TrigSource) {
        _item->name = "ÈÑÒÎ×ÍÈÊ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_TrigSourceChan1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_TrigSourceChan2);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_TrigSourceExt);
        _item->numberSubItems = 3;
    } else if(_type == MI_TrigPolarity) {
        _item->name = "ÏÎËßÐÍÎÑÒÜ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_TrigPolarityFront);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_TrigPolarityFall);
        _item->numberSubItems = 2;
    } else if(_type == MI_TrigCoupling) {
        _item->name = "ÑÂßÇÜ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_TrigCouplingDC);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_TrigCouplingAC);
        _item->numberSubItems = 2;
    } else if(_type == MI_TrigFiltr) {
        _item->name = "ÔÈËÜÒÐ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_TrigFiltrDisable);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_TrigFiltrLPF);
        MenuSubItem_Init(&(_item->subItems[2]), MSI_TrigFiltrHPF);
        _item->numberSubItems = 3;
    } else if(_type == MI_MeasFreq) {
        _item->name = "×ÀÑÒÎÒÀ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasFreq1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasFreq2);
        _item->numberSubItems = 2;
    } else if(_type == MI_MeasT) { 
        _item->name = "ÏÅÐÈÎÄ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasT1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasT2);
        _item->numberSubItems =2;
    } else if(_type == MI_MeasDurationPOS) {
        _item->name = "ÄËÈÒ+";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasDurationPOS1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasDurationPOS2);
        _item->numberSubItems = 2;
    } else if(_type == MI_MeasDurationNEG) {
        _item->name = "ÄËÈÒ-";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasDurationNEG1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasDurationNEG2);
        _item->numberSubItems = 2;
    } else if(_type == MI_MeasDuty) {
        _item->name = "ÑÊÂÀÆÍÎÑÒÜ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasDuty1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasDuty2);
        _item->numberSubItems = 2;
    } else if(_type == MI_MeasDurationFront) {
        _item->name = "ÄËÈÒ. ÔÐÎÍÒÀ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasDurationFront1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasDurationFront2);
        _item->numberSubItems = 2;
    } else if(_type == MI_MeasDurationFall) {
        _item->name = "ÄËÈÒ. ÑÏÀÄÀ";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasDurationFall1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasDurationFall2);
        _item->numberSubItems = 2;
    } else if(_type == MI_MeasVampl) {
        _item->name = "Vàìïë.";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasVampl1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasVampl2);
        _item->numberSubItems = 2;
    } else if(_type == MI_MeasVpp) {
        _item->name = "Vð-ð";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasVpp1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasVpp2);
        _item->numberSubItems = 2;
    } else if(_type == MI_MeasVrms) {
        _item->name = "Vñð.";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasVaverage1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasVaverage2);
        _item->numberSubItems = 2;
    } else if(_type == MI_MeasVmax) {
        _item->name = "Vmax";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasVmax1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasVmax2);
        _item->numberSubItems = 2;
    } else if(_type == MI_MeasVaverage) {
        _item->name = "Vñð.";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasVaverage1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasVaverage2);
        _item->numberSubItems = 2;
    } else if(_type == MI_MeasVmin) {
        _item->name = "Vmin";
        MenuSubItem_Init(&(_item->subItems[0]), MSI_MeasVmin1);
        MenuSubItem_Init(&(_item->subItems[1]), MSI_MeasVmin2);
        _item->numberSubItems = 2;
    } else {
        ERROR_LOG("Unprocessing type MenuItem");
    }
    _item->subItems[0].selected = true;
};

/*
int     NumberSelectedSubItem(struct MenuItem *_item) {
    for(int i = 0; i < _item->numberSubItems; i++) {
        if(_item->subItems[i].selected) {
            return i;
        }
    }
    return -1;
}
*/

/*
void    MenuItem_ChoosePrevValue(struct MenuItem *_item) {
    int indexSelected = NumberSelectedSubItem(_item);
    indexSelected--;
    if(indexSelected < 0) {
        indexSelected = _item->numberSubItems - 1;
    }
    struct MenuSubItem *itemUnselect = &(_item->subItems[NumberSelectedSubItem(_item)]);
    struct MenuSubItem *itemSelect = &(_item->subItems[indexSelected]);
    itemUnselect->selected = false;
    itemSelect->selected = true;
}
*/

int     PositionOnScreenRel(struct MenuItem *_item) {
    return _item->position - 
        _item->page->firstItemOnScreen;
}

int     PositionOnScreenAbsY(struct MenuItem *_item) {
    return MENU_HEIGHT_TITLE + DISPLAY_TOP_GRID + PositionOnScreenRel(_item) * MENU_ITEM_HEIGHT;
}

int     HeightOpened(struct MenuItem *_item) {
    return MENU_HEIGHT_TITLE + _item->numberSubItems * MENU_SUBITEM_HEIGHT_OPENED;
}

void    MenuItem_DrawOpened(struct MenuItem *_item) {
    int y = 0,
        x = DISPLAY_RIGHT_GRID - MENU_ITEM_WIDTH_TITLE - 1;
    if(PositionOnScreenAbsY(_item) + HeightOpened(_item) < DISPLAY_BOTTOM_GRID) {
        y = PositionOnScreenAbsY(_item);
    } else {
        y = DISPLAY_BOTTOM_GRID - HeightOpened(_item) - 1;
    }
    Display_SetDrawStyle(ColorR064, ColorWHITE);
    Display_DrawRect(x, y, MENU_ITEM_WIDTH_TITLE, MENU_ITEM_HEIGHT_TITLE);
    Display_DrawText(_item->name, x, y);
    y += MENU_ITEM_HEIGHT_TITLE;
    for(int i = 0; i < _item->numberSubItems; i++) {
        struct MenuSubItem *subItem = &(_item->subItems[i]);
        MenuSubItem_DrawOpened(subItem, x, y);
        y += MENU_SUBITEM_HEIGHT_OPENED;
    }
}

int     MenuItem_PositionOnScreenRel(struct MenuItem *_item) {
    return _item->position - _item->page->firstItemOnScreen;
}

int     MenuItem_PositionOnScreenAbsY(struct MenuItem *_item) {
    return MENU_HEIGHT_TITLE + DISPLAY_TOP_GRID + MenuItem_PositionOnScreenRel(_item) * MENU_ITEM_HEIGHT;
}

void    MenuItem_Draw(struct MenuItem *_item) {
    if(MenuItem_PositionOnScreenRel(_item) < MENU_PAGE_ITEMS_ON_PAGE) {
        int x = DISPLAY_X_MENU_ITEM,
            y = MenuItem_PositionOnScreenAbsY(_item);
        Display_SetDrawStyle(ColorR238, ColorWHITE);
        Display_DrawRect(x, y, MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT);
        Display_DrawText(_item->name, x, y);

        if(_item->selected) {
            Display_SetDrawStyle(ColorWHITE, 0);
        } else {
            Display_SetDrawStyle(ColorR048, ColorR048);
        }
        int yRect = y + 17;
        Display_DrawRect(x + 3, yRect, MENU_ITEM_WIDTH_RECT_FOR_VALUE, MENU_ITEM_HEIGHT_RECT_FOR_VALUE);
        uint color = ColorWHITE;
        if(_item->selected) {
            color = ColorBLACK;
        }
        Display_DrawText(subItemNames[_item->subItems[_item->numberSubItems].type], x + 3, yRect);
    }
}

//**********************************************************************************************************
void    MenuPage_Init(struct MenuPage *_page, enum TypeMenuPage _type) {
    _page->enabled = false;
    _page->firstItemOnScreen = 0;
    
    if(_type == MP_Chan1) {
        _page->name = "ÊÀÍÀË 1";
        _page->items = itemsChan1;
        _page->numberItems = 3;
        MenuItem_Init(&(_page->items[0]), MI_Chan1enter, _page);
        MenuItem_Init(&(_page->items[1]), MI_Chan1coupling, _page);
        MenuItem_Init(&(_page->items[2]), MI_Chan1factor, _page);
    } else if(_type == MP_Chan1) {
        _page->name = "ÊÀÍÀË 2";
        _page->items = itemsChan2;
        _page->numberItems = 3;
        MenuItem_Init(&(_page->items[0]), MI_Chan2enter, _page);
        MenuItem_Init(&(_page->items[1]), MI_Chan2coupling, _page);
        MenuItem_Init(&(_page->items[2]), MI_Chan2factor, _page);
    } else if(_type == MP_Cursors) {
        _page->name = "ÊÓÐÑÎÐÛ";
        _page->items = itemsCursors;
        _page->numberItems = 5;
        MenuItem_Init(&(_page->items[0]), MI_CursChoice, _page);
        MenuItem_Init(&(_page->items[1]), MI_CursCursorsX, _page);
        MenuItem_Init(&(_page->items[2]), MI_CursMeasT, _page);
        MenuItem_Init(&(_page->items[3]), MI_CursCursorxY, _page);
        MenuItem_Init(&(_page->items[4]), MI_CursMeasV, _page);
    } else if(_type == MP_Display) {
        _page->name = "ÄÈÑÏËÅÉ";
        _page->items = itemsDisplay;
        _page->numberItems = 5;
        MenuItem_Init(&(_page->items[0]), MI_DisplMapping, _page);
        MenuItem_Init(&(_page->items[1]), MI_DisplGrid, _page);
        MenuItem_Init(&(_page->items[2]), MI_DisplAccum, _page);
        MenuItem_Init(&(_page->items[3]), MI_DisplUpdate, _page);
        MenuItem_Init(&(_page->items[4]), MI_DisplScreen, _page);
    } else if(_type == MP_Memory) {
        _page->name = "ÏÀÌßÒÜ";
        _page->items = itemsMemory;
        _page->numberItems = 5;
        MenuItem_Init(&(_page->items[0]), MI_MemOperation, _page);
        MenuItem_Init(&(_page->items[1]), MI_MemSource, _page);
        MenuItem_Init(&(_page->items[2]), MI_MemStorage, _page);
        MenuItem_Init(&(_page->items[3]), MI_MemMemory, _page);
        MenuItem_Init(&(_page->items[4]), MI_MemExecution, _page);
    } else if(_type == MP_Trig) {
        _page->name = "ÇÀÏÓÑÊ";
        _page->items = itemsTrig;
        _page->numberItems = 5;
        MenuItem_Init(&(_page->items[0]), MI_TrigMode, _page);
        MenuItem_Init(&(_page->items[1]), MI_TrigSource, _page);
        MenuItem_Init(&(_page->items[2]), MI_TrigPolarity, _page);
        MenuItem_Init(&(_page->items[3]), MI_TrigCoupling, _page);
        MenuItem_Init(&(_page->items[4]), MI_TrigFiltr, _page);
    } else if(_type == MP_Measures) {
        _page->name = "ÈÇÌÅÐÅÍÈß";
        _page->items = itemsMeasures;
        _page->numberItems = 13;
        MenuItem_Init(&(_page->items[0]), MI_MeasFreq, _page);
        MenuItem_Init(&(_page->items[1]), MI_MeasT, _page);
        MenuItem_Init(&(_page->items[2]), MI_MeasDurationPOS, _page);
        MenuItem_Init(&(_page->items[3]), MI_MeasDurationNEG, _page);
        MenuItem_Init(&(_page->items[4]), MI_MeasDuty, _page);
        MenuItem_Init(&(_page->items[5]), MI_MeasDurationFront, _page);
        MenuItem_Init(&(_page->items[6]), MI_MeasDurationFall, _page);
        MenuItem_Init(&(_page->items[7]), MI_MeasVampl, _page);
        MenuItem_Init(&(_page->items[8]), MI_MeasVpp, _page);
        MenuItem_Init(&(_page->items[9]), MI_MeasVrms, _page);
        MenuItem_Init(&(_page->items[10]), MI_MeasVaverage, _page);
        MenuItem_Init(&(_page->items[11]), MI_MeasVmax, _page);
        MenuItem_Init(&(_page->items[12]), MI_MeasVmin, _page);
    }

    _page->items[0].selected = true;
    _page->openedItem = 0;

    for(int i = 0; i < _page->numberItems; i++) {
        _page->items[i].position = i;
    }
};

void    MenuPage_Draw(struct MenuPage *_page) {
    if(!_page->enabled) {
        return;
    }

    Display_SetDrawStyle(ColorR211, ColorWHITE);
    Display_DrawRect(DISPLAY_X_MENU_PAGE, DISPLAY_Y_MENU_PAGE, DISPLAY_WIDTH_TITLE_MENU_PAGE, MENU_HEIGHT_TITLE);
    Display_DrawText(_page->name, DISPLAY_X_MENU_PAGE, DISPLAY_Y_MENU_PAGE);

    for(int i = 0; i < _page->numberItems; i++) {
        MenuItem_Draw(&(_page->items[i]));
    }
}

//**********************************************************************************************************
void    Menu_Init() {

    MenuPage_Init(&pages[MP_Chan1],   MP_Chan1);
    MenuPage_Init(&pages[MP_Chan2],   MP_Chan2);
    MenuPage_Init(&pages[MP_Cursors], MP_Cursors);
    MenuPage_Init(&pages[MP_Display], MP_Display);
    MenuPage_Init(&pages[MP_Memory],  MP_Memory);
    MenuPage_Init(&pages[MP_Trig],    MP_Trig);
    MenuPage_Init(&pages[MP_Measures],MP_Measures);

    numberCurPage = MP_Chan1;
    curPage = &(pages[numberCurPage]);
    buttonTimer = -1;
}

void    Menu_Draw() {
    Display_Clear();
    Display_DrawGrid();
    Display_DrawCursors();
    MenuPage_Draw(curPage);
}

void    Menu_PressButton(enum Button _button) {
    buttonTimer = _button;

    switch(_button) {
        case B_Menu:
            curPage->enabled = !curPage->enabled;
            break;
        case B_F1:  
        case B_F2:  
        case B_F3:  
        case B_F4:  
        case B_F5:
            if(curPage->enabled) {
                struct MenuItem *itemUnderKey = &(curPage->items[curPage->firstItemOnScreen + _button - 1]);
                if(itemUnderKey) {
                    if(!curPage->openedItem || itemUnderKey == curPage->openedItem) {
                        for(int i = 0; i < curPage->numberItems; i++) {
                            curPage->items[i].selected = false;
                        }
                        itemUnderKey->selected = true;
                        int indexSelected = -1;
                        for(int i = 0; i < itemUnderKey->numberSubItems; i++) {
                            if(itemUnderKey->subItems[i].selected) {
                                indexSelected = i;
                                break;
                            }
                        }
                        itemUnderKey->subItems[indexSelected].selected = false;
                        indexSelected++;
                        if(indexSelected > (itemUnderKey->numberSubItems - 1)) {
                            indexSelected = 0;
                        }
                        itemUnderKey->subItems[indexSelected].selected = true;
                    } else if(curPage->openedItem != itemUnderKey) {
                        curPage->openedItem = 0;
                    }
                }
            }
            break;
        case B_Chan1:   
        case B_Chan2:   
        case B_Trig:    
        case B_Cursors: 
        case B_Display: 
        case B_Measures:    
        case B_Service: 
        case B_Memory:  
        case B_Info:
            if(numberCurPage == _button) {
                curPage->enabled = !curPage->enabled;
            } else {
                numberCurPage = _button;
                curPage = &(pages[_button]);
                curPage->enabled = true;
            };
            break;
        default:
            break;
    }

    Menu_Draw();
};

void    Menu_ReleaseButton(enum Button _button) {

};

void    Menu_RotateRegulatorRight(enum Regulator _reg) {

};

void    Menu_RotateRegulatorLeft(enum Regulator _reg) {

};
