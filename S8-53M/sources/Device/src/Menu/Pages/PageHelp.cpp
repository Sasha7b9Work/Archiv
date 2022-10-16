// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "common/Display/Font/Font_.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Pages/HelpContent.h"
#include "Definition.h"


void DrawSB_Help_ParagraphEnter(int x, int y)
{
    Char('\x4a').Draw4SymbolsInRect(x + 2, y + 2, TypeFont::UGO2);
}

void DrawSB_Help_ParagraphLeave(int x, int y)
{
    Char('\x48').Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}

void DrawSB_Help_ParagraphPrev(int x, int y)
{
    Char('\x4c').Draw4SymbolsInRect(x + 2, y + 5, TypeFont::UGO2);
}

void DrawSB_Help_ParagraphNext(int x, int y)
{
    Char('\x4e').Draw4SymbolsInRect(x + 2, y + 5, TypeFont::UGO2);
}

void OnHelpRegSet(int)
{

}

DEF_SMALL_BUTTON(sbHelpParagraphEnter, PageHelp::self,
    "�������", "Open",
    "��������� ������ �������",
    "Opens the section of the reference",
    PageHelpContent::EnterParagraphIsActive, PageHelpContent::EnterParagraph, DrawSB_Help_ParagraphEnter, nullptr
)

DEF_SMALL_BUTTON(sbHelpParagraphLeave, PageHelp::self,
    "�������", "Close",
    "��������� ������ �������",
    "Closes the section of the reference",
    PageHelpContent::LeaveParagraphIsActive, PageHelpContent::LeaveParagraph, DrawSB_Help_ParagraphLeave, nullptr
)

DEF_SMALL_BUTTON(sbHelpParagraphPrev, PageHelp::self,
    "���������� ������", "Previous section",
    "������� ���������� ������ �������",
    "To choose the previous section of the reference",
    nullptr, PageHelpContent::PrevParagraph, DrawSB_Help_ParagraphPrev, nullptr
)

DEF_SMALL_BUTTON(sbHelpParagraphNext, PageHelp::self,
    "", "",
    "",
    "",
    nullptr, PageHelpContent::NextParagraph, DrawSB_Help_ParagraphNext, nullptr
)

static void PressSB_Help_Exit()
{
}

DEF_SMALL_BUTTON(sbExitHelp, PageHelp::self,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, PressSB_Help_Exit, DrawSB_Exit, nullptr
)

DEF_PAGE_SB(pageHelp, PageMain::self, NamePage::Help_SB,
    "������", "HELP",
    "������� ������� ������",
    "To open sections of the help",
    sbExitHelp,
    sbHelpParagraphEnter,
    sbHelpParagraphLeave,
    Item::empty,
    sbHelpParagraphPrev,
    sbHelpParagraphNext,
    nullptr, nullptr,  PageHelpContent::Draw, OnHelpRegSet
);


const Page *PageHelp::self = &pageHelp;
