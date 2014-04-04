# /usr/bin python

from array import array
from struct import pack
from sys import byteorder
import copy
import pyaudio
import wave
import audioop
from collections import deque
import os
import urllib2
import urllib


LANG_CODE = 'en-US'  # Language to use

GOOGLE_SPEECH_URL = 'https://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&pfilter=2&lang=%s&maxresults=6' % (LANG_CODE)

FLAC_CONV = 'flac -f'  # We need a WAV to FLAC converter. flac is available
                       # on Linux


THRESHOLD = 4000  # audio levels not normalised.
CHUNK_SIZE = 1024
SILENT_CHUNKS = 1 * 44100 / 1024  # about 3sec
FORMAT = pyaudio.paInt16
FRAME_MAX_VALUE = 2 ** 15 - 1
NORMALIZE_MINUS_ONE_dB = 10 ** (-1.0 / 20)
RATE = 16000
CHANNELS = 1
TRIM_APPEND = RATE / 4

def is_silent(data_chunk):
    """Returns 'True' if below the 'silent' threshold"""
    return max(data_chunk) < THRESHOLD

def normalize(data_all):
    """Amplify the volume out to max -1dB"""
    # MAXIMUM = 16384
    normalize_factor = (float(NORMALIZE_MINUS_ONE_dB * FRAME_MAX_VALUE)
                        / max(abs(i) for i in data_all))

    r = array('h')
    for i in data_all:
        r.append(int(i * normalize_factor))
    return r

def trim(data_all):
    _from = 0
    _to = len(data_all) - 1
    for i, b in enumerate(data_all):
        if abs(b) > THRESHOLD:
            _from = max(0, i - TRIM_APPEND)
            break

    for i, b in enumerate(reversed(data_all)):
        if abs(b) > THRESHOLD:
            _to = min(len(data_all) - 1, len(data_all) - 1 - i + TRIM_APPEND)
            break

    return copy.deepcopy(data_all[_from:(_to + 1)])

def record():
    """Record a word or words from the microphone and 
    return the data as an array of signed shorts."""

    p = pyaudio.PyAudio()
    stream = p.open(format=FORMAT, channels=CHANNELS, rate=RATE, input=True, output=True, frames_per_buffer=CHUNK_SIZE)

    silent_chunks = 0
    audio_started = False
    data_all = array('h')

    while True:
        # little endian, signed short
        data_chunk = array('h', stream.read(CHUNK_SIZE))
        if byteorder == 'big':
            data_chunk.byteswap()
        data_all.extend(data_chunk)

        silent = is_silent(data_chunk)

        if audio_started:
            if silent:
                silent_chunks += 1
                if silent_chunks > SILENT_CHUNKS:
                    break
            else: 
                silent_chunks = 0
        elif not silent:
            audio_started = True              

    sample_width = p.get_sample_size(FORMAT)
    stream.stop_stream()
    stream.close()
    p.terminate()

    data_all = trim(data_all)  # we trim before normalize as threshhold applies to un-normalized wave (as well as is_silent() function)
    data_all = normalize(data_all)
    return sample_width, data_all

def record_to_file(path):
    "Records from the microphone and outputs the resulting data to 'path'"
    sample_width, data = record()
    data = pack('<' + ('h' * len(data)), *data)

    wave_file = wave.open(path, 'wb')
    wave_file.setnchannels(CHANNELS)
    wave_file.setsampwidth(sample_width)
    wave_file.setframerate(RATE)
    wave_file.writeframes(data)
    wave_file.close()


def stt_google_wav(audio_fname):
    """ Sends audio file (audio_fname) to Google's text to speech 
        service and returns service's response. We need a FLAC 
        converter if audio is not FLAC (check FLAC_CONV). """

#    print "Sending ", audio_fname
#    #Convert to flac first
#    filename = audio_fname.name
#    del_flac = False
#    if 'flac' not in filename:
#        del_flac = True
#        print "Converting to flac"
#        print FLAC_CONV + filename
#        os.system(FLAC_CONV + ' ' + filename)
#        filename = filename.split('.')[0] + '.flac'
#    print filename
#    print type(filename)
    f = open(audio_fname, 'rb')
    flac_cont = f.read()
    f.close()

    # Headers. A common Chromium (Linux) User-Agent
    hrs = {"User-Agent": "Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.63 Safari/535.7", 
           'Content-type': 'audio/x-flac; rate=16000'}  

    req = urllib2.Request(GOOGLE_SPEECH_URL, data=flac_cont, headers=hrs)
    print "Sending request to Google TTS"
#    print "response", response
    try:
        p = urllib2.urlopen(req)
        response = p.read()
        res = eval(response)['hypotheses']
    except:
        print "Couldn't parse service response"
        res = None
#
#    if del_flac:
#        os.remove(filename)  # Remove temp file
#        
    return res

def main():
#    record_to_file('demo.wav')
    audiofile = file('demo.wav')
    print type(audiofile)
    fname = audiofile.name
    if 'flac' not in fname:
        print "Converting to flac"
        print FLAC_CONV + fname
        os.system(FLAC_CONV + ' ' + fname)
        fname = fname.split('.')[0] + '.flac'
#    print name, type(name)
    r = stt_google_wav('demo.flac')
    print r
    # Remove temp file. Comment line to review.
#    os.remove(audiofile)

if __name__ == '__main__':
    print("Wait in silence to begin recording; wait in silence to terminate")
#    record_to_file('demo.wav')
    main()
    print("done - result written to demo.wav")