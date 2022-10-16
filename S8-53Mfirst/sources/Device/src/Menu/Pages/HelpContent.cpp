// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Menu/Pages/HelpContent.h"
#include "Menu/Pages/HelpContentPages.h"
#include "Settings/Settings.h"


pchar PageHelpContent::stringForHint = 0;
const Item *PageHelpContent::itemHint = 0;

const PageHelpItem PageHelpItem::empty(&Item::emptyData);


int currentParagraph = 0;   // ���� TypePage(currentPage) == TypePageHelp::Content, �� ��������� �� ������� ������ ����������
const PageHelpItem *currentPage = PageHelpContent::PageMain::self;


static const int WIDTH = 295;


static void DrawPageContent()
{
    int y = 50;
    Text(TITLE(currentPage)).DrawInCenterRect(0, y, WIDTH, 10);

    int numPage = 0;

    y += 40;

    while(currentPage->OwnData()->pages[numPage])
    {
        Page *page = (Page*)currentPage->OwnData()->pages[numPage];
        pchar title = TITLE(page);
        if(currentParagraph == numPage)
        {
            Text(title).DrawInCenterRectOnBackground(0, y, WIDTH, 10, Color::BACK, 2, Color::FILL);
        }
        else
        {
            Text(title).DrawInCenterRect(0, y, WIDTH, 10, Color::FILL);
        }
        y += 16;
        numPage++;
    }
}

static void DrawPageDescription()
{
    Text(TITLE(currentPage)).DrawInCenterRect(0, 3, WIDTH, 10);
    Text(HINT(currentPage)).DrawInRectWithTransfers(2, 15, WIDTH - 5, 240);
}


void PageHelpContent::Draw()
{
    uint startTime = TIMER_MS;
    Region(319 - Grid::Right(), 20).Fill(Grid::Right(), 0, Color::BACK);
    Region(319 - Grid::Right(), 21).Fill(Grid::Right(), 219);
    Region(WIDTH, 237).Fill(1, 1);
    Rectangle(WIDTH + 2, 239).Draw(0, 0, Color::FILL);

    if(currentPage->OwnData()->type == TypePageHelp::Content)
    {
        DrawPageContent();
    }
    else if(currentPage->OwnData()->type == TypePageHelp::Description)
    {
        DrawPageDescription();
    }
    Text("%d", TIMER_MS - startTime).Draw(2, 230, Color::FILL);
}

static int NumParagraphs(const PageHelpItem *page)
{
    int retValue = 0;
    while(page->OwnData()->pages[retValue])
    {
        retValue++;
    }
    return retValue;
}

void PageHelpContent::NextParagraph()
{
    if(currentPage->OwnData()->type == TypePageHelp::Content)
    {
        Math::CircleIncrease<int>(&currentParagraph, 0, NumParagraphs(currentPage) - 1);
    }
}

void PageHelpContent::PrevParagraph()
{
    if(currentPage->OwnData()->type == TypePageHelp::Content)
    {
        Math::CircleDecrease<int>(&currentParagraph, 0, NumParagraphs(currentPage) - 1);
    }
}

void PageHelpContent::EnterParagraph()
{
    if(currentPage->OwnData()->type == TypePageHelp::Content)
    {
        currentPage = (const PageHelpItem *)currentPage->OwnData()->pages[currentParagraph];
    }
    currentParagraph = 0;
}

void PageHelpContent::LeaveParagraph()
{
    currentParagraph = 0;
    if(currentPage->OwnData()->parent)
    {
        currentPage = (const PageHelpItem *)currentPage->OwnData()->parent;
    }
}

bool PageHelpContent::LeaveParagraphIsActive()
{
    return currentPage->OwnData()->parent != 0;
}

bool PageHelpContent::EnterParagraphIsActive()
{
    return currentPage->OwnData()->type == TypePageHelp::Content;
}

void PageHelpContent::SetItemForHint(const Item *item)
{
    stringForHint = 0;
    itemHint = item;
}
