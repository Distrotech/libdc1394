/*
 * 1394-Based Digital Camera Control Library
 * Copyright (C) 2000 SMART Technologies Inc.
 *
 * Written by Gord Peters <GordPeters@smarttech.com>
 * Additions by Chris Urmson <curmson@ri.cmu.edu>
 * Additions by Damien Douxchamps <ddouxchamps@users.sf.net>
 * Additions by David Moore <dcm@acm.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __DC1394_MACOSX_H__
#define __DC1394_MACOSX_H__

#include "control.h"
#include "macosx/capture.h"
#include <IOKit/firewire/IOFireWireLib.h>
#include <CoreServices/CoreServices.h>

#include "platform.h"

typedef enum {
  BUFFER_EMPTY = 0,
  BUFFER_FILLED = 1,
} buffer_status;

typedef struct _buffer_info {
  platform_camera_t * craw;
  int              i;
  buffer_status    status;
  struct timeval   filltime;
  int              num_dcls;
  NuDCLRef *       dcl_list;
} buffer_info;

struct _platform_t {
  int dummy;
};

typedef struct __dc1394_capture
{
  unsigned int             num_frames;
  int                      frame_pages;
  int                      current;
  /* components needed for the DMA based video capture */
  IOFireWireLibIsochChannelRef    chan;
  IOFireWireLibRemoteIsochPortRef rem_port;
  IOFireWireLibLocalIsochPortRef  loc_port;
  IOFireWireLibNuDCLPoolRef       dcl_pool;
  IOVirtualRange           databuf;
  buffer_info *            buffers;
  CFRunLoopRef             run_loop;
  CFStringRef              run_loop_mode;
  dc1394capture_callback_t callback;
  void *                   callback_user_data;
  int                      notify_pipe[2];
  uint8_t                  frames_ready;
  MPCriticalRegionID       mutex;
  MPQueueID                termination_queue;
  MPSemaphoreID            thread_init_semaphore;
  MPTaskID                 task;
  CFSocketRef              socket;
  CFRunLoopSourceRef       socket_source;
  uint8_t                  iso_is_allocated;
  uint8_t                  iso_is_started;

  uint32_t                 flags;
  Boolean                  do_irm;

  dc1394video_frame_t     *frames;
} dc1394capture_t;

struct _platform_camera_t {
  IOFireWireLibDeviceRef  iface;
  UInt32                  generation;

  dc1394camera_t * camera;

  dc1394capture_t         capture;

  int                     capture_is_set;
  int                     iso_channel_is_set;
  int                     iso_channel;
  int                     iso_auto_started;
};


#endif
