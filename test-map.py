#!/usr/bin/env python

import os

import OpenGL
OpenGL.ERROR_CHECKING = False
from OpenGL.GL import *
from OpenGL.GLU import *

import pygame, pygame.image
from pygame.locals import *

textures = [0,0]

def resize((width, height)):
    glViewport(0, 0, width, height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    glOrtho(0, width, height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

def init():
    glEnable(GL_TEXTURE_2D)
    load_textures()
    glShadeModel(GL_SMOOTH)
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glClearDepth(1.0)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LEQUAL)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)

def load_textures():
    texturefile = os.path.join('art','test','groundtest.png')
    textureSurface = pygame.image.load(texturefile)
    textureData = pygame.image.tostring(textureSurface, "RGBX", 1)

    glBindTexture(GL_TEXTURE_2D, textures[0])
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, textureSurface.get_width(), textureSurface.get_height(), 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, textureData );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)

def put_map(themap):
    glBegin(GL_QUADS)
    y = 0.0
    for line in themap:
        x = 0.0
        for tile in line:
            sy = .0625 * (15 - tile / 16)
            sx = .0625 * (tile % 16)
            glTexCoord2f(sx, sy); glVertex2f(x, y + 32.0);
            glTexCoord2f(sx + .0625, sy); glVertex2f(x + 32.0, y + 32.0);
            glTexCoord2f(sx + .0625, sy + .0625); glVertex2f(x + 32.0, y);
            glTexCoord2f(sx, sy + .0625); glVertex2f(x, y);
            x += 32.0
        y += 32.0
    glEnd();				

def draw():
    #glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
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
        [ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 ],
        [ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 ],
        [ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 ],
        [ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 ],
        [ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 ],
        [ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 ],
    ]
    put_map(themap)

def main():

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
#        if frames > 200:
#            break

    print "fps:  %d" % ((frames*1000)/(pygame.time.get_ticks()-start))


if __name__ == '__main__': main()

#import cProfile
#cProfile.run('main()')


