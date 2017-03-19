#include <stdlib.h>
#include <stdio.h>
#include "parse_pcap.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("FORMAT: ./main [pcap_file_name]\n");
        return -1;
    }
    printf("The file name is %s\n", argv[1]);
    if (PCAP_PARSE_SUCCESS != parse_pcap_file(argv[1])) {
        printf("%s-%d: Parse the pcap file error.\n", __func__, __LINE__);
        return -1;
    }

    if (PCAP_PARSE_SUCCESS != free_pcap_file()) {
        printf("%s-%d: Free the pcap file error.\n", __func__, __LINE__);
        return -1;
    }
    return 0;
}
