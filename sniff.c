#include <stdio.h>
#include <string.h> // memset
#include <stdlib.h> // exit
#include <unistd.h> // getopt
#include <signal.h> // Ctrl+C handling
#include <arpa/inet.h> // struct in_addr
#include <pcap.h> // libpcap



pcap_t *pcap_session = NULL; // libpcap session handle

// ctrl+c interrupt hanlder
void bypass_sigint(int sig_no) {
    printf("**\nCapture process interrupted by user...\n");
    if (pcap_session != NULL) {
        pcap_close(pcap_session);
    }
    exit(0); // we're done
}

int main(int argc, char *argv[]) {
    char *device = NULL;  // device to sniff
    char argch;           // to manage command line arguments
    char errbuf[PCAP_ERRBUF_SIZE];        // to handle libpcap error message
    int  siz = 1518,    // max number of bytes captured for each datagram
         promisc = 0;   // deactive promiscuous mode?? promiscuous mode??


    // install ctrl+c handle
    struct sigaction sa, osa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler  = &bypass_sigint;
    sigaction(SIGINT, &sa, &osa);

    while((argch = getopt(argc, argv, "hpd:")) != EOF) {
        switch(argch) {
            case 'd': // device name
                device = optarg;
                break;
            
            case 'h':
                printf("Usage: sniff [-d XXX -h]\n");
                printf("-d XXX: device to capture from, where XXX is device name (ex: eth0).\n");
                printf("-h : show this information.\n");
                printf("-p : active promiscuous capture mode.\n");

                if (argc == 2) return 0;
                break;
            case 'p':
                promisc = 1;
                break;
        }
    }

    // identify device to use
    if (device == NULL && (device = pcap_lookupdev(errbuf)) == NULL) {
        fprintf(stderr, "error - %s", errbuf);
        return -2;
    } else {
        printf("device = %s %s\n", device, promisc ? " (promiscuous)": "");
    }

    // extract IP informaiton for network connected to device
    bpf_u_int32 netp,   // ip address of network
                maskp;  // network mask
    if ((pcap_lookupnet(device, &netp, &maskp, errbuf)) == -1) {
        fprintf(stderr, "error - %s\n", errbuf);
        return -3;
    }

    // translate ip address into textual form for display
    struct in_addr addr;
    char *net;
    addr.s_addr = netp;
    if ((net = inet_ntoa(addr)) == NULL) {
        fprintf(stderr, "error - inet_ntoa() failed");
    } else {
        printf("network ip = %s\n", net);
    }

    // Translate network mask int textual for for display
    char *mask;
    addr.s_addr = maskp;
    if ((mask = inet_ntoa(addr)) == NULL) {
        fprintf(stderr, "error - inet_ntoa() failed\n");
    } else {
        printf("network mask = %s\n", mask);
    }

    // Open a libpcap capture session
    pcap_session = pcap_open_live(device, siz, promisc, 1000, errbuf);
    
    if (pcap_session == NULL) {
        fprintf(stderr, "error - pcap_open_live() failed: %s", errbuf);
        return -4;
    }

    // capture code will be inserted below

    // close the session
    pcap_close(pcap_session);
        
    return 0;
}