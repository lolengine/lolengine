//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "lolgl.h"

namespace lol
{

#if LOL_BUILD_DEBUG
static std::map<GLenum, char const *> gl_dbg_source_to_str
{
    { GL_DEBUG_SOURCE_API, "API" },
    { GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Window System" },
    { GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compiler" },
    { GL_DEBUG_SOURCE_THIRD_PARTY, "Third Party" },
    { GL_DEBUG_SOURCE_APPLICATION, "Application" },
    { GL_DEBUG_SOURCE_OTHER, "Other" },
};

static std::map<GLenum, char const *> gl_dbg_type_to_str
{
    { GL_DEBUG_TYPE_ERROR, "Error" },
    { GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated Behaviour" },
    { GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Undefined Behaviour" },
    { GL_DEBUG_TYPE_PORTABILITY, "Portability" },
    { GL_DEBUG_TYPE_PERFORMANCE, "Performance" },
    { GL_DEBUG_TYPE_MARKER, "Marker" },
    { GL_DEBUG_TYPE_PUSH_GROUP, "Push Group" },
    { GL_DEBUG_TYPE_POP_GROUP, "Pop Group" },
    { GL_DEBUG_TYPE_OTHER, "Other" },
};

static std::map<GLenum, char const *> gl_dbg_severity_to_str
{
    { GL_DEBUG_SEVERITY_HIGH, "high" },
    { GL_DEBUG_SEVERITY_MEDIUM, "medium" },
    { GL_DEBUG_SEVERITY_LOW, "low" },
    { GL_DEBUG_SEVERITY_NOTIFICATION, "notification" },
};

static void gl_debug(GLenum source, GLenum type, GLuint id,
                     GLenum severity, GLsizei length,
                     const GLchar *message, const void *user_param)
{
    std::string error;

    switch (id)
    {
    default:
        if (gl_dbg_source_to_str.count(source) == 1)
            error += lol::format(" %s", gl_dbg_source_to_str[source]);
        if (gl_dbg_type_to_str.count(type) == 1)
            error += lol::format(" %s", gl_dbg_type_to_str[type]);
        if (gl_dbg_severity_to_str.count(severity) == 1)
            error += lol::format(" (%s)", gl_dbg_severity_to_str[severity]);
        msg::debug("GL%s [id:%x]: %s\n", error.c_str(), id, message);
        break;
    // Some debug messages are just annoying informational messages
    case 0x20061: // glFramebufferRenderbuffer
    case 0x20071: // glBufferData
        break;
    }
}

#if defined LOL_USE_GLEW && !defined __APPLE__
static void gl_debug_amd(GLuint id, GLenum category, GLenum severity,
                         GLsizei length, const GLchar* message,
                         GLvoid* user_param)
{
    gl_debug(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, id,
             severity, length, message, user_param);
}
#endif
#endif

void gpu::setup_debug()
{
#if LOL_BUILD_DEBUG
    GLint glflags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &glflags);

    if (glflags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        // Debug Output available

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#if defined LOL_USE_GLEW && !defined __APPLE__
        if (GLEW_ARB_debug_output)
            glDebugMessageCallbackARB(gl_debug, NULL);
        else if (GLEW_AMD_debug_output)
            glDebugMessageCallbackAMD(gl_debug_amd, NULL);
        else if (GLEW_KHR_debug)
#endif
            glDebugMessageCallback(gl_debug, NULL);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    }
#endif
}

void gpu::error(char const *msg, int error)
{
    msg::error("%s: GL error 0x%04x\n", msg, error);
}

}

