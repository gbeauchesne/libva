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

/*
 * Initial EGL backend, and subject to change
 *
 * Gstreamer gst-gltexture has a framework to support associating a buffer
 * to a texture via EGL_KHR_image_base and GL_OES_EGL_image_external.
 *
 * EGL_KHR_image_base:
 *   EGLImageKHR eglCreateImageKHR(
 *                           EGLDisplay dpy,
 *                           EGLContext ctx,
 *                           EGLenum target,
 *                           EGLClientBuffer buffer,
 *                           const EGLint *attrib_list)
 *
 * GL_OES_EGL_image_external:
 * This extension provides a mechanism for creating EGLImage texture targets
 * from EGLImages.  This extension defines a new texture target TEXTURE_EXTERNAL_OES.
 * This texture target can only be specified using an EGLImage.
 * The first eglCreateImageKHR will create an EGLImage from an EGLClientBufferm, and with
 * an EGLImage, gst-gltexture can use GL_OES_EGL_image_external extension to create textures.
 *
 * eglCreateImageKHR and GL_OES_EGL_image_external are all called directly from gst-gltexture,
 * thus the simplest way to support gst-gltexture is defining a new API to pass EGLClientBuffer
 * to gst-gltexture.
 *
 * EGLClientBuffer is gfx/video driver implementation specific (?). It means we need to pass up
 * the low-level buffer ID (or handle) of the decoded surface to gst-gltexture, and gst-gltexture
 * then pass down it to gfx driver.  
 *
 * Bellow API vaGetEGLClientBufferFromSurface is for this purpose
 */

#include "sysdeps.h"
#include "va.h"
#include "va_backend_egl.h"
#include "va_egl.h"

#define CTX(dpy) (((VADisplayContextP)dpy)->pDriverContext)
#define CHECK_DISPLAY(dpy) if( !vaDisplayIsValid(dpy) ) { return VA_STATUS_ERROR_INVALID_DISPLAY; }

static inline int
check_vtable(struct VADriverVTableEGL *vtable)
{
    return vtable && ((vtable->version & 0xffffff00) == VA_EGL_ID);
}

VAStatus
vaGetEGLClientBufferFromSurface (
    VADisplay           dpy,
    VASurfaceID         surface,
    EGLClientBuffer    *buffer /* out */
)
{
    VADriverContextP ctx;
    struct VADriverVTableEGL_Deprecated *va_egl;

    CHECK_DISPLAY(dpy);
    ctx = CTX(dpy);

    /* Check we are not using the new VTable */
    if (check_vtable(ctx->vtable_egl))
        return VA_STATUS_ERROR_UNIMPLEMENTED;

    /* Assume deprecated VA/EGL VTable */
    va_egl = (struct VADriverVTableEGL_Deprecated *)ctx->vtable_egl;
    if (!va_egl->vaGetEGLClientBufferFromSurface)
        return VA_STATUS_ERROR_UNIMPLEMENTED;
    return va_egl->vaGetEGLClientBufferFromSurface(ctx, surface, buffer);
}

/* Returns the EGL client buffer associated with a VA surface */
VAStatus
vaGetSurfaceBufferEGL(
    VADisplay           dpy,
    VASurfaceID         surface,
    EGLClientBuffer    *out_buffer
)
{
    VADriverContextP ctx;
    struct VADriverVTableEGL *vtable;

    CHECK_DISPLAY(dpy);
    ctx = CTX(dpy);

    vtable = ctx->vtable_egl;
    if (!check_vtable(vtable) || !vtable->vaGetSurfaceBufferEGL)
        return VA_STATUS_ERROR_UNIMPLEMENTED;
    return vtable->vaGetSurfaceBufferEGL(ctx, surface, out_buffer);
}

/* Returns the EGL client buffer associated with a VA image */
VAStatus
vaGetImageBufferEGL(
    VADisplay           dpy,
    VAImageID           image,
    EGLClientBuffer    *out_buffer
)
{
    VADriverContextP ctx;
    struct VADriverVTableEGL *vtable;

    CHECK_DISPLAY(dpy);
    ctx = CTX(dpy);

    vtable = ctx->vtable_egl;
    if (!check_vtable(vtable) || !vtable->vaGetImageBufferEGL)
        return VA_STATUS_ERROR_UNIMPLEMENTED;
    return vtable->vaGetImageBufferEGL(ctx, image, out_buffer);
}

/* Queries VA/EGL buffer attributes */
VAStatus
vaGetBufferAttributeEGL(
    VADisplay           dpy,
    EGLClientBuffer     buffer,
    EGLenum             attribute,
    EGLint             *value
)
{
    VADriverContextP ctx;
    struct VADriverVTableEGL *vtable;

    CHECK_DISPLAY(dpy);
    ctx = CTX(dpy);

    vtable = ctx->vtable_egl;
    if (!check_vtable(vtable) || !vtable->vaGetBufferAttributeEGL)
        return VA_STATUS_ERROR_UNIMPLEMENTED;
    return vtable->vaGetBufferAttributeEGL(ctx, buffer, attribute, value);
}
