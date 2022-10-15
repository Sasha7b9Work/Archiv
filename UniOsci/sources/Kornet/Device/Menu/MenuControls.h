#pragma once
#include "defines.h"
#include "Display/Grid.h"
#include "Keyboard/Buttons.h"
#include "Utils/CommonFunctions.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Control
{
public:
    virtual void Draw(int, int) const {};

    virtual void PressKey(Key) const {};

    static const int CONTROLS_ON_SUBPAGE = 5;

    static const int HEIGHT = Grid::SIZE_CELL * 4 / 3 - 3;

    static int Width();

protected:
    Control(char *titleRu, char *titleEn)
    {
        title_[0] = titleRu;
        title_[1] = titleEn;
    };

    char *title_[2];

    char *Title() const;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EmptyControl : public Control
{
public:
    EmptyControl() : Control("", "") {};
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Page : public Control
{
public:
    Page(char *titleRu = 0, char *titleEn = 0, const Control **controls_ = 0, int numControls_ = 0) : 
        Control(titleRu, titleEn), controls(controls_), numControls(numControls_), firstControl(0)  {};

    virtual void Draw(int x, int y) const;

    virtual void PressKey(Key key) const;

private:
    const Control *ControlUnderKey(Key key) const;

    const Control **controls;

    int numControls;

    int firstControl;       // Номер элемента управления, который соответствует кнопке F1

    static EmptyControl emptyControl;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Choice : public Control
{
public:
    Choice(char *titleRu, char *titleEn, const char **items_, void *cell_, uint8 num, pFuncVV funcOnChanged_ = EmptyFuncVV,
           const char **items_2 = 0, void *cell_2 = 0, uint8 num2_ = 0) : 
        Control(titleRu, titleEn), funcOnChanged(funcOnChanged_)
        {
            items0 = items_;
            cells[0] = (uint8 *)cell_;
            max[0] = (uint8)(num - 1);
            
            items1 = items_2;
            cells[1] = (uint8 *)cell_2;
            max[1] = (uint8)(num2_ - 1);

            numAlternate = 0;
        };

    void SetAlternateMode(int numAlternate);

private:
    virtual void PressKey(Key key) const;

    virtual void Draw(int x, int y) const;

    char *NameItem() const;
    
    char *title[2];

    const char **items0;

    const char **items1;

    uint8 *cells[2];
    ///< *cell изменяется от 0 до max
    uint8 max[2];
    /// Вызывается при изменении значения
    pFuncVV funcOnChanged;

    int numAlternate;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Button : public Control
{
public:
    Button(char *titleRu, char *titleEn, pFuncVB funcOnPress_) : Control(titleRu, titleEn), funcOnPress(funcOnPress_) {};

private:
    virtual void PressKey(Key key) const;

    virtual void Draw(int x, int y) const;

    pFuncVB funcOnPress;
};
