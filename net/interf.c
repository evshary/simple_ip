#include "interf.h"
#include "Utils.h"
#include <string.h> // Linux

static Interface *gInterface = NULL;
static pParse ParseRoute [] = {
    &parse_ethernet,
    NULL
};

#define ETHERNET_HDR_LEN       22
#define ETH_TYPE_IPV4       0x0800

void add_interface(Interface *pInterF, const char *szName, UINT8 Mac[6])
{
    if ((NULL == pInterF) ||
            (NULL == szName)) {
        return;
    }

    int NameLen = MIN(INTERF_NAME_LEN - 1, strlen(szName));

    strncpy(pInterF->szName, szName, NameLen);
    memset(pInterF->szName + NameLen, '\0', INTERF_NAME_LEN - NameLen);

    strncpy(pInterF->MacAdr, Mac, 6);

    pInterF->RxCount = 0;
    pInterF->TxCount = 0;
    pInterF->blEnabled = true;

    pInterF->pNext = gInterface;
    gInterface = pInterF;
}

void parse_packet(unsigned char *payload, unsigned int length, Interface *pInterF)
{
    Packet pData;
    pData.Len = length;
    pData.pRaw = payload;

    for (int i = 0; NULL != ParseRoute[i]; i++) {
        switch (ParseRoute[i](&pData, pInterF)) {
            case R_ERROR:
            // Log
            case R_DROP:
                return;
            case R_SUCCESS:
            default:
                break;
        }
    }
}

RESULT parse_ethernet(Packet *pData, Interface *pInterF)
{
    EthernetHeader *eth_header = pData->pRaw;
    if (pData->Len < ETHERNET_HDR_LEN) {
        return R_ERROR;
    }

    show_ethernet_info(eth_header);

    switch (eth_header->Type) {
        case ETH_TYPE_IPV4:
            // parse input
            return R_SUCCESS;
        default:
            return R_DROP;
    }
}

void print_hex(char *str, unsigned int len)
{
    int i;
    printf("0x");
    for(i=0; i<len; i++)
    {
        printf("%02x", str[i] & 0xff);
    }
}

void show_ethernet_info(EthernetHeader *eth_header)
{
    printf("Ethernet header info:\n");
    //printf("Preamble: ");
    //print_hex(eth_header->Preamble, 8);
    //printf("\n");
    printf("Dst MAC: ");
    print_hex(eth_header->DestMac, 6);
    printf("\n");
    printf("Src MAC: ");
    print_hex(eth_header->SrcMac, 6);
    printf("\n");
    printf("Type: %d\n", eth_header->Type);
}
