/*
 * hardware_init.c
 *
 * STM32 initialization.
 *
 * Copyright 2011 - Benjamin Bonny    <benjamin.bonny@gmail.com>,
 *                  Cédric Le Ninivin <cedriclen@gmail.com>,
 *                  Guillaume Normand <guillaume.normand.gn@gmail.com>          
 *
 * All rights reserved.
 * MB Led
 * Telecom ParisTech - ELECINF344/ELECINF381
 *
 * This file is part of MB Led Project.
 *
 * MB Led Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MB Led Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MB Led Project.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */


// FreeRTOS and STM32 features
#include <stm32f10x.h>
#include <FreeRTOS.h>

// GLiP features
#include "hardware_init.h"
#include "flash_constant.h"


void hardware_init () {

  // Activating HSE
  RCC->CR |= 1<<16;

  // Wait for HSE to be ready
  while (!(RCC->CR & (1<<17)));

  // Configure the PLL
  RCC->CFGR = (RCC->CFGR & 0xFFC0FFFF) | 0x1D0000;

  // Configure the AHB : no division
  RCC->CFGR = RCC->CFGR & 0xFFFFFF7F;

  // Configure APB2
  RCC->CFGR = RCC->CFGR & 0xFFFFDFFF;

  // Configure APB1
  RCC->CFGR = (RCC->CFGR & 0xFFFFF8FF) | (1<<10);

  // Put the Wait state to 2
  FLASH->ACR = 0x12;

  // Activating PLL
  RCC->CR |= (1<<24);

  // Wait for PLL to be ready
  while (!(RCC->CR & (1<<25)));

  // Put the PLL as system clock
  RCC->CFGR |= 0x2;

  // Wait for changement to be efficient
  while ((RCC->CFGR & (3<<2)) != (2<<2));

  // Stop HSI
  RCC->CR &= ~1;

  // AHB clock = HCLK = 72MHz                                                
  RCC->CFGR &=  ~RCC_CFGR_HPRE_0 & ~RCC_CFGR_HPRE_1 & ~RCC_CFGR_HPRE_2;                                    
 
  // APB1 clock = HCLK/2 = 36MHz                                             
  RCC->CFGR |= RCC_CFGR_PPRE1_2;
  RCC->CFGR &=  ~RCC_CFGR_PPRE1_0 & ~RCC_CFGR_PPRE1_1;

  // horloge pour AFIO                                                       
  RCC-> APB2ENR |= RCC_APB2ENR_AFIOEN;


  // Put the address of the interruption vector
  // depending of the mode : RAM or FLASH
#ifdef PROGRAM_MODE_RAM
  SCB->VTOR = 0x20000000;
#endif

    
#ifdef PROGRAM_MODE_FLASH
  SCB->VTOR = 0x8000000;
#endif
  
  // No subpriority
  SCB->AIRCR = 0x5FA0000 | (3<<8);

  // Configure SysTick
  SysTick->CTRL |= 1<<2;
}
