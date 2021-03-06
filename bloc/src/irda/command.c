/*
 * command.c
 *
 * Leader showing application
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


#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "algo.h"
#include "packet.h"
#include "turn.h"
#include "send.h"
#include "election.h"
#include "debug.h"
#include "ping.h"
#include "command.h"


xQueueHandle commandQueue; // queue used to notice the current game/animation/task

void command_init(){
  commandQueue = xQueueCreate(1,sizeof(Packet));
}



void command_receive(Packet * packet){
  xQueueSend(commandQueue, ( void * )packet, 0); // put the packet into the queue
}
