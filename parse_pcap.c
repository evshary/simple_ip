#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include "pcap.h"
#include "parse_pcap.h"

#define PCAP_MAGIC_CODE "\xd4\xc3\xb2\xa1"
#define MAX_BUF_LEN 512

typedef struct _PKT_LIST {
    struct _PKT_LIST *pkt_next;
    struct pcap_pkthdr pkt_header;
    unsigned char *pkt_content;
} PKT_LIST;

PKT_LIST *pkt_head = NULL;
PKT_LIST *pkt_now = NULL;
PKT_LIST *pkt_end = NULL;

void show_pcap_header_info(struct pcap_file_header *p_header)
{
    printf("Major version: %u\n", p_header->version_major);
    printf("Minor version: %u\n", p_header->version_minor);
    printf("This zone: %u\n", p_header->thiszone);
    printf("Accuracy of timestamps: %u\n", p_header->sigfigs);
    printf("Max len of each pkt: %u\n", p_header->snaplen);
    printf("Link type: %s\n", (p_header->linktype == 1) ? "Ethernet" : "Others");
}

void show_pkt_info(struct pcap_pkthdr *pkt_header)
{
    printf("Time: sec:%u msec:%u\n", pkt_header->ts.tv_sec, pkt_header->ts.tv_usec);
    printf("Capture length: %u\n", pkt_header->caplen);
    printf("Real length: %u\n", pkt_header->len);
}

int parse_pcap_file(char *file_name)
{
    FILE *fp;
    struct pcap_file_header p_header;
    PKT_LIST *pkt_tmp;

    if (NULL == file_name) {
        printf("%s-%d: File name is NULL.", __func__, __LINE__);
        return PCAP_PARSE_NO_FILE;
    }

    fp = fopen(file_name, "rb");
    if (fp == NULL) {
        printf("%s-%d: File can't be opened.", __func__, __LINE__);
        return PCAP_PARSE_FORMAT;
    }

    fread(&p_header, sizeof(struct pcap_file_header), 1, fp);
    if (memcmp(&p_header.magic, PCAP_MAGIC_CODE, 4) != 0) {
        printf("%s-%d: File's format error.", __func__, __LINE__);
        return PCAP_PARSE_FORMAT;
    }

    show_pcap_header_info(&p_header);

    while (!feof(fp)) {
        // malloc for pkt header
        pkt_tmp = (PKT_LIST *)malloc(sizeof(PKT_LIST));
        pkt_tmp->pkt_next = NULL;
        fread(&pkt_tmp->pkt_header, sizeof(struct pcap_pkthdr), 1, fp);
        //show_pkt_info(&pkt_tmp->pkt_header);
        // malloc new space for pkt content
        pkt_tmp->pkt_content = (unsigned char *)malloc(pkt_tmp->pkt_header.caplen);
        fread(pkt_tmp->pkt_content, pkt_tmp->pkt_header.caplen, 1, fp);
        // add to list
        if (pkt_head == NULL) {
            pkt_head = pkt_tmp;
            pkt_end = pkt_tmp;
        } else {
            pkt_end->pkt_next = pkt_tmp;
            pkt_end = pkt_tmp;
        }
    }
    pkt_now = pkt_head;

    fclose(fp);
    return PCAP_PARSE_SUCCESS;
}

unsigned int get_next_pkt(unsigned char **payload)
{
    PKT_LIST *pkt_tmp = pkt_now;

    if (pkt_now == NULL) {
        *payload = NULL;
        return 0;
    }
    pkt_now = pkt_now->pkt_next;
    //show_pkt_info(&pkt_tmp->pkt_header);
    *payload = pkt_tmp->pkt_content;
    return pkt_tmp->pkt_header.caplen;
}

int free_pcap_file()
{
    pkt_now = pkt_head;
    do {
        pkt_head = pkt_head->pkt_next;
        free(pkt_now->pkt_content);
        free(pkt_now);
        pkt_now = pkt_head;
    } while (pkt_now != NULL);
    return PCAP_PARSE_SUCCESS;
}
