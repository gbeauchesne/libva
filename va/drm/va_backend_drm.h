/*
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

#ifndef VA_BACKEND_DRM_H
#define VA_BACKEND_DRM_H

#include <va/va_backend.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \brief VA/DRM API identifier. */
#define VA_DRM_API_ID           (0x44524d00) /* DRM0 */
/** \brief VA/DRM API version. */
#define VA_DRM_API_VERSION      (VA_DRM_API_ID + 1)

/* Forward declarations */
struct _VABufferInfoDRM;

/** \brief VA/DRM implementation hooks. */
struct VADriverVTableDRM {
    /**
     * \brief Interface version.
     *
     * This field is set to \ref VA_DRM_API_VERSION from libva. It is
     * useful to VA drivers for determining the layout of this VTable.
     */
    unsigned int version;

    /** \brief Hook to return DRM buffer info associated with a VA surface. */
    VAStatus (*vaGetSurfaceBufferDRM)(
        VADriverContextP         ctx,
        VASurfaceID              surface,
        struct _VABufferInfoDRM *out_buffer_info
    );

    /** \brief Hook to return DRM buffer info associated with a VA image. */
    VAStatus (*vaGetImageBufferDRM)(
        VADriverContextP         ctx,
        VAImageID                image,
        struct _VABufferInfoDRM *out_buffer_info
    );
};

#ifdef __cplusplus
}
#endif

#endif /* VA_BACKEND_DRM_H */
