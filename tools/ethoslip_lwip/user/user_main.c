/*
 ============================================================================
 Name        : user_main.c
 Author      : hxdyxd
 Version     :
 Copyright   : Your copyright notice
 Description : user_main
 ============================================================================
 */

#include <stdlib.h>
#include <string.h>
#include "app_debug.h"
#include "soft_timer.h"
#include "lwip/pbuf.h"
#include "lwip/inet.h"
#include "lwip/dhcp.h"
#include "lwip/apps/httpd.h"

#include "wlan_api.h"

#define WIFI_NOT_HWADDR   0
#define WIFI_GOT_HWADDR   1
#define WIFI_CONNECTED    2
#define WIFI_DHCP_NOT_IP  3
#define WIFI_DHCP_GOT_IP  4
#define WIFI_RUNING       5

#define WIFI_FREE         255


#define TEST_SERVER  "120.79.154.122"
#define SERVER_PORT   8080

//global netif
extern struct netif netif;


static uint8_t wifi_status = WIFI_NOT_HWADDR;
static unsigned char udp_out_buffer[4000];
struct udp_pcb *udp_sock = NULL;


void udp_debug_callback(void)
{
	char *str = "hello world!!!";

	struct pbuf *p = NULL;
	 p = pbuf_alloc(PBUF_TRANSPORT, strlen(str), PBUF_RAM);
	if (p == NULL) {
		printf("Cannot allocate pbuf to receive packet\n");
		return;
	}

	err_t err = pbuf_take(p, str, strlen(str) );
	if(err != ERR_OK) {
	  	printf("Cannot take pbuf\n");
		pbuf_free(p);
		return;
	}

	udp_send(udp_sock, p);
	pbuf_free(p);
	APP_DEBUG("udp: %s\n", str);
}

void udp_data_input_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p,
				  const ip_addr_t *addr,  u16_t port)
{

	int total_len = p->tot_len;
	pbuf_copy_partial(p, udp_out_buffer, total_len, 0);

	for(int i=0;i<total_len;i++)
		printf("%02x ", udp_out_buffer[i]);
	printf("\n");
	printf("total_len = %d\n", total_len);
	pbuf_free(p);
}

void udp_test(void)
{
	udp_sock = udp_new();
	if(udp_sock == NULL) {
		APP_ERROR("Failed to new udp pcb\n");
	}

	if(ERR_OK != udp_bind(udp_sock, IP_ADDR_ANY, SERVER_PORT) ) {
		APP_ERROR("Failed to bind udp port\n");
	}

	struct ip_addr s_ip;
	s_ip.u_addr.ip4.addr = inet_addr(TEST_SERVER);
	s_ip.type = IPADDR_TYPE_V4;
	if(ERR_OK != udp_connect(udp_sock, &s_ip, SERVER_PORT) ) {
		APP_ERROR("Failed to connect udp server\n");
	}

	udp_recv(udp_sock, udp_data_input_callback, NULL);

	//udp sender timer
	soft_timer_create(0, 1, 1, udp_debug_callback, 2000);
}


/*
 * �@ȡӲ����ַ�ɹ����{����
 */
void get_hwaddr_callback(uint8_t *hwaddr, int len)
{
	//get hwaddr success
	APP_DEBUG("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
			hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);
	memcpy(netif.hwaddr, hwaddr, len);
	soft_timer_delete(1);
	wifi_status = WIFI_GOT_HWADDR;
}

/*
 * �@ȡӲ����ַ���r̎����{����
 */
void get_hwaddr_timeout_callback(void)
{
	//get hwaddr timeout
	APP_WARN("timeout, retry ...\n");
	if(wlan_api_get_hwaddr(get_hwaddr_callback) >= 0) {
		soft_timer_create(1, 1, 0, get_hwaddr_timeout_callback, 2000);
	}
}

void ipv6_addr_callback(void)
{
	printf("ipv6:\n");
	for(int j=0;j<3;j++) {
		printf("ipv6 addr%d: ", j);
		uint8_t *v6_addr = (uint8_t *)&(netif_ip6_addr(&netif, j)->addr[0]);
		for(int i=0;i<16;i+=2) {
			if(i!=0) {
				printf(":");
			}
			printf("%02x%02x", v6_addr[i], v6_addr[i+1]);
		}
		printf("\n");
	}
}


/*
 * mainѭ�h
 */
void user_loop(void)
{
	//todo ...

	switch(wifi_status) {
	case WIFI_NOT_HWADDR:
		{
			APP_DEBUG("get hwaddr ...\n");
			if(wlan_api_get_hwaddr(get_hwaddr_callback) >= 0) {
				soft_timer_create(1, 1, 0, get_hwaddr_timeout_callback, 2000);
			}
			wifi_status = WIFI_FREE;
		}
		break;
	case WIFI_GOT_HWADDR:
		{
			//create ipv6 linklocal addr
			netif_create_ip6_linklocal_address(&netif, 1);
			//connect to wifi
			char *ssid_2 = "MTK";
			char *passwd_2 = "12345678";
			wlan_api_connect(ssid_2, passwd_2, strlen(ssid_2), strlen(passwd_2));
			APP_DEBUG("wifi connected\n");
			wifi_status = WIFI_CONNECTED;
		}
		break;
	case WIFI_CONNECTED:
		dhcp_start(&netif );
		APP_DEBUG("[dhcp] discover ...\n");
		wifi_status = WIFI_DHCP_NOT_IP;
		break;
	case WIFI_DHCP_NOT_IP:
		if(dhcp_supplied_address(&netif) == 1) {
			APP_DEBUG("[dhcp] success!\n");
			printf("ip: %s\n", ip4addr_ntoa(netif_ip4_addr(&netif)) );
			printf("netmask: %s\n", ip4addr_ntoa(netif_ip4_netmask(&netif)) );
			printf("gw: %s\n", ip4addr_ntoa(netif_ip4_gw(&netif)) );
			wifi_status = WIFI_DHCP_GOT_IP;
		}
		break;
	case WIFI_DHCP_GOT_IP:
		//todo ...
		{
			ipv6_addr_callback();
			udp_test();
			httpd_init();
			wifi_status = WIFI_RUNING;
		}
		break;
	case WIFI_RUNING:
		//todo ...
		break;
	default:
		break;
	}

}

/*
 * ��ʼ��
 */
void user_init(void)
{
	//todo ...
}


