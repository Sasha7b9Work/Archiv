#include "Utils/GlobalFunctions.h"
#include "Globals.h"
#include "Menu/MenuItems.h"
#include "Definition.h"
#include "HelpContent.h"


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageHelp
 *  @{
 */


void DrawSB_Help_ParagraphEnter(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x4a');
    Painter::SetFont(TypeFont_8);
}

void DrawSB_Help_ParagraphLeave(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x48');
    Painter::SetFont(TypeFont_8);
}

void DrawSB_Help_ParagraphPrev(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 5, '\x4c');
    Painter::SetFont(TypeFont_8);
}

void DrawSB_Help_ParagraphNext(int x, int y)
{
    Painter::SetFont(TypeFont_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 5, '\x4e');
    Painter::SetFont(TypeFont_8);
}

void OnHelpRegSet(int angle) 
{

}

extern const Page mpHelp;

const SmallButton sbHelpParagraphEnter
(
    &mpHelp, HelpContent_EnterParagraphIsActive,
    "???????", "Open",
    "????????? ?????? ???????",
    "Opens the section of the reference",
    HelpContent_EnterParagraph,
    DrawSB_Help_ParagraphEnter
);

const SmallButton sbHelpParagraphLeave
(
    &mpHelp, HelpContent_LeaveParagraphIsActive,
    "???????", "Close",
    "????????? ?????? ???????",
    "Closes the section of the reference",
    HelpContent_LeaveParagraph,
    DrawSB_Help_ParagraphLeave
);

const SmallButton sbHelpParagraphPrev
(
    &mpHelp, 0,
    "?????????? ??????", "Previous section",
    "??????? ?????????? ?????? ???????",
    "To choose the previous section of the reference",
    HelpContent_PrevParagraph,
    DrawSB_Help_ParagraphPrev
);

const SmallButton sbHelpParagraphNext
(
    &mpHelp, 0,
    "", "",
    "",
    "",
    HelpContent_NextParagraph,
    DrawSB_Help_ParagraphNext
);

static void PressSB_Help_Exit()
{
    Display::RemoveAddDrawFunction();
}

const SmallButton sbExitHelp
(
    &mpHelp,
    COMMON_BEGIN_SB_EXIT,
    PressSB_Help_Exit,
    DrawSB_Exit
);

static const arrayItems itemsHelp =
{
    (void*)&sbExitHelp,
    (void*)&sbHelpParagraphEnter,
    (void*)&sbHelpParagraphLeave,
    (void*)0,
    (void*)&sbHelpParagraphPrev,
    (void*)&sbHelpParagraphNext    
};

static const Page mpHelp            ///< ??????
(
    &mainPage, 0,
    "??????", "HELP",
    "??????? ??????? ??????",
    "To open sections of the help",
    Page_SB_Help, &itemsHelp, EmptyFuncVV, HelpContent_Draw, OnHelpRegSet
);


//----------------------------------------------------------------------------------------------------------------------------------------------------
void *PageHelp::GetPointer()
{
    return (void *)&mpHelp;
}


/** @}  @}
 */
