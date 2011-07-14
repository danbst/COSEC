
/* 
 *  NIC registers 
*/
/* ID registers */
#define RTL8139_IDR0    0x0000
#define RTL8139_IDR1    0x0001
#define RTL8139_IDR2    0x0002
#define RTL8139_IDR3    0x0003
#define RTL8139_IDR4    0x0004
#define RTL8139_IDR5    0x0005

/* Multicast registers */
#define RTL8139_MAR0    0x0008
#define RTL8139_MAR1    0x0009
#define RTL8139_MAR2    0x000A
#define RTL8139_MAR3    0x000B
#define RTL8139_MAR4    0x000C
#define RTL8139_MAR5    0x000D
#define RTL8139_MAR6    0x000E
#define RTL8139_MAR7    0x000F

/* Transmit states of descriptors */
#define RTL8139_TSD0    0x0010
#define RTL8139_TSD1    0x0014
#define RTL8139_TSD2    0x0018
#define RTL8139_TSD3    0x001C

/* Transmit start addresses of descripors */
#define RTL8139_TSAD0   0x0020
#define RTL8139_TSAD1   0x0024
#define RTL8139_TSAD2   0x0028
#define RTL8139_TSAD3   0x002C

/* receive buffer start address */
#define RTL8139_RBSTART 0x0030

/* early receive byte count register */
#define RTL8139_ERBCR   0x0034

/* early Rx status register */
#define RTL8139_ERSR    0x0036

/* command register */
#define RTL8139_CR      0x0037

/* current address of packet read */
#define RTL8139_CAPR    0x0038

/* current buffer address, total Tx byte count */
#define RTL8139_CBR     0x003A

/* interrupt mask register */
#define RTL8139_IMR     0x003C

/* interrupt status register */
#define RTL8139_ISR     0x003E

/* Tx config register */
#define RTL8139_TCR     0x0040
/* Rx config register */
#define RTL8139_RCR     0x0044

/* Timer count register */
#define RTL8139_TCTR    0x0048
/* Missed packet counter: 24bit */
#define RTL8139_MPC     0x004C

/* 93C46 command register */
#define RTL8139_9346CR  0x0050

#define RTL8139_CONFIG1 0x0052

