#ifndef INTERF_H
#define INTERF_H

#include "stdbool.h"

#define INTERF_NAME_LEN	64
#define UINT8			unsigned char
#define UINT16			unsigned short

typedef enum RESULT {
    R_ERROR = -1,
    R_SUCCESS,
    R_DROP
} RESULT;

typedef struct Interface {
    char szName[INTERF_NAME_LEN];
    UINT8 MacAdr[6];
    int RxCount;
    int TxCount;
    bool blEnabled;

    struct Interface *pNext;
} Interface;

typedef struct EthernetFrame {
    UINT8 Preamble[8];
    UINT8 DestMac[6];
    UINT8 SrcMac[6];
    union {
        UINT16 PayloadLen;
        UINT16 Type;
    };
    UINT8 *pPayload;
    UINT8 Checksum[4];
} EthernetFrame;

typedef struct Packet {
    unsigned int Len;
    EthernetFrame Frame;

    UINT8 *pRaw;
} Packet;

typedef RESULT(*pParse)(Packet *, Interface *);

void add_interface(Interface *pInterF, const char *szName, UINT8 Mac[6]);
void parse_packet(Packet *pData, Interface *pInterF);
RESULT parse_ethernet(Packet *pData, Interface *pInterF);

#endif //INTERF_H
