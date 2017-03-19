#ifndef _PCAP_H
#define _PCAP_H

#define int32 int
#define uint32 unsigned int
#define uint16 unsigned short

struct pcap_file_header {
    uint32 magic;
    uint16 version_major;
    uint16 version_minor;
    int32 thiszone;
    uint32 sigfigs;
    uint32 snaplen;
    uint32 linktype;
};

struct pkt_timeval {
    uint32 tv_sec;   // second
    uint32 tv_usec;  // micro second
};

struct pcap_pkthdr {
    struct pkt_timeval ts;
    uint32 caplen;
    uint32 len;
};

#endif
