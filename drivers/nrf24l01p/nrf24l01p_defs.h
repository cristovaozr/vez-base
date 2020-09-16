/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#ifndef DRIVERS_NRF24L01P_NRF24L01P_DEFS_H_
#define DRIVERS_NRF24L01P_NRF24L01P_DEFS_H_

#define BIT(n) (1 << (n))

#define NRF24L01P_REG_CONFIG        0x00
#define CONFIG_MASK_RX_DR           BIT(6)
#define CONFIG_MASK_TX_DS           BIT(5)
#define CONFIG_MASK_MAX_RT          BIT(4)
#define CONFIG_EN_RCR               BIT(3)
#define CONFIG_CRO                  BIT(2)
#define CONFIG_PWR_UP               BIT(1)
#define CONFIG_PRIM_RX              BIT(0)

#define NRF24L01P_REG_EN_AA         0x01
#define EN_AA_ENNA_P5               BIT(5)
#define EN_AA_ENNA_P4               BIT(4)
#define EN_AA_ENNA_P3               BIT(3)
#define EN_AA_ENNA_P2               BIT(2)
#define EN_AA_ENNA_P1               BIT(1)
#define EN_AA_ENNA_P0               BIT(0)

#define NRF24L01P_REG_EN_RXADDR     0x02
#define EN_RXADDR_ERX_P5            BIT(5)
#define EN_RXADDR_ERX_P4            BIT(4)
#define EN_RXADDR_ERX_P3            BIT(3)
#define EN_RXADDR_ERX_P2            BIT(2)
#define EN_RXADDR_ERX_P1            BIT(1)
#define EN_RXADDR_ERX_P0            BIT(0)

#define NRF24L01P_REG_SETUP_AW      0x03
#define SETUP_AW_AW_MASK            0b00000011
#define SETUP_AW_AW_3_BYTES         0b00000001
#define SETUP_AW_AW_4_BYTES         0b00000010
#define SETUP_AW_AW_5_BYTES         0b00000011

#define NRF24L01P_REG_SETUP_RETR    0x04
#define SETUP_RETR_ARD_MSK          0b11110000
#define SETUP_RETR_ARC_MSK          0b00001111

#define NRF24L01P_REG_RF_CH         0x05
#define RF_CH_RF_CH_MSK             0b01111111

#define NRF24L01P_REG_RF_SETUP      0x06
#define RF_SETUP_CONT_WAVE          BIT(7)
#define RF_SETUP_RF_DR_LOW          BIT(5)
#define RF_SETUP_PLL_LOCK           BIT(4)
#define RF_SETUP_RF_DR_HIGH         BIT(3)
#define RF_SETUP_RF_PWR_MSK         0b00000110
#define RF_SETUP_RF_PWR_0           0b00000110
#define RF_SETUP_RF_PWR_6           0b00000100
#define RF_SETUP_RF_PWR_12          0b00000010
#define RF_SETUP_RF_PWR_18          0b00000000

#define NRF24L01P_REG_STATUS        0x07
#define STATUS_RX_DR                BIT(6)
#define STATUS_TX_DS                BIT(5)
#define STATUS_MAX_RT               BIT(4)
#define STATUS_RX_P_NO_MSK          0b00001110
#define STATUS_TX_FULL              BIT(0)

#define NRF24L01P_REG_OBSERVE_TX    0x08
#define OBSERVE_TX_PLOS_CNT_MASK    0b11110000
#define OBSERVE_TX_ARC_CNT          0b00001111

#define NRF24L01P_REG_RPD           0x09
#define RPD_RPD                     BIT(0)

#define NRF24L01P_REG_RX_ADDR_P0    0x0a
#define NRF24L01P_REG_RX_ADDR_P1    0x0b
#define NRF24L01P_REG_RX_ADDR_P2    0x0c
#define NRF24L01P_REG_RX_ADDR_P3    0x0d
#define NRF24L01P_REG_RX_ADDR_P4    0x0e
#define NRF24L01P_REG_RX_ADDR_P5    0x0f

#define NRF24L01P_REG_TX_ADDR       0x10
#define NRF24L01P_REG_RX_PW_P0      0x11
#define NRF24L01P_REG_RX_PW_P1      0x12
#define NRF24L01P_REG_RX_PW_P2      0x13
#define NRF24L01P_REG_RX_PW_P3      0x14
#define NRF24L01P_REG_RX_PW_P4      0x15
#define NRF24L01P_REG_RX_PW_P5      0x16

#define NRF24L01P_REG_FIFO_STATUS   0x17
#define FIFO_STATUS_TX_REUSE        BIT(6)
#define FIFO_STATUS_TX_FULL         BIT(5)
#define FIFO_STATUS_TX_EMPTY        BIT(4)
#define FIFO_STATUS_RX_FULL         BIT(1)
#define FIFO_STATUS_RX_EMPTY        BIT(0)

#define NRF24L01P_REG_DYNPD         0x1c
#define DYNPD_DPL_P5                BIT(5)
#define DYNPD_DPL_P4                BIT(4)
#define DYNPD_DPL_P3                BIT(3)
#define DYNPD_DPL_P2                BIT(2)
#define DYNPD_DPL_P1                BIT(1)
#define DYNPD_DPL_P0                BIT(0)

#define NRF24L01P_REG_FEATURE       0x1d
#define FEATURE_EN_DPL              BIT(2)
#define FEATURE_EN_ACK_PAY          BIT(1)
#define FEATURE_EN_DYN_ACK          BIT(0)

#endif // DRIVERS_NRF24L01P_NRF24L01P_DEFS_H_
