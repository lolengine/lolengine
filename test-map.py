#!/usr/bin/env python

import os

import OpenGL
OpenGL.ERROR_CHECKING = False
from OpenGL.GL import *
from OpenGL.GLU import *

import pygame, pygame.image
from pygame.locals import *

import numpy
from math import sin, cos

textures = [0,0]
buflist = False

def resize((width, height)):
    glViewport(0, 0, width, height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    glOrtho(0, width, height, 0, -1, 10);
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

def init():
    glEnable(GL_TEXTURE_2D)
    load_textures()
    make_vbo()
    glShadeModel(GL_SMOOTH)
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glClearDepth(1.0)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LEQUAL)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)

    #glEnable(GL_ALPHA_TEST)
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    #glBlendFunc(GL_SRC_ALPHA, GL_ONE)


def load_textures():
    texturefile = os.path.join('art','test','groundtest.png')
    textureSurface = pygame.image.load(texturefile)
    textureData = pygame.image.tostring(textureSurface, "RGBA", 1)

    glBindTexture(GL_TEXTURE_2D, textures[0])
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, textureSurface.get_width(), textureSurface.get_height(), 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, textureData );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)

def make_vbo():
    global buflist
    buflist = glGenBuffers(3)

    vertices = [False] * (20 * 15)
    for y in range(15):
        for x in range(20):
            ty = y * 32
            tx = x * 32
            # Z coord is used for blit order
            vertices[x + y * 20] = [tx, ty, y * 0.01,
                                    tx + 32, ty, y * 0.01,
                                    tx + 32, ty + 32, y * 0.01,
                                    tx, ty + 32, y * 0.01]
    vertices = numpy.array(vertices, dtype=numpy.float32)
    glBindBuffer(GL_ARRAY_BUFFER, buflist[0])
    glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW)

    indices = numpy.array([x for x in range(4 * 20 * 15)], dtype=numpy.int)
    glBindBuffer(GL_ARRAY_BUFFER, buflist[2])
    glBufferData(GL_ARRAY_BUFFER, indices, GL_STATIC_DRAW)

def put_map(themap):
    uvs = [False] * (20 * 15)
    index = 0
    for line in themap:
        for tile in line:
            ty = .0625 * (15 - tile / 16)
            tx = .0625 * (tile % 16)
            uvs[index] = [tx, ty + .0625,
                          tx + .0625, ty + .0625,
                          tx + .0625, ty,
                          tx, ty]
            index += 1
    uvs = numpy.array(uvs, dtype=numpy.float32)
    glBindBuffer(GL_ARRAY_BUFFER, buflist[1])
    glBufferData(GL_ARRAY_BUFFER, uvs, GL_STATIC_DRAW)

    glEnableClientState(GL_VERTEX_ARRAY)
    glEnableClientState(GL_TEXTURE_COORD_ARRAY)
    glEnableClientState(GL_INDEX_ARRAY)

    glBindTexture(GL_TEXTURE_2D, textures[0])

    glBindBuffer(GL_ARRAY_BUFFER, buflist[0])
    glVertexPointer(3, GL_FLOAT, 0, None)
    glBindBuffer(GL_ARRAY_BUFFER, buflist[1])
    glTexCoordPointer(2, GL_FLOAT, 0, None)
    glBindBuffer(GL_ARRAY_BUFFER, buflist[2])
    glIndexPointer(GL_INT, 0, None)

    glDrawArrays(GL_QUADS, 0, 4 * 20 * 15)

    glDisableClientState(GL_VERTEX_ARRAY)
    glDisableClientState(GL_TEXTURE_COORD_ARRAY)
    glDisableClientState(GL_INDEX_ARRAY)

def draw():
    #glClear(GL_DEPTH_BUFFER_BIT) # Full redraw: no need to clear color buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()

    themap = [
        [ 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 ],
        [ 17,  0,  1,  1,  1, 33,  1,  1,  1,  1,  1,  1,  2, 33,  3, 17, 17, 17, 17, 17 ],
        [ 17, 16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 16, 17, 19,  3, 17, 17, 17, 17 ],
        [ 17, 18, 17, 48, 49, 50, 48, 49, 50, 48, 49, 17, 16, 17, 16, 18, 17, 17, 17, 17 ],
        [ 17, 16, 17, 48, 49, 50, 48, 49, 50, 48, 49, 17, 16, 17, 16, 16, 17, 17, 17, 17 ],
        [ 17, 16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18, 17, 19, 35, 17, 17, 17, 17 ],
        [ 17, 32,  1,  1,  1,  1,  1,  1,  1,  1, 33,  1, 34,  1, 35, 17, 17, 17, 17, 17 ],
        [ 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 ],
        [ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 ],
        [ 51, 51, 52, 52, 52, 51, 52, 52, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 ],
        [ 51, 51, 52, 52, 52, 51, 51, 52, 51, 52, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 ],
        [ 51, 51, 52, 52, 52, 51, 51, 51, 51, 51, 51, 52, 52, 51, 51, 51, 51, 51, 51, 51 ],
        [ 51, 51, 52, 52, 52, 51, 51, 51, 51, 51, 51, 52, 52, 51, 51, 51, 51, 51, 51, 51 ],
        [ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 ],
        [ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 ],
    ]
    glPushMatrix()
    glTranslatef(50.0 * sin(frames * 0.05), 50.0 * cos(frames * 0.08), 0)
    put_map(themap)
    glPopMatrix()
    glTranslatef(0, 0, 0.2)
    put_map(themap)

frames = 0

def main():
    global frames

    video_flags = OPENGL|DOUBLEBUF

    pygame.init()
    surface = pygame.display.set_mode((640,480), video_flags)

    resize((640,480))
    init()

    frames = 0
    ticks = pygame.time.get_ticks()
    start = ticks
    while 1:
        event = pygame.event.poll()
        if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
            break

        # Enforce 33 fps
        while pygame.time.get_ticks() < ticks + 33:
            pygame.time.wait(1)
        ticks = pygame.time.get_ticks()

        draw()
        pygame.display.flip()
        frames = frames+1
        #if frames > 200:
        #    break

    print "fps:  %d" % ((frames*1000)/(pygame.time.get_ticks()-start))


if __name__ == '__main__': main()

#import cProfile
#cProfile.run('main()')


