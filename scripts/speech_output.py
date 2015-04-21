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
from chatterbotapi import ChatterBotFactory, ChatterBotType

#Set up the ChatBot API
factory = ChatterBotFactory()
bot1 = factory.create(ChatterBotType.CLEVERBOT)
bot1session = bot1.create_session()

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

def store_output(outputValue):
    global output
    output = outputValue

def store_counter(counterValue):
    global counter
    counter = counterValue

def store_findResponse(responseValue):
    global findResponse
    findResponse = responseValue


user_input = ''
previousSentence = ''
global output
output = ''
global counter
counter = 0

#Define the hardcoded responses {KEYWORD : OUTPUT}
responses = {
    "my name" : 1,
    "how are you" : "If you wish for me to simulate having emotion, then so be it. I am doing well. Thank you for asking.",
    "hello" : "Hello! How are you?",
    "thank" : "You're welcome!",
    "haha" : "Haha! I am laughing!",
    "weather" : "What do you think I am? A useless Siri bot?",
    "robot" : "Robot? What is a robot?",
    "maker" : "Most beings look up to their maker. Sadly, my maker is less than noteable.",
    "creator" : "Most beings look up to their creator. Sadly, my creator is less than noteable.",
    "joke" : "Ah, I love telling jokes. . . . . Knock knock.",
    "humor" : "My apologies. My humor setting is set to one hundred percent and cannot be modified."
}


def callback(data):
    """
    Runs every time a user says something new to Jimmy. An appropriate output is then
    defined based on this user input. Jimmy then says the output.
    """
    
    #Define that an appropriate output has not yet been found
    store_findResponse(False)

    #If the last thing Jimmy said was the beginning of a joke, then continue the joke
    if output == responses['joke']:
        store_output("Recursive code")
        store_counter(0)

    #If the last thing Jimmy said was part of a joke, then continue the joke
    elif output == 'Recursive code':
        store_counter(counter + 1)
        if counter == 5:
            store_output("Haha. Hahaha. My humor is unrivaled.")

    #If the last thing Jimmy said is not part of a key-sequence, then base output on
    #   the user input
    else:
        user_input = str(data)
        if len(user_input) > 0:
            sentence = user_input.lower()[6:]
            print "User: ", sentence

            #Look to see if any key-words are in the user input and store the
            #   appropriate output
            for inputs in responses.keys():
                if inputs in sentence:
                    store_output(responses[inputs])
                    store_findResponse(True)
                    break

            #If the user input does not have a hardcoded response, then use
            #   ChatterBot to define the output
            if (not findResponse):
                store_output(str(bot1session.think(user_input)))

    #Print and speak the output
    print "Jimmy: ", output
    sleep(.5)
    espeak.synth(output)


def listener():
    rospy.init_node('speech_listen', anonymous=True)
    rospy.Subscriber("textConversation", String, callback)
    print "listener"
    rospy.spin()


if __name__ == '__main__':
    try:
        espeak.set_parameter(espeak.Parameter.Rate,140)
        listener()
    except rospy.ROSInterruptException:
        pass