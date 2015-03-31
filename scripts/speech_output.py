#!/usr/bin/env python

import roslib; roslib.load_manifest('jimmy')
import rospy
import random
from time import sleep
from std_msgs.msg import String
from espeak import espeak
from jimmy.msg import *
import string
import random

#Things Jimmy can respond to:
#   - Hello
#       - Hello! How are you?
#   - My name is <NAME>.
#       - Ah. <NAME> is a wonderful name.
#   - How are you?
#       - If you wish for me to simulate having emotion, then so be it.
#         I am doing well. Thank you for asking.
#   - Thank you.
#       - You're welcome.
#   - What is your name?
#       - My name is Jimmy. I'm sorry... I can't seem to recall your
#         name. Would you mind reminding me?
#   - Haha!
#       - Haha, I am laughing!
#   - What is the weather tomorrow?
#       - What do you think I am? A useless Siri bot?


user_input = ''
previousSentence = ''
global userName


def callback(data):
    user_input = str(data)
    if len(user_input) > 0:
        sentence = user_input.lower()[6:]
        print "User: ", sentence

        if 'my name' in sentence:
            words = user_input.split(' ')
            for word in words:
                if word[0].isupper() and word != 'My':
                    userName = "".join(c for c in word if c.isalpha())
                    sleep(.5)
                    response = "Ah. " + userName + " is a wonderful name."
                else:
                    userName = "".join(c for c in random.choice(words) if c.isalpha())
                    sleep(.5)
                    response = "It seems that you are not intelligent enough to capitalize your own name. Therefore, I will randomly choose a word to call you. Let's see. . . . I shall call you . . . . . . " + userName + " ."
            print "Jimmy: ", response
            espeak.synth(response)
            print "Name: ", userName

        elif 'how are you' in sentence:
            sleep(.5)
            response = "If you wish for me to simulate having emotion, then so be it. I am doing well. Thank you for asking."
            print "Jimmy: ", response
            espeak.synth(response)

        elif 'hello' in sentence:
            sleep(.5)
            response = "Hello! How are you."
            print "Jimmy: ", response
            espeak.synth(response)

        elif 'thank' in sentence:
            response = "You're welcome."
            sleep(.5)
            print "Jimmy: ", response
            espeak.synth(response)

        elif 'your name' in sentence:
            # if len(userName) == 0:
            response = "My name is Jimmy. I'm sorry... I don't believe that you have told me your name yet. What is your name?"
            # else:
            #     response = "My name is Jimmy. How could you not know that? I know your name . . . . . " + userName + " ."
            sleep(.5)
            print "Jimmy: ", response
            espeak.synth(response)
        
        elif "haha" in sentence:
            response = "Haha, I am laughing!"
            sleep(.5)
            print "Jimmy: ", response
            espeak.synth(response)
        
        elif "weather" in sentence:
            response = "What do you think I am? A useless Siri bot?"
            sleep(.5)
            print "Jimmy: ", response
            espeak.synth(response)
        
        previousSentence = sentence


def listener():
    rospy.init_node('speech_listen', anonymous=True)
    rospy.Subscriber("textConversation", String, callback)
    rospy.spin()

if __name__ == '__main__':
    try:
        espeak.set_parameter(espeak.Parameter.Rate,140)
        listener()
    except rospy.ROSInterruptException:
        pass