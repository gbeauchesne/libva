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

#ifndef VA_EGL_H
#define VA_EGL_H

#include <va/va.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup api_winsys_egl VA/EGL API
 *
 * @{
 */

/*This function is used to get EGLClientBuffer
 * (lower 16bits is buffer index, upper 16bits 
 * is BC device id.) from surface id. Application
 * should maintain EGLClientBuffer itself.*/

/* DEPRECATED */
VAStatus vaGetEGLClientBufferFromSurface (
    VADisplay dpy,
    VASurfaceID surface,	
    EGLClientBuffer *buffer     /* out*/
);

/**
 * \brief Buffer structures.
 *
 * The buffer structure defines how many EGLImages need to be created
 * and used for rendering, which components will be sampled from each
 * EGLImage and how they map to rgba components in the shade code.
 *
 * @note
 * The naming convention separates planes by '_' and within each
 * plane, the order of R, G, B, A, Y, U and V indicates how those
 * components map to the rgba value returned by the sampler. X
 * indicates that the corresponding component in the rgba value is not
 * used.
 */
enum {
    /** \brief Packed RGB, with significant alpha channel. */
    VA_EGL_BUFFER_STRUCTURE_RGBA = 0x305e,
    /** \brief Packed RGB, alpha channel is not relevant. */
    VA_EGL_BUFFER_STRUCTURE_RGB = 0x305d,
    /** \brief Planar YUV, 1 plane for Y and 1 plane for UV. */
    VA_EGL_BUFFER_STRUCTURE_Y_UV = 0x31d8,
    /** \brief Planar YUV, 3 planes for Y U V. */
    VA_EGL_BUFFER_STRUCTURE_Y_U_V = 0x31d7,
};

/** \brief Picture structures. */
enum {
    /** \brief Whole frame picture. */
    VA_EGL_PICTURE_STRUCTURE_FRAME = VA_FRAME_PICTURE,
    /** \brief Top-field picture. */
    VA_EGL_PICTURE_STRUCTURE_TOP_FIELD = VA_TOP_FIELD,
    /** \brief Bottom-field picture. */
    VA_EGL_PICTURE_STRUCTURE_BOTTOM_FIELD = VA_BOTTOM_FIELD
};

/** \brief VA buffer info specific to the EGL backend. */
typedef struct _VABufferInfoEGL {
    /** \brief Underlying surface buffer suitable for eglCreateImageKHR(). */
    EGLClientBuffer     buffer;
    /** \brief Buffer structure, see VA_EGL_BUFFER_STRUCTURE_xxx. */
    unsigned int        structure;
    /** \brief Surface width in pixels. */
    unsigned int        width;
    /** \brief Surface height in pixels. */
    unsigned int        height;
} VABufferInfoEGL;

/**
 * \brief Returns the EGL client buffer info associated with a VA surface.
 *
 * This functions returns the underlying surface buffer handle
 * suitable to eglCreateImageKHR() along with some additional
 * information describing the VA @surface size and buffer structure.
 *
 * @param[in]   dpy     the VA display
 * @param[in]   surface the VA surface
 * @param[out]  out_buffer_info the returned VA/EGL buffer information
 * @return VA_STATUS_SUCCESS if operation is successful, another #VAStatus
 *     value otherwise.
 */
VAStatus
vaGetSurfaceBufferEGL(
    VADisplay           dpy,
    VASurfaceID         surface,
    VABufferInfoEGL    *out_buffer_info
);

/**
 * \brief Returns the EGL client buffer info associated with a VA image.
 *
 * This functions returns the underlying image buffer handle suitable
 * to eglCreateImageKHR() along with some additional information
 * describing the VA @image size and buffer structure.
 *
 * Note: paletted formats are not supported. In this case,
 * VA_STATUS_ERROR_INVALID_IMAGE_FORMAT is returned.
 *
 * @param[in]   dpy     the VA display
 * @param[in]   image   the VA image
 * @param[out]  out_buffer_info the returned VA/EGL buffer information
 * @return VA_STATUS_SUCCESS if operation is successful, another #VAStatus
 *     value otherwise.
 */
VAStatus
vaGetImageBufferEGL(
    VADisplay           dpy,
    VAImageID           image,
    VABufferInfoEGL    *out_buffer_info
);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* VA_EGL_H */
