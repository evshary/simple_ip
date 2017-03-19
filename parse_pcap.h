#ifndef _PARSE_PCAP_H
#define _PARSE_PCAP_H

#define PCAP_PARSE_SUCCESS  0
#define PCAP_PARSE_NO_FILE -1
#define PCAP_PARSE_FORMAT  -2

int parse_pcap_file(char *file_name);
int free_pcap_file();

#endif
