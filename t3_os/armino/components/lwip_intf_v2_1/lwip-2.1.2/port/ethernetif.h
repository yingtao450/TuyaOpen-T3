/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : ethernetif.h
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "common/bk_include.h"
// Modified by TUYA Start
#if (CONFIG_ETH || CONFIG_SPI_ETH)
#include "lwip/err.h"
#include "lwip/netif.h"
#endif
// Modified by TUYA End
#ifdef CONFIG_ETH
/* Exported functions ------------------------------------------------------- */
err_t ethernetif_init(struct netif *netif);
void ethernet_link_thread(void *argument);

#endif /* CONFIG_ETH */
// Modified by TUYA Start
#if CONFIG_SPI_ETH
/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
  uint16_t rx_len;
  uint8_t rx_status;
};
err_t spi_ethernetif_init(struct netif *netif);
int net_spi_eth_init(void);
void *net_get_spi_eth_handle(void);
void *net_get_sta_handle(void);
#endif /* CONFIG_SPI_ETH */
// Modified by TUYA End
#endif /* __ETHERNETIF_H__ */

