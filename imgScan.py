
# D = (r * tan(asin(edgeDist / r)) + robotHeight) / tan(asin(edgeDist / r))
# D = (r * tan(asin(edgeDist / r)) + robotHeight - barrelHeight) / tan(asin(edgeDist / r))
#
#
# image array is in format [y][x][rgb]

import matplotlib.image as mpimg
from numpy import ndarray
import math


def isRed(pixel):
	if type(pixel) != ndarray:
		print "!--Error--> wrong type passed to isRed"
		return
	if len(pixel) != 3:
		print "!--Error--> wrong length passed to isRed"
		return
	if (pixel[0] > 2*pixel[1]) and (pixel[0] > 2*pixel[2]) and (pixel[0] > 0.4):
		return True
	else:
		return False

def scanHorizontalLineForRed(image, lineInt):
	if type(image) != ndarray:
		print "!--Error--> var 'image' in scanHorizontalLineForRed must be ndarray"
		return []
	if type(lineInt) != int:
		print "!--Error--> var 'lineInt' in scanHorizontalLineForRed must be int"
		return []
	pixelIndices=[]
	for i in range(image.shape[1]):
		if isRed(image[lineInt][i]):
			pixelIndices.append(i)
	return pixelIndices
	
def scanVerticalLineForRed(image, lineInt):
	if type(image) != ndarray:
		print "!--Error--> var 'image' in scanVerticalLineForRed must be ndarray"
		return []
	if type(lineInt) != int:
		print "!--Error--> var 'lineInt' in scanVerticalLineForRed must be int"
		return []
	pixelIndices=[]
	for i in range(image.shape[0]):
		if isRed(image[i][lineInt]):
			pixelIndices.append(i)
	return pixelIndices
	
#Take the image
#Scan through every line
#How do I determine if the cylinder has begun?
#I want a solid line of reds. I want the last line to have some red
#

img=mpimg.imread('testInputData/red_cylinder.png')
topIndices=[]
topCorner=[]
bottomCorner=[]
bottomIndices=[]
indices=[]

for i in range(img.shape[0]):
	lastIndices = indices
	indices = scanHorizontalLineForRed(img, i)
	if len(indices) > 0 and len(lastIndices) > 0:
		if len(topIndices) == 0:
			topIndices = [i-1,lastIndices[len(lastIndices)/2] ]
		elif len(indices) == len(lastIndices) and len(topCorner) == 0:
			topCorner = [i-1, lastIndices[0], i-1, lastIndices[len(lastIndices)-1] ]
		elif len(bottomCorner) == 0 and len(indices) < len(lastIndices):
			bottomCorner = [i-1, lastIndices[0], i-1, lastIndices[len(lastIndices)-1] ]
	elif len(bottomIndices) == 0 and len(indices) == 0 and len(lastIndices) > 0:
		bottomIndices = [i-1,lastIndices[len(lastIndices)/2] ]

if len(topIndices) > 0: print "top middle = " + str(topIndices[0]) + ", " + str(topIndices[1])
else: print "topIndices is empty!"
if len(topCorner) > 0: print "top corner = " + str(topCorner[0]) + ", " + str(topCorner[1]) + " and " + str(topCorner[2]) + ", " + str(topCorner[3])
else: print "topCorner is empty!"
if len(bottomCorner) > 0: print "bottom corner = " + str(bottomCorner[0]) + ", " + str(bottomCorner[1]) + " and " + str(bottomCorner[2]) + ", " + str(bottomCorner[3])
else: print "bottomCorner is empty!"
if len(bottomIndices) > 0: print "bottom middle = " + str(bottomIndices[0]) + ", " + str(bottomIndices[1])
else: print "bottomIndices is empty!"

# D = (r * tan(asin(edgeDist / r)) + robotHeight) / tan(asin(edgeDist / r))
# D = (r * tan(asin(edgeDist / r)) + robotHeight - barrelHeight) / tan(asin(edgeDist / r))
#
#distance from the ground is 2
#height of barrel is 5
#radius is 1.5
radius = 1.5
barrelHeight = 5
robotHeight = 2

edgeDis = float(topIndices[0] - topCorner[0]) / float(topIndices[1] - topCorner[1])
edgeDis2 = float(bottomIndices[0] - bottomCorner[0]) / float(bottomIndices[1] - bottomCorner[1])

dis2 = (radius * math.tan(math.asin(edgeDis2)) + robotHeight) / math.tan(math.asin(edgeDis2))
dis = (radius * math.tan(math.asin(edgeDis)) + robotHeight - barrelHeight) / math.tan(math.asin(edgeDis))

print dis2
print dis
