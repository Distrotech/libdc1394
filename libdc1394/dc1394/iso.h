#ifndef __ISO_H__
#define __ISO_H__

#include <dc1394/control.h>

/**
 * dc1394_iso_set_persist:
 * @camera: A camera handle.
 *
 * Calling this function will cause isochronous channel and bandwidth
 * allocations to persist beyond the lifetime of this dc1394camera_t
 * instance.  Normally (when this function is not called), any allocations
 * would be automatically released upon freeing this camera or a
 * premature shutdown of the application (if possible).  For this function
 * to be used, it must be called prior to any allocations or an error will
 * be returned.
 *
 * Returns: DC1394_SUCCESS if the operation succeeded.
 * DC1394_FUNCTION_NOT_SUPPORTED if the current platform/driver does not
 * allow persistent allocations.
 */
dc1394error_t dc1394_iso_set_persist (dc1394camera_t * camera);

/**
 * dc1394_iso_allocate_channel:
 * @camera: A camera handle.
 * @channels_allowed: A bitmask of acceptable channels for the allocation.
 *   The LSB corresponds to channel 0 and the MSB corresponds to channel
 *   63.  Only channels whose bit is set will be considered for the allocation.
 *   If @channels_allowed = 0, the complete set of channels supported by
 *   this camera will be considered for the allocation.
 * @channel: The allocated channel number is returned here.
 *
 * Allocates an isochronous channel.  This
 * function may be called multiple times, each time allocating an additional
 * channel.  The channel is automatically re-allocated if there is a bus
 * reset.  The channel is automatically released when this dc1394camera_t
 * is freed or if the application shuts down prematurely.  If the channel
 * needs to persist beyond the lifetime of this application, call
 * dc1394_iso_set_persist() first.  Note that this function does _not_
 * automatically program @camera to use the allocated channel for isochronous
 * streaming.  You must do that manually using dc1394_video_set_iso_channel().
 *
 * Returns: DC1394_SUCCESS if the operation succeeded.  The allocated
 * channel is stored in @channel.  DC1394_FUNCTION_NOT_SUPPORTED if the
 * current driver/platform does not allow channel allocation.
 * DC1394_NO_ISO_CHANNEL if none of the requested channels are available.
 */
dc1394error_t dc1394_iso_allocate_channel (dc1394camera_t * camera,
    uint64_t channels_allowed, int * channel);

/**
 * dc1394_iso_release_channel:
 * @camera: A camera handle.
 * @channel: The channel number to release.
 *
 * Releases a previously allocated channel.  It is acceptable to release
 * channels that were allocated by a different process or host.  If
 * attempting to release a channel that is already released, the function
 * will succeed.
 *
 * Returns: DC1394_SUCCESS if the operation succeeded.
 * DC1394_FUNCTION_NOT_SUPPORTED if the current driver/platform does not
 * allow channel release.
 */
dc1394error_t dc1394_iso_release_channel (dc1394camera_t * camera,
    int channel);

/**
 * dc1394_iso_allocate_bandwidth:
 * @camera: A camera handle.
 * @bandwidth_bytes: The number of isochronous bytes to allocate.
 *
 * Allocates isochronous bandwidth.  This functions allocates bandwidth
 * _in addition_ to any previous allocations.  It may be called multiple
 * times.  The bandwidth is automatically re-allocated if there is a bus
 * reset.  The bandwidth is automatically released if this camera is freed
 * or the application shuts down prematurely.  If the bandwidth needs to
 * persist beyond the lifetime of this application, call
 * dc1394_iso_set_persist() first.
 *
 * Returns: DC1394_SUCCESS if the operation succeeded.
 * DC1394_FUNCTION_NOT_SUPPORTED if the current driver/platform does not
 * allow bandwidth allocation.  DC1394_NO_BANDWIDTH if there is not enough
 * available bandwidth to support the allocation.  In this case,
 * no bandwidth is allocated.
 */
dc1394error_t dc1394_iso_allocate_bandwidth (dc1394camera_t * camera,
    int bandwidth_bytes);

/**
 * dc1394_iso_release_bandwidth:
 * @camera: A camera handle.
 * @bandwidth_bytes: The number of isochronous bytes to free.
 *
 * Releases previously allocated isochronous bandwidth.  Each dc1394camera_t
 * keeps track of a running total of bandwidth that has been allocated.
 * Released bandwidth is subtracted from this total for the sake of
 * automatic re-allocation and automatic release on shutdown.  It is also
 * acceptable for a camera to release more bandwidth than it has allocated
 * (to clean up for another process for example).  In this case, the
 * running total of bandwidth is not affected.  It is acceptable to
 * release more bandwidth than is allocated in total for the bus.  In this
 * case, all bandwidth is released and the function succeeds.
 *
 * Returns: DC1394_SUCCESS if the operation succeeded.
 * DC1394_FUNCTION_NOT_SUPPORTED if the current driver/platform does not
 * allow bandwidth release.
 */
dc1394error_t dc1394_iso_release_bandwidth (dc1394camera_t * camera,
    int bandwidth_bytes);

/**
 * dc1394_iso_release_all:
 * @camera: A camera handle.
 *
 * Releases all channels and bandwidth that have been previously allocated
 * for this dc1394camera_t.  Note that this information can only be tracked
 * per process, and there is no knowledge of allocations for this camera
 * by previous processes.  To release resources in such a case, the manual
 * release functions dc1394_iso_release_channel() and
 * dc1394_iso_release_bandwidth() must be used.
 * 
 * Returns: DC1394_SUCCESS if the operation succeeded.  DC1394_FAILURE
 * if some resources were not able to be released.
 */
dc1394error_t dc1394_iso_release_all (dc1394camera_t * camera);

#endif
