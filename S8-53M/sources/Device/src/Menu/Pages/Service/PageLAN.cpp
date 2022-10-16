// 2021/05/12 15:22:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Warnings.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


static uint16 portMask = 0;
static uint16 portGateway = 0;


static void OnChanged_Enable(bool)
{
    Warnings::ShowWarningGood("��� ���������� ��������� � ����", "FOR THE INTRODUCTION OF CHANGES");
    Warnings::ShowWarningGood("��������� ������", "TURN OFF OSCILLOSCOPE");
}


DEF_CHOICE_2(cEnable, PageService::PageEthernet::self,
    "Ethernet", "Ethernet"
    ,
    "����� ������������� ethernet, �������� \"��������\" � ��������� ������.\n"
    "����� ��������� ethernet, �������� \"���������\" � ��������� ������."
    ,
    "To involve ethernet, choose \"Included\" and switch off the device.\n"
    "To disconnect ethernet, choose \"Disconnected\" and switch off the device."
    ,
    "��������", "Included",
    "���������", "Disconnected",
    set.LAN.enabled, nullptr, OnChanged_Enable, nullptr
)


DEF_IPADDRESS(ipIP, PageService::PageEthernet::self,
    "IP �����", "IP-address",
    "��������� IP ������",
    "Set of IP-address",
    IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3, set.LAN.port, OnChanged_Enable
)


DEF_IPADDRESS(ipMask, PageService::PageEthernet::self,
    "����� �������", "Network mask",
    "��������� ����� �������",
    "Set of network mask",
    NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3, portMask, OnChanged_Enable
)


DEF_IPADDRESS(ipGateway, PageService::PageEthernet::self,
    "����", "Gateway",
    "��������� ������ ��������� �����",
    "Set of gateway address",
    GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3, portGateway, OnChanged_Enable
)


DEF_MACADDRESS(macMAC, PageService::PageEthernet::self,
    "��� �����", "MAC-address",
    "��������� ����������� ������",
    "Set of MAC-address",
    MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5, OnChanged_Enable
)


DEF_PAGE_5(pageEthernet, PageService::self, NamePage::ServiceEthernet,
    "ETHERNET", "ETHERNET",
    "��������� ethernet",
    "Settings of ethernet",
    cEnable,
    ipIP,
    ipMask,
    ipGateway,
    macMAC,
    nullptr, nullptr, nullptr, nullptr
)


const Page *PageService::PageEthernet::self = &pageEthernet;
