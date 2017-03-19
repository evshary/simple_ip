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
    return 0;
}
