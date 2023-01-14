#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "output.h"
#include "type.h"

void print_eth(char *buffer)
{
	struct eth_hdr *eth = (struct eth_hdr *)buffer;
	int ind;

	printf("ETH source      : ");
	for (ind = 0; ind < 5; ind += 1)
		printf("%.2x:", eth->src[ind]);
	printf("%.2x\n", eth->src[5]);

        printf("ETH destination : ");
        for (ind = 0; ind < 5; ind += 1)
                printf("%.2x:", eth->dst[ind]);
        printf("%.2x\n", eth->dst[5]);

	printf("ETH protocol    : %d\n", eth->protocol);

}

void print_arp(char *buffer)
{
	struct arp_hdr *arph = (struct arp_hdr * )(buffer + sizeof(struct eth_hdr));
	int ind;

	printf("ARP source mac  : ");
	for (ind = 0; ind < 5; ind += 1)
		printf("%.2x:", arph->src_mac[ind]);
	printf("%.2x\n", arph->src_mac[5]);

	printf("ARP target mac  : ");
	for (ind = 0; ind < 5; ind += 1)
		printf("%.2x:", arph->dst_mac[ind]);
	printf("%.2x\n", arph->dst_mac[5]);

	printf("ARP source ip   : %u.%u.%u.%u\n",
		arph->src_ip[0], arph->src_ip[1], arph->src_ip[2], arph->src_ip[3]);
	printf("ARP target ip   : %u.%u.%u.%u\n",
		arph->dst_ip[0] ,arph->dst_ip[1], arph->dst_ip[2], arph->dst_ip[3]);

	printf("ARP operation   : ");
	if (arph->oper == htons(1)) printf("request\n");
	else if (arph->oper == htons(2)) printf("reply\n");
	else printf("%d\n", arph->oper);
}

void print_ip(char *buffer)
{
	struct sockaddr_in src, dst;
	struct ip_hdr *iph = (struct ip_hdr *)buffer;

	src.sin_addr.s_addr = iph->src;
	dst.sin_addr.s_addr = iph->dst;

	printf("IP source       : %s\n", inet_ntoa(src.sin_addr));
	printf("IP destination  : %s\n", inet_ntoa(dst.sin_addr));
	printf("IP tos          : %d\n", iph->service);
	printf("IP ttl          : %d\n", iph->ttl);
	printf("IP id           : %d\n", iph->ident);
	printf("IP check        : %x\n", iph->check);
}

void print_icmp(char *buffer)
{
	struct icmp_hdr *icmph = (struct icmp_hdr *)(buffer + sizeof(struct ip_hdr));

	printf("ICMP type       : ");
	if (icmph->type == 0) printf("echo reply\n");
	if (icmph->type == 3) printf("destination unreachable\n");
	if (icmph->type == 4) printf("source quench\n");
	if (icmph->type == 5) printf("redirect");
	if (icmph->type == 8) printf("echo request\n");
	if (icmph->type == 9) printf("router advertisement\n");
	if (icmph->type == 10) printf("router selection\n");
	if (icmph->type == 11) printf("time exceeded\n");
	if (icmph->type == 12) printf("parameter problem\n");
	if (icmph->type == 13) printf("timestamp\n");
	if (icmph->type == 14) printf("timestamp reply\n");
	if (icmph->type == 15) printf("information request\n");
	if (icmph->type == 16) printf("information reply\n");
	if (icmph->type == 17) printf("address mask request\n");
	if (icmph->type == 18) printf("address mask reply\n");
	if (icmph->type == 30) printf("traceroute\n");	
	printf("ICMP code       : %d\n", icmph->code);
	printf("ICMP id         : %d\n", icmph->id);
	printf("ICMP seq        : %d\n", icmph->seq);
	printf("ICMP check      : %x\n", icmph->check);
}

void print_tcp(char *buffer)
{
	struct tcp_hdr *tcph = (struct tcp_hdr *)(buffer + sizeof(struct ip_hdr));

	printf("TCP source      : %d\n", htons(tcph->src));
	printf("TCP destination : %d\n", htons(tcph->dst));
	printf("TCP seq         : %d\n", tcph->seq);
	printf("TCP ack         : %d\n", tcph->ack);
	printf("TCP flag        : ");
	if ((tcph->flag | 1) == tcph->flag) printf("fin,");
	if ((tcph->flag | 2) == tcph->flag) printf("syn,");
	if ((tcph->flag | 4) == tcph->flag) printf("rst,");
	if ((tcph->flag | 8) == tcph->flag) printf("psh,");
	if ((tcph->flag | 16) == tcph->flag) printf("ack,");
	if ((tcph->flag | 32) == tcph->flag) printf("urg,");
	printf("\n");
	printf("TCP check       : %x\n", tcph->check);
}

void print_udp(char *buffer)
{
	struct udp_hdr *udph = (struct udp_hdr *)(buffer + sizeof(struct ip_hdr));

	printf("UDP source      : %d\n", htons(udph->src));
	printf("UDP destination : %d\n", htons(udph->dst));
	printf("UDP checksum    : %x\n", udph->check);
}
