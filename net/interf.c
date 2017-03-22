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

void parse_packet(Packet *pData, Interface *pInterF)
{
    for (int i = 0; NULL != ParseRoute[i]; i++) {
        switch (ParseRoute[i](pData, pInterF)) {
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
    if (pData->Len < ETHERNET_HDR_LEN) {
        return R_ERROR;
    }

    memcpy(&pData->Frame, pData->pRaw, ETHERNET_HDR_LEN);
    pData->Frame.pPayload = pData->pRaw + ETHERNET_HDR_LEN;

    switch (pData->Frame.Type) {
        case ETH_TYPE_IPV4:
            // parse input
            return R_SUCCESS;
        default:
            return R_DROP;
    }
}
