#!/usr/bin/env python

import sys
import urllib
import urllib2

args = sys.argv

if len(args) < 2:
    print "Usage: python HttpClient.py [url]"
    exit(1)

url = args[1]
request = urllib2.Request(url)
response = urllib2.urlopen(request)
html = response.read()
print html

