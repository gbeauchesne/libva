/*
 * va_drm.h - Raw DRM API
 *
 * Copyright (c) 2012 Intel Corporation. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL INTEL AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef VA_DRM_H
#define VA_DRM_H

#include <va/va.h>

/**
 * \file va_drm.h
 * \brief The raw DRM API
 *
 * This file contains the \ref api_drm "Raw DRM API".
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <drm_fourcc.h>

/** \brief VA Buffer info specific to the DRM backend. */
typedef struct _VABufferInfoDRM {
    /** \brief Native DRM buffer handle (name). */
    uintptr_t           handle;
    /** \brief DRM buffer format, see DRM_FORMAT_xxx. */
    uint32_t            format;
    /** \brief Width in pixels for the DRM image. */
    uint16_t            width;
    /** \brief Height in pixels for the DRM image. */
    uint16_t            height;
    /** \brief Number of planes associated to this DRM image. */
    uint32_t            num_planes;
    /** \brief Number of bytes for a line stride. */
    uint32_t            pitches[3];
    /** \brief Number of bytes to add from base to reach the specified plane. */
    intptr_t            offsets[3];
} VABufferInfoDRM;

/**
 * \brief Returns a VA display derived from the specified DRM connection.
 *
 * This function returns a (possibly cached) VA display from the
 * specified DRM connection @fd.
 *
 * @param[in]   fd      the DRM connection descriptor
 * @return the VA display
 */
VADisplay
vaGetDisplayDRM(int fd);

/**
 * \brief Returns the DRM buffer name associated with a VA surface.
 *
 * This function returns the underlying DRM buffer handle for the supplied
 * VA @surface. Additional information is also filled into @out_buffer_info
 * so that to describe the layout of the associated DRM buffer.
 *
 * @param[in]   dpy     the VA display
 * @param[in]   surface the VA surface
 * @param[out]  out_buffer_info the returned VA/DRM buffer information
 * @return VA_STATUS_SUCCESS if operation is successful, another #VAStatus
 *     value otherwise.
 */
VAStatus
vaGetSurfaceBufferDRM(
    VADisplay           dpy,
    VASurfaceID         surface,
    VABufferInfoDRM    *out_buffer_info
);

/**
 * \brief Returns the DRM buffer name associated with a VA image.
 *
 * This function returns the underlying DRM buffer handle for the
 * supplied VA @image. Additional information is also filled into
 * @out_buffer_info so that to describe the layout of the associated
 * DRM buffer.
 *
 * Note: paletted formats are not supported. In this case,
 * VA_STATUS_ERROR_INVALID_IMAGE_FORMAT is returned.
 *
 * @param[in]   dpy     the VA display
 * @param[in]   image   the VA image
 * @param[out]  out_buffer_info the returned VA/DRM buffer information
 * @return VA_STATUS_SUCCESS if operation is successful, another #VAStatus
 *     value otherwise.
 */
VAStatus
vaGetImageBufferDRM(
    VADisplay           dpy,
    VAImageID           image,
    VABufferInfoDRM    *out_buffer_info
);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* VA_DRM_H */
