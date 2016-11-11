# -*- coding: utf-8 -*-
"""
Created on Sun Dec 13 19:01:55 2015

@author: Zhan
"""
import numpy as np
import cv2
import sys

def vframes(fb,fe,st,num,files):    
    print files
    cap = cv2.VideoCapture(files)
    print cap.isOpened()
    cap.set(cv2.cv.CV_CAP_PROP_POS_MSEC,fb)   
    rate=cap.get(cv2.cv.CV_CAP_PROP_FPS)+1
    totframe=float((fe-fb)*rate/1000)
    deltaf=float(totframe/st)
    base=int(fb*rate/1000)
    t=0
    n=0
    print deltaf
    while t<totframe:
        ret,frame=cap.read()
        cv2.imwrite('./temp/images/frame_'+num+'_'+str(n)+'.jpg',frame)
        print t
        #cv2.imshow('frame',frame)
        t=t+deltaf
        cap.set(cv2.cv.CV_CAP_PROP_POS_FRAMES,base+int(t)-1)
        n=n+1
        
fb=int(sys.argv[1])
fe=int(sys.argv[2])
st=int(sys.argv[3])
num=sys.argv[4]
vframes(fb,fe,st,num,sys.argv[5])
#vframes(4500,5500,10,r'op.mp4')