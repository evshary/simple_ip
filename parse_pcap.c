#include "pcap.h"
#include "parse_pcap.h"

int parse_pcap_file(char *file_name)
{
    if (file_name == 0)
        return PCAP_PARSE_NO_FILE;
    return 0;
}

int free_pcap_file()
{
    return 0;
}
