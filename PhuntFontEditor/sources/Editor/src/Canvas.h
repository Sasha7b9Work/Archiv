#pragma once
#include "Font/BitmapFont.h"
#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/textfile.h>
#pragma warning(pop)


struct BitmapFont;


class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent);

    void OnPaint(wxPaintEvent &);

    void Rebuild();
    
    // ��������� �������
    void ZoomUp();
    
    // ��������� �������
    void ZoomDown();
    
    // �������� �������, �� �������� � �������� �����
    void ClearBadSymbols();

    void ImportFont(wxTextFile &file, const wxString &nameFont);
    
    // ��������/��������� ����� ������ ��������
    void EnableModeSelectSymbols(bool enable);

    BitmapFont *GetFont();

private:

    void OnMouseMove(wxMouseEvent &);

    void OnMouseLeave(wxMouseEvent &);

    void OnMouseLeftDown(wxMouseEvent &);

    void OnMouseLeftUp(wxMouseEvent &);

    void OnMouseRightDown(wxMouseEvent &);

    void OnMouseRightUp(wxMouseEvent &);

    void TuneScrollBar();

    void Resize();

    void DrawCursor(wxPaintDC &dc);
    
    // ���������� ����� �������, �� ������� ��������� ����
    void HighlightPixel(wxPaintDC &dc);
    
    // ��������/����� ��������� � �������, ������� ���������� ��� ��������
    void HighlightSymbol(wxPaintDC &dc);
    
    // ����������� ������ ������
    wxSize CurrentSize();

    wxScrolledWindow *sw = nullptr;

    enum Mode
    {
        Edit,           // �������� �����, � ������� ����� ������������� �������
        SelectSymbols   // ����� ������ ��������
    } mode = Edit;
};


extern Canvas *TheCanvas;
