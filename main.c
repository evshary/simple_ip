#include <stdlib.h>
#include <stdio.h>
#include "parse_pcap.h"
#include "net/interf.h"

int main(int argc, char *argv[])
{
    unsigned char *payload = NULL;

    if (argc != 2) {
        printf("FORMAT: ./main [pcap_file_name]\n");
        return -1;
    }
    printf("The file name is %s\n", argv[1]);
    if (PCAP_PARSE_SUCCESS != parse_pcap_file(argv[1])) {
        printf("%s-%d: Parse the pcap file error.\n", __func__, __LINE__);
        return -1;
    }
    Interface eth0;
    add_interface(&eth0, "eth0", "\00\11\22\33\44\55");
    do {
        int length;
        length = get_next_pkt(&payload);
        parse_packet(payload, length, &eth0);
    } while (payload != NULL);

    if (PCAP_PARSE_SUCCESS != free_pcap_file()) {
        printf("%s-%d: Free the pcap file error.\n", __func__, __LINE__);
        return -1;
    }
    return 0;
}
