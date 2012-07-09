Name

    INTEL_VA_pixel_buffer

Name Strings

    EGL_INTEL_VA_pixel_buffer

Contact

    Gwenole Beauchesne, Intel Corporation (gwenole.beauchesne 'at' intel.com)

Status

    Proposal

Version

    Version 1, July 09, 2012

Number

    EGL Extension #not assigned

Dependencies

    Requires EGL 1.4 or later. This extension is written against the
    wording of the EGL 1.4 specification.

    EGL_KHR_image_base is required.

    Requires VA-API 0.33 or later. This extension is written against
    the wording of the VA/EGL API version 1 specification.

Overview

    This extension provides a mechanism for creating EGLImage objects
    from VA-API resources.

IP Status

    Open-source; freely implementable.

New Tokens

    Accepted in the <target> parameter of eglCreateImageKHR:

        EGL_VA_PIXEL_BUFFER_INTEL               0x31DB

    Accepted in the <attrib_list> parameter of eglCreateImageKHR:

        EGL_VA_BUFFER_PLANE_INTEL               0x31D6
        EGL_VA_BUFFER_STRUCTURE_INTEL           0x3080
        EGL_VA_PICTURE_STRUCTURE_INTEL          0x31DA

    Accepted as values for the EGL_VA_BUFFER_STRUCTURE_INTEL
    attribute, as exposed in <va/va_egl.h> from VA-API:

        VA_EGL_BUFFER_STRUCTURE_RGBA            0x305E
        VA_EGL_BUFFER_STRUCTURE_RGB             0x305D
        VA_EGL_BUFFER_STRUCTURE_Y_UV            0x31D8
        VA_EGL_BUFFER_STRUCTURE_Y_U_V           0x31D7

    Accepted as values for the EGL_VA_PICTURE_STRUCTURE_INTEL
    attribute, as exposed in <va/va_egl.h> from VA-API:

        VA_EGL_PICTURE_STRUCTURE_FRAME          0x0000
        VA_EGL_PICTURE_STRUCTURE_TOP_FIELD      0x0001
        VA_EGL_PICTURE_STRUCTURE_BOTTOM_FIELD   0x0002

Additions to the EGL 1.4 Specification:

    Import a va_egl_client_buffer by calling eglCreateImageKHR with
    va_egl_client_buffer as EGLClientBuffer, EGL_VA_PIXEL_BUFFER_INTEL
    as the target, NULL context and an empty attrib_list. In case of
    multi-planar buffers such as many YUV buffers (e.g. NV12), use the
    EGL_VA_BUFFER_PLANE_INTEL attribute to specify the plane to create
    the EGLImage for. The value of the attribute is the index of the
    plane, as defined by the buffer structure.

    If no EGL_VA_BUFFER_PLANE_INTEL attribute is specified, then the
    plane at index zero is implied.

    The EGL_VA_BUFFER_STRUCTURE_INTEL attribute indicates that the
    client application would like the implementation to treat the
    pixel buffer with the specified structure. This is most useful
    with multi-planar YUV buffers, so that they could be used as a
    single EGLImage with packed YUV pixel values. Should the
    implementation not support buffer structure coercion, then
    eglCreateImageKHR shall return EGL_NO_IMAGE_KHR.

    If no EGL_VA_BUFFER_STRUCTURE_INTEL attribute is specified, then
    the internal buffer structure is implied. In this case, it is up
    to the client application to retain and use the buffer structure
    as returned by the vaGetSurfaceBufferEGL() function.

    The EGL_VA_PICTURE_STRUCTURE_INTEL attribute indicates that the
    client application wants the implementation to treat the pixel
    buffer as a whole "frame" or interlaced field. If interlaced,
    implementations shall only honour this attribute, by returning a
    valid EGLImage, if it is able to skip every over line implicitly.

    It is suggested that most implementations could support the
    "top-field" structure (VA_EGL_PICTURE_STRUCTURE_TOP_FIELD) by
    considering a line stride twice as large as the reported internal
    stride for the selected plane.

    Likewise, most implementations may support the "bottom-field"
    structure (VA_EGL_PICTURE_STRUCTURE_BOTTOM_FIELD) by considering a
    line stride twice as large as the reported internal stride for the
    selected plane and a pixel buffer base offset by the reported
    internal stride for the selected plane.

    If no EGL_VA_PICTURE_STRUCTURE_INTEL attribute is specified, then
    the "frame" structure (VA_EGL_PICTURE_STRUCTURE_FRAME) is implied,
    even if the pixel buffer actually represents interlaced contents.

Revision History

    Version 1, July 09, 2012
        Initial draft (Gwenole Beauchesne)
