#pragma once


class wxString;
class wxXmlNode;


class XML
{
public:

    // ���������� ������� node � ������ name, ���� ������� ����������
    static wxXmlNode *FindChildren(const wxXmlNode *node, const wxString &name);
};
