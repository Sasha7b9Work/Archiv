#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PageMeasures
{
public:
    static const PageBase *pointer;

    static const PageBase *GetPageFrequencyCounter();

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class PageAuto
    {
    public:
        static const PageBase *pointer;

        //--------------------------------------------------------------------------------------------------------------------------------------------
        class PageTune
        {
        public:
            static const PageBase *pointer;
        };
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class PageCursors
    {
    public:
        static const PageBase *pointer;

        //--------------------------------------------------------------------------------------------------------------------------------------------
        class PageSet
        {
        public:
            static const PageBase *pointer;

            static void Draw_Set_Channel(int x, int y);

            static void OnPress_Set_Channel();
            /// ������� ��������� ������
            static void IncCursCntrlU(Chan ch);
            /// ������� ��������� ������
            static void IncCursCntrlT(Chan ch);

            static void OnPress_Set_U();

            static void OnPress_Set_T();

            static void SetCursPos100(Chan ch);
            /// �������� �������� ������� ������� ���������� �� delta �����
            static void SetShiftCursPosU(Chan ch, int numCur, float delta);
            /// �������� �������� ������� ������� ������� �� delta �����
            static void SetShiftCursPosT(Chan ch, int numCur, float delta);

            static bool OnRegSet(int angle);

            static void UpdateCursorsForLook();
            /// ���������� ������� ������� ����������
            static void SetCursorU(Chan ch, int numCur, float pos);
            /// ���������� ������� ������� �������
            static void SetCursorT(Chan ch, int numCur, float pos);
            /// ���������� true, ���� �������� ����� ��������� ����� ���������� �������/�������
            static bool IsRegSetActiveOnCursors();

        private:
            static const float MAX_POS_U;
            static const float MAX_POS_T;

            /// ���������� �������� ��������� ���������
            static void SetCursSource(Chan ch);
        };
    };
};
