import sensor, time, image, pyb, ustruct, ulab
from ulab import numpy as np

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_contrast(3)
sensor.set_brightness(1)
sensor.skip_frames(30)
sensor.set_auto_gain(True)
sensor.set_auto_whitebal(False)
sensor.set_vflip(False)
sensor.set_hmirror(True)

uart = pyb.UART(3, 19200)
usb = pyb.USB_VCP()
clock = time.clock()

distance = 0
angle = 0

def getCam(threshold):
    pixels = [0]
    rectSpace = [0]
    cnt = 0
    dx = dy = a = angle = cnt = 0

    for blob in img.find_blobs(threshold, pixels_threshold = 10, area_threshold = 100, merge = True, margin = 10):
        pixels.append(blob.pixels())
        rectSpace.append(blob.rect())
        cnt += 1

    maxVal = max(pixels)
    num = pixels.index(maxVal)

    if cnt > 0:
        dx = rectSpace[num][0] + int(rectSpace[num][2] / 2)
        dy = rectSpace[num][1] + int(rectSpace[num][3] / 2)
        distance = int(np.sqrt(pow(160 - dx,2) + pow(120 - dy,2)))
        angle = 90 - int(np.degrees(np.arctan2(120 - dy,dx - 160)))
        a = (120 - dy) / (dx - 160)
        if angle < 0:
            angle += 360

    return angle, dx, dy, a, cnt

while True:
    try:
        clock.tick()
        img = sensor.snapshot()
        blue = getCam([(0, 100, -6, 12, -35, -17)])
        #yellow = getCam([(0, 100, -3, 7, 26, 127)])

        bAngle = -1

        if blue[4] >= 0:
            bAngle = blue[0]
        #if yellow[4] >= 0:
            #yAngle = yellow[0]

        print(bAngle)

        #if usb.isconnected():
            #img.draw_cross(160,120,(0,0,0))
            #img.draw_circle(160,120,120,(255,255,255))
            #img.draw_cross(blue[1],blue[2],(0,0,0))
            #img.draw_cross(yellow[1],yellow[2],(0,0,0))
            #img.draw_line(160,120,blue[1],blue[2],(0,0,255))
            #img.draw_line(160,120,yellow[1],yellow[2],(255,255,0))
            #img.draw_string(10,10,"Angle:%d" % angle,(0,0,0),scale = 2, mono_space = False)
            #img.draw_string(10,30,"Tit:%f Dis:%d" % (a, distance),(0,0,0),scale = 2, mono_space = False)

        #uart.write(str(bAngle)+" "+str(yAngle)+'\0')
        #time.sleep(1)
        #uart.write(ustruct.pack('B',bAngle))
        uart.write(str(bAngle)+'\0')



    except (ZeroDivisionError, RuntimeError, OSError) as err:
        if usb.isconnected():
            print(err)
        pass
