#!/usr/bin/python

import urllib2
url = "https://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&lang=en-US"
audio = open('output_1396413797.flac','rb').read()
headers={'Content-Type': 'audio/x-flac; rate=16000', 'User-Agent':'Mozilla/5.0'}
request = urllib2.Request(url, data=audio, headers=headers)
response = urllib2.urlopen(request)
print response.read()

#f = open('demo.flac')
#data = f.read()
#f.close()
#
#req = urllib2.Request('https://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&lang=en-US', data=data, headers={'Content-type': 'audio/x-flac; rate=16000'})
#
#try:
#    ret = urllib2.urlopen(req)
#except urllib2.URLError:
#    print "Error Transcribing Voicemail"
#    sys.exit(1)
#
#resp = ret.read()
#text = json.loads(resp)['hypotheses'][0]['utterance']
#print text