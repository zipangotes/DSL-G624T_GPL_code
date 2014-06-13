#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <atm.h>
#include <linux/atmdev.h>
#include <linux/atmbr2684.h>

#define fetch __atmlib_fetch

/* Written by Marcell GAL <cell@sch.bme.hu> to make use of the */
/* ioctls defined in the br2684... kernel patch */
/* Compile with cc -o br2684ctl br2684ctl.c -latm */

#include <asm/types.h>		/* atlas fix */
#define SOCK_DGRAM	1	/* atlas fix */

int lastsock, lastitf;

void fatal(char *str, int i)
{
	perror(str);
	exit(-2);
};

int create_br(char *nstr)
{
	int num, err;

        /* Hack by denys to check SHOWTIME */

        char buf[256];
        FILE *f;
        
        f = fopen("/proc/avalanche/avsar_modem_training", "r");
        if(!f)
          exit(5);

        fgets(buf, sizeof(buf), f);
        if(strncmp(buf, "SHOWTIME", 8))
          exit(6);

        fclose(f);


	if(lastsock<0) {
		lastsock = socket(PF_ATMPVC, SOCK_DGRAM, ATM_AAL5);
	}
	if (lastsock<0) {
		perror("socket creation");
	} else {
	/* create the device with ioctl: */
	num=atoi(nstr);
	if( num>=0 && num<1234567890){
	   struct atm_newif_br2684 ni;
	   ni.backend_num = ATM_BACKEND_BR2684;
	   ni.media = BR2684_MEDIA_ETHERNET;
	   ni.mtu = 1500;
	   sprintf(ni.ifname, "nas%d", num);
           err=ioctl (lastsock, ATM_NEWBACKENDIF, &ni);
           printf("create:%d\n",err);
           if (err>=0) ;;;
           lastitf=num;	/* even if we didn't create, because existed, assign_vcc wil want to know it! */
          } else {
          	fprintf (stderr, "err: strange itf number %d", num );
          }
          }
  return 0;
}

int get_type(const char *type)
{
  int result;
  int item;
  
  /* Valid Rates UBR,CBR,VBRRT,VBRNRT */

  item = fetch(&type,"!none","ubr","cbr","vbr","abr",NULL);
  switch (item) {
    case 1: fprintf(stderr,"ATM_UBR\n");
      break;
    case 2: fprintf(stderr,"ATM_CBR\n");
      break;
    case 3: fprintf(stderr,"ATM_VBR\n");
      break;
    default: fprintf(stderr," QOS type not supported \n");
      return ATM_CBR;
  }
  
  return result=item;
}

int assign_vcc(char *astr, int encap, int qos_type, int pcr,int scr, int bufsize)
{
    struct ifreq ifr;

    int err, errno;
    struct atm_qos qos;
    struct sockaddr_atmpvc addr;
    int fd,skfd;
    struct atm_backend_br2684 be;

	memset(&addr, 0, sizeof(addr));
	err=text2atm(astr,(struct sockaddr *)(&addr), sizeof(addr), T2A_PVC);
           printf("text2atm:%d\n",err);
#if 0
	addr.sap_family = AF_ATMPVC;
    addr.sap_addr.itf = itf;
    addr.sap_addr.vpi = 0;
    addr.sap_addr.vci = vci;
#endif
    fprintf(stderr,"Communicating over ATM %d.%d.%d\n", addr.sap_addr.itf,
                                           addr.sap_addr.vpi,
                                           addr.sap_addr.vci);

    if ((fd = socket(PF_ATMPVC, SOCK_DGRAM, ATM_AAL5)) < 0)
       fprintf(stderr,"failed to create socket %d", errno);


    memset(&qos, 0, sizeof(qos));
    qos.aal                     = ATM_AAL5;
    //    qos.txtp.traffic_class      = ATM_UBR;
    qos.txtp.traffic_class      = qos_type;
    qos.txtp.max_sdu            = 1524;
    //    qos.txtp.pcr                = ATM_MAX_PCR;
    qos.txtp.pcr                = pcr;
    qos.txtp.scr                = scr;
    qos.rxtp = qos.txtp;

        err=setsockopt(fd,SOL_SOCKET,SO_SNDBUF, &bufsize ,sizeof(bufsize));
        fprintf(stderr,"setsockopt SO_SNDBUF: (%d) %s\n",err, strerror(err));

    if (setsockopt(fd, SOL_ATM, SO_ATMQOS, &qos, sizeof(qos)) < 0)
        fprintf(stderr,"setsockopt SO_ATMQOS %d", errno);

       err = connect(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_atmpvc));

       if (err < 0)
           fatal("failed to connect on socket: %d", errno);

           /* attach the vcc to device: */

    be.backend_num = ATM_BACKEND_BR2684;
    be.ifspec.method = BR2684_FIND_BYIFNAME;
    sprintf(be.ifspec.spec.ifname, "nas%d", lastitf);
    be.fcs_in = BR2684_FCSIN_NO;
    be.fcs_out = BR2684_FCSOUT_NO;
    be.fcs_auto = 0;
/* mwh   be.encaps = encap ? BR2684_ENCAPS_VC : BR2684_ENCAPS_LLC; */
    switch(encap) {
	case 0:
		be.encaps = BR2684_ENCAPS_LLC;
		break;
	case 1:
		be.encaps = BR2684_ENCAPS_VC;
		break;
	case 2:
		fprintf(stderr, "Autodetect unsupported - defaulting to  Bridged LLC!! \n");
		be.encaps = BR2684_ENCAPS_LLC;
		break;
	case 3:
		be.encaps = BR2684_ENCAPS_LLC_ROUTED;
		break;
	case 4:
		be.encaps = BR2684_ENCAPS_VC_ROUTED;
		break;
	default:
		fprintf(stderr, "Bad encapsulation - defaulting to  Bridged LLC!! \n");
		be.encaps = BR2684_ENCAPS_LLC;
    }
    be.has_vpiid = 0;
    be.send_padding = 0;
    be.min_size = 0;
    err=ioctl (fd, ATM_SETBACKEND, &be);
    fprintf(stderr, "assign:%d\n",err);

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    sprintf(ifr.ifr_name, "nas%d", lastitf);
    err=ioctl(skfd, SIOCGIFFLAGS, &ifr);
    fprintf(stderr, "SIOCGIFFLAGS:%d\n",err);
    sprintf(ifr.ifr_name, "nas%d", lastitf);
    ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
    err=ioctl(skfd, SIOCSIFFLAGS, &ifr);
    fprintf(stderr, "SIOCGIFFLAGS:%d\n",err);
    close(skfd);

    return fd ;
}

void usage(char *s)
{
	printf("usage: %s [-b] [-e number][-r number][-q number][[-c number] [-a [itf.]vpi.vci]*]*\n", s);
	printf("               valid options for '-e number':\n");
	printf("                -e 0 = Bridged LLC encapsulation\n");
	printf("                -e 1 = Bridged VC-Mux encapsulation\n");
	printf("                -e 2 = Auto-detect encapsulation -NOT SUPPORTED!\n");
	printf("                -e 3 = Routed LLC encapsulation\n");
	printf("                -e 4 = Routed VC-Mux encapsulation\n\n");
	printf("                -r  PCR Peak Cell Rate \n");
	printf("                -q  SCR Sustainable Cell Rate \n");
	exit(1);
}

int main (int argc, char **argv)
{
int c, background=0, encap=0, sndbuf=8192;
int traffic_type=2, pcr=262144, scr=262144;
 
 
	lastsock=-1;
	lastitf=0;

while ((c = getopt(argc, argv,"a:bc:r:q:t:e:s:")) !=EOF)
	switch (c) {
		case 'a':
			assign_vcc(optarg, encap, traffic_type, pcr, scr, sndbuf);
			break;
		case 'b':
			background=1;
			break;
		case 'c':
			create_br(optarg);
			break;
		case 'e':
			encap=(atoi(optarg));
			if((encap<0) || (encap>4)){
				fprintf(stderr, "invalid encap: %s:\n",optarg);
				encap=0;
			}
			break;
	        case 'r':
			pcr = (atoi(optarg));
			break;
	        case 'q':
			scr = (atoi(optarg));
			break;

	        case 't':
			traffic_type = get_type(optarg);
			break;
		case 's':
			sndbuf=(atoi(optarg));
			if(sndbuf<0){
				fprintf(stderr, "invalid sndbuf: %s:\n",optarg);
				sndbuf=8192;
			}
			break;
		default:
			usage(argv[0]);
	}
	if (argc != optind) usage(argv[0]);

	if(lastsock>=0) close(lastsock);

	if (background) {
		pid_t pid;
	/* this seems to be broken, do not use -b for now */
		pid=fork();
		if (pid < 0) {
			fprintf(stderr,"fork returned negative: %d\n", pid);
			exit(2);
		} else if (pid) {
			fprintf(stderr,"Background pid: %d\n",pid);
			exit(0);
		}
	}
	
	while (1) sleep(30);	/* to keep the sockets... */
	return 0;
}
