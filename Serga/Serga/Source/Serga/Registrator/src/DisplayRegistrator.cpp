#include "DisplayRegistrator.h"
#pragma warning(disable:4127)
#include <QColorDialog>
#pragma warning(default:4127)
#include <cmath>
#include "../../common/defines.h"
#include "SignalScreenRegistrator.h"

using std::vector;

DisplayRegistrator::DisplayRegistrator(QFrame *_parent, int _channel, std::vector<Data> &_rDatas):
    QFrame(_parent),
    signalScreen(NULL)
{

    frame = _parent;

    signalScreen = new SignalScreenRegistrator(this, &settings, _channel, _rDatas);
    signalScreen->move(2, 2);
    signalScreen->show();

    connect(signalScreen, SIGNAL(MoveSignal(int)), this, SLOT(SlotMoveSignal(int)));
}

DisplayRegistrator::~DisplayRegistrator() 
{
    SAFE_DELETE(signalScreen);
}

void DisplayRegistrator::DrawData(int lastPoint) 
{
    signalScreen->Draw(lastPoint);
}

void DisplayRegistrator::Reset() 
{
    signalScreen->Reset();
}

void DisplayRegistrator::ConnectToDevice(bool _connected) 
{
    signalScreen->ConnectToDevice(_connected);
}

void DisplayRegistrator::SetColorBackground(const QColor _color) 
{
    if(_color.isValid()) 
    {
        settings.colorBackground = _color;
        signalScreen->Repaint();
    }
}

void DisplayRegistrator::SetColorGrid(const QColor _color) 
{
    if(_color.isValid()) 
    {
        settings.colorGrid = _color;
        signalScreen->Repaint();
    }
}

void DisplayRegistrator::SetColorSignal(const QColor _color) 
{
    if(_color.isValid()) 
    {
        settings.colorSignal[0] = _color;
        signalScreen->Repaint();
    }
}

void DisplayRegistrator::SetColorCurs(const QColor /*_color*/) 
{
}

int DisplayRegistrator::GetHeight() 
{
    return signalScreen->GetHeight();
}

void DisplayRegistrator::EnableUCurs(bool _enable) 
{
    settings.enableUCurs = _enable;
    signalScreen->Repaint();
}

void DisplayRegistrator::SetPosUCur(int _numCur, int _pos) 
{
    settings.posUCur[_numCur] = _pos;
    signalScreen->Repaint();
}

void DisplayRegistrator::SlotMoveSignal(int delta) 
{
    emit SignalMoveSignal(delta);
}
