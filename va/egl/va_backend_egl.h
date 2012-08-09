/*
 * Copyright (c) 2011 Intel Corporation. All Rights Reserved.
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

#ifndef VA_BACKEND_EGL_H
#define VA_BACKEND_EGL_H

#include <stdint.h>
#include <va/va.h>
#include <va/va_backend.h>
#include <EGL/egl.h>

/** \brief VTable version (major) id for VA/EGL hooks. */
#define VA_EGL_ID                       (0x45474c00) /* EGL0 */
/** \brief VTable version for VA/EGL hooks. */
#define VA_EGL_VTABLE_VERSION           (VA_EGL_ID + 1)
/** \brief VA/EGL client buffer struct version. */
#define VA_EGL_CLIENT_BUFFER_VERSION    (VA_EGL_ID + 1)

/* Forward declarations */
struct va_egl_client_buffer;

/**
 * \brief Pixel formats.
 *
 * Describe how the pixel buffer is represented in memory.
 *
 * @note
 * Native byte-order means the least significant byte is stored first
 * in memory on little-endian platforms, and that the most significant
 * byte is stored first in memory on big-endian platforms.
 *
 * e.g. VA_EGL_PIXEL_FORMAT_ARGB8888 represents 32-bit pixel values
 * that are stored as BGRA on little-endian platforms and ARGB on
 * big-endian platforms.
 */
enum {
    /** \brief Packed RGB 8:8:8, 32bpp, A R G B, native endian. */
    VA_EGL_PIXEL_FORMAT_ARGB8888 = 1,
    /** \brief Packed RGB 8:8:8, 32bpp, 0 R G B, native endian. */
    VA_EGL_PIXEL_FORMAT_XRGB8888,
    /** \brief Packed RGB 8:8:8, 32bpp, B G R A, native endian. */
    VA_EGL_PIXEL_FORMAT_BGRA8888,
    /** \brief Packed RGB 8:8:8, 32bpp, A B G R, native endian. */
    VA_EGL_PIXEL_FORMAT_ABGR8888,
    /** \brief Packed RGB 8:8:8, 32bpp, R G B A, native endian. */
    VA_EGL_PIXEL_FORMAT_RGBA8888,
    /** \brief Packed YUV 8:8:8, 32bpp, A Y U V, native endian. */
    VA_EGL_PIXEL_FORMAT_AYUV8888,
    /** \brief Planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for U V. */
    VA_EGL_PIXEL_FORMAT_NV12,
    /** \brief Grayscale, 8bpp, 1 plane for Y. */
    VA_EGL_PIXEL_FORMAT_GRAY8,
    /**
     * \brief Planar YUV 4:1:0, 10bpp, 3 planes, Y U V.
     *
     * @note
     * Quarter horizontal chroma resolution, half vertical chroma
     * resolution.
     */
    VA_EGL_PIXEL_FORMAT_YUV410P,
    /**
     * \brief Planar YUV 4:1:1, 12bpp, 3 planes, Y U V.
     *
     * @note
     * Quarter horizontal chroma resolution, full vertical chroma
     * resolution.
     */
    VA_EGL_PIXEL_FORMAT_YUV411P,
    /**
     * \brief Planar YUV 4:2:0, 12bpp, 3 planes, Y U V.
     *
     * @note
     * Half horizontal chroma resolution, half vertical chroma
     * resolution.
     */
    VA_EGL_PIXEL_FORMAT_YUV420P,
    /**
     * \brief Planar YUV 4:2:2, 16bpp, 3 planes, Y U V.
     *
     * @note
     * Half horizontal chroma resolution, full vertical chroma
     * resolution.
     */
    VA_EGL_PIXEL_FORMAT_YUV422P,
    /**
     * \brief Planar YUV 4:4:4, 24bpp, 3 planes, Y U V.
     *
     * @note
     * Full horizontal chroma resolution, full vertical chroma
     * resolution.
     */
    VA_EGL_PIXEL_FORMAT_YUV444P,
};

/**
 * \brief VA/EGL client buffer.
 *
 * This structure is maintained by the VA driver. This is not a
 * mandatory representation of the EGLClientBuffer. VA drivers are
 * free to implement their own data structure as long as the
 * corresponding EGL implementation is aware of it, and it fulfills
 * the VA/EGL API requirements.
 *
 * EGL implementations may hold extra data in @private_data. Should
 * the EGL implementation destroy its @private_data, then it shall
 * also reset this field value to NULL. Otherwise, it can provide a
 * destructor to the VA driver implementation. The VA driver will call
 * that @destroy_private_data function, if necessary.
 */
struct va_egl_client_buffer {
    /**
     * \brief Struct version.
     *
     * Implementations shall set this field to \ref
     * VA_EGL_CLIENT_BUFFER_VERSION.
     */
    uint32_t            version;
    /** \brief Native buffer handle (e.g. DRM name). */
    uintptr_t           handle;
    /**
     * \brief Buffer structure, see VA_EGL_BUFFER_STRUCTURE_xxx.
     *
     * This field represents the buffer structure exposed to the
     * client application. It is just a convenience value to avoid
     * further conversions from pixel format to buffer structure in
     * EGL implementations.
     */
    uint32_t            structure;
    /** \brief Pixel format, see VA_EGL_PIXEL_FORMAT_xxx. */
    uint32_t            format;
    /** \brief Width in pixels. */
    uint16_t            width;
    /** \brief Height in pixels. */
    uint16_t            height;
    /** \brief Number of planes associated to this pixel buffer. */
    uint32_t            num_planes;
    /** \brief Number of bytes for a line stride. */
    uint32_t            pitches[3];
    /** \brief Number of bytes to add from base to reach the specified plane. */
    intptr_t            offsets[3];
    /** \brief Data private to EGL implementations. */
    void               *private_data;
    /** \brief Destructor for @private_data. */
    void              (*destroy_private_data)(void *private_data);
};

/** \brief VA/EGL implementation hooks. */
struct VADriverVTableEGL {
    /**
     * \brief Interface version.
     *
     * Implementations shall set this field to \ref VA_EGL_VTABLE_VERSION.
     */
    unsigned int         version;

    /** \brief Hook to return EGL buffer associated with the VA surface. */
    VAStatus
    (*vaGetSurfaceBufferEGL)(
        VADriverContextP ctx,
        VASurfaceID      surface,
        EGLClientBuffer *out_buffer
    );

    /** \brief Hook to return EGL buffer associated with the VA image. */
    VAStatus
    (*vaGetImageBufferEGL)(
        VADriverContextP ctx,
        VAImageID        image,
        EGLClientBuffer *out_buffer
    );

    /** \brief Hook to query VA/EGL buffer attributes. */
    VAStatus
    (*vaGetBufferAttributeEGL)(
        VADriverContextP ctx,
        EGLClientBuffer  buffer,
        EGLenum          attribute,
        EGLint          *value
    );
};

/* DEPRECATED */
struct VADriverVTableEGL_Deprecated {
    /* Get EGL ClientBufer buffer index and device id from surface id */
    VAStatus (*vaGetEGLClientBufferFromSurface) (
        VADriverContextP ctx,
        VASurfaceID surface,
        void **buffer
    );
    /* TBD: more APIs for EGL */
};

#endif /* VA_BACKEND_EGL_H */
