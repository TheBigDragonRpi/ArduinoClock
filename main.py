import numpy as np
from PIL import Image
from time import sleep as sleep
import math

z=[]
u={}
d={}
h={}
m={}

for i in range(1,10):
    u[i] = []
    d[i] = []
    h[i] = []
    m[i] = []

size = 240

mask = Image.new("L",(size,size),255)
im1 = Image.open('im1.jpg').resize(mask.size)
im2 = Image.new("L",(size,size),13)

def c(x,y,r):
    return((r-5)**2<(x-(size-1)/2)**2+(y-(size-1)/2)**2<r**2)

def e(x,y,r):
    return((r**2)-(63*(math.sqrt(abs(y-((size-1)/2)))))<((x-(size-1)/2)/2)**2+((y-(size-1)/2)/1.078)**2<r**2)

def l(y,p):
    return(p+5>abs(y-(size-1)/2)>=p)

"""for x in range(size):
    for y in range(size):
        for num in time:
            if number(x,y):
                if((not(5>y-(size-1)/2>-5)) and not(((x-(size-1)/2)**2+(y-(size-1)/2)**2)<61**2)):
                    if(c(x,y,120) or c(x,y,66) or e(x,y,61) or e(y,x,61) or l(x,0) or l(y,5)):
                        mask.putpixel((x,y),0)"""
def setNum(lvl,x,y):
    if(c(x,y,120)):
        lvl[1].append((x,y))
    if(c(x,y,66)):
        lvl[2].append((x,y))
    if(e(x,y,60)): 
        lvl[4].append((x,y))
    if(e(y,x,60)):
        lvl[3].append((x,y))
    if(l(y,5)):
        lvl[6].append((x,y))


for x in range(size):
    for y in range(size):
        if((not(5>y-(size-1)/2>-5)) and not(((x-(size-1)/2)**2+(y-(size-1)/2)**2)<61**2)):
            if(l(x,0)):
                z.append((x,y))
            if(y<=(size-1)/2):
                if(x>=(size-1)/2): #u
                    setNum(u,x,y)
                else: #d
                    setNum(d,x,y)
            else:
                if(x>=(size-1)/2): #u
                    setNum(h,x,y)
                else: #d
                    setNum(m,x,y)
def clock(number,lvl,hour=""):
    global mask
    for pos in z:
        mask.putpixel(pos,0)
    if(number==0):
        return
    if(number==5):
        for pos in lvl[4]:
            mask.putpixel(pos,0)
        for pos in lvl[1]:
            mask.putpixel(pos,0)
    if(number==7):
        for pos in lvl[6]:
            mask.putpixel(pos,0)
        for pos in lvl[1]:
            mask.putpixel(pos,0)
    if(number==8):
        for pos in lvl[6]:
            mask.putpixel(pos,0)
        for pos in lvl[2]:
            mask.putpixel(pos,0)
    if(number==9):
        for pos in lvl[6]:
            mask.putpixel(pos,0)
        for pos in lvl[2]:
            mask.putpixel(pos,0)
        for pos in lvl[1]:
            mask.putpixel(pos,0)
    for pos in lvl[number]:
        mask.putpixel(pos,0)
    im = Image.composite(im2, im1, mask)
    if hour!="":
        im.save("img{}.jpg".format(hour))

"""for dhour in range(3):
    for uhour in range(10):
        if(dhour==2 and uhour>3):
            break
        for dmin in range(6):
            for umin in range(10):
                mask = Image.new("L",(size,size),255) 
                clock(dhour,m)
                clock(uhour,h)
                clock(dmin,d)
                # clock(umin,u,str(str(dhour)+str(uhour)+str(dmin)+str(umin)))
                clock(umin,u,"animated")
                sleep(.3)
"""

print(u,d,h,m,z)
        