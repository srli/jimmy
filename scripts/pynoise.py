#!/usr/bin/env python

"""
Created on Fri Mar 28 16:17:22 2014

@author: sophie
"""
#import os
#import pygame

##pygame.init()
##pygame.mixer.init() 
##print ("That's nice!")
#file = os.path.relpath("scripts/media/hello.wav")
##pygame.mixer.music.load(file)
##pygame.mixer.music.play()
##while pygame.mixer.music.get_busy(): 
##    pygame.time.Clock().tick(1)
#    
#    
#path = os.path.join(os.path.expanduser('~'), 'catkin_ws', 'src', 'jimmy', 'scripts','media')
#print (path)
#
#filepath = os.path.join(path, 'hello.wav')
#print filepath
#print os.path.dirname(os.path.realpath("media"))
#
#
#print os.path.relpath("scripts/media/hello.wav")

from espeak import espeak
import os
import subprocess

FNULL = open(os.devnull, 'w')
retcode = subprocess.call(['echo', 'foo'], stdout=FNULL, stderr=subprocess.STDOUT)

espeak.set_parameter(espeak.Parameter.Rate,150)
espeak.set_parameter(espeak.Parameter.Pitch,99)
#espeak.set_voice()

speech = raw_input("What should I say?  ")

def say(something):
    espeak.synth(something)
    
say(speech)