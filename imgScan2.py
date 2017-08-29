
# D = (r * tan(asin(edgeDist / r)) + robotHeight) / tan(asin(edgeDist / r))
# D = (r * tan(asin(edgeDist / r)) + robotHeight - barrelHeight) / tan(asin(edgeDist / r))
#
#
# image array is in format [y][x][rgb]

################################################################################

import math

radius = 1.5
barrelHeight = 5.0
robotHeight = 2.0
barrelWidth = 2 * radius

cHeight = robotHeight
cDist = 5
pixHeight = 0.002820972
pixWidth = pixHeight
#cHeight = (bottomIndices[0] - float(img.shape[0]/2) ) * pixHeight
#fHeight = robotHeight
# fdist / fheight = cdist / cheight
# fdist = cdist * fheight / cheight
#fDist = radius + cDist * fHeight / cHeight

################################################################################

import matplotlib.image as mpimg
import matplotlib.pyplot as plt
from numpy import ndarray
import math

class Vector:
	def __init__(self, x, y):
		self.x = x
		self.y = y
class Point:
	def __init__(self, x, y):
		self.x = x
		self.y = y

def isRed(pixel):
	if type(pixel) != ndarray:
		print "!--Error--> wrong type passed to isRed"
		return
	if len(pixel) != 3:
		print "!--Error--> wrong length passed to isRed"
		return
	if (pixel[0] > 2*pixel[1]) and (pixel[0] > 2*pixel[2]) and (pixel[0] > 0.1):
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
		if isRed(image[lineInt][i]) and (not isRed(image[lineInt][i-1]) or i == 0) :
			pixelIndices.append(i)
	return pixelIndices

#Scan a select portion of a horizontal line, returning an array of indices
#Every even index(0, 2, 4, etc) denotes when red begins, every odd (1, 3, 5, etc)
#is the pixel after the last red pixel in a line

def scanH2(image, y, xStart, xFinish):
	if type(image) != ndarray:
		print "!--Error--> var 'image' in scanHorizontalLineForRed must be ndarray"
		return []
	pixelIndices=[]
	for x in range(xStart, xFinish):
		if isRed(image[y][x]):
			if x == xStart or not isRed(image[y][x-1]):
				pixelIndices.append([x])
			if x == image.shape[1] - 1 or not isRed(image[y][x+1]):
				pixelIndices[-1].append(x+1)
	return pixelIndices

##
#New scan method
#Scan vertically from the middle
#Compare the barrel's radius and robot's height to the measured ratio
#Depending on how off this is, scan other portions of the red body
#Expect no more than one hidden barrel.

def scanBody3(image, y, xStart, xFinish, radius, roHeight):
	midPoint = (xFinish + xStart) / 2
	pHeight = 0
	while(isRed(image[y+pHeight][midPoint])):
		pHeight += 1
	pWidth = xFinish - xStart
	cWidth = pixWidth * pWidth
	cHeight = pixHeight * pHeight
	# if cWidth is not within 10% of 3/2 of cHeight, then I need to look for
	# other points depending on the height I do find
	if(abs(cWidth-1.5*cHeight) <= .1*1.5*cHeight):
		#I have one cylinder
		return {"midPoint":[midPoint], "bottomPoint":[y+pHeight]}
	elif(cWidth < 1.5 * cHeight):
		#I assume there is one and it is partially hidden.

		#test two heights, take the greater as nearer the centerside
		midPoint1 = (xStart + midPoint) / 2
		midPoint2 = (midPoint + xFinish) / 2
		pHeight1 = 0
		while(isRed(image[y+pHeight1][midPoint1])):
			pHeight1 += 1
		pHeight2 = 0
		while(isRed(image[y+pHeight2][midPoint2])):
			pHeight2 += 1
		if(pHeight1 > pHeight): pHeight = pHeight1
		if(pHeight2 > pHeight): pHeight = pHeight2
		if(pHeight1 > pHeight2):
			pWidth = int(math.floor(1.5 * pHeight + 0.5))
			midPoint = xFinish - pWidth/2
		elif(pHeight2 > pHeight1):
			pWidth = int(math.floor(1.5 * pHeight + 0.5))
			midPoint = xStart + pWidth/2
		if(midPoint < 0 or midPoint > image.shape[1]):
			return {"midPoint":[midPoint],"bottomPoint":[y+pHeight]}
		else:
			pHeight = 0
			print "pHeight: " + str(pHeight) + " midPoint: " + str(midPoint)
			while(isRed(image[y+pHeight][midPoint])):
				pHeight += 1
			return {"midPoint":[midPoint],"bottomPoint":[y+pHeight]}

	elif(cWidth > 1.5 * cHeight):
		#I assume there are two.
		pWidth = math.floor(1.5 * pHeight + 0.5)
		midLeft = int(math.floor(((xFinish - pWidth) + xStart)/2))
		midRight = int(math.floor(((xStart + pWidth) + xFinish)/2))
		lHeight = 0
		rHeight = 0
		pHeight2 = 0
		while(isRed(image[y + lHeight][midLeft])):
			lHeight += 1
		while(isRed(image[y + rHeight][midRight])):
			rHeight += 1
		if(lHeight > rHeight):
			pHeight2 = rHeight
			pMid = xStart + math.floor(0.5 * pWidth + 0.5)
			pWidth2 = math.floor(1.5 * pHeight2 + 0.5)
			pMid2 = xFinish - math.floor(0.5 * pWidth2 + 0.5)
		elif(rHeight > lHeight):
			pHeight2 = lHeight
			pMid = xFinish - math.floor(0.5 * pWidth + 0.5)
			pWidth2 = math.floor(1.5 * pHeight2 + 0.5)
			pMid2 = xStart + math.floor(0.5 * pWidth2 + 0.5)
		else:
			pHeight = lHeight
			pHeight2 = rHeight
			pWidth = math.floor((xFinish - xStart) / 2)
			pWidth2 = pWidth
			pMid = xFinish - math.floor(0.5 * pWidth + 0.5)
			pMid2 = xStart + math.floor(0.5 * pWidth2 + 0.5)
		return {"midPoint":[pMid, pMid2], "bottomPoint":[y+pHeight, y+pHeight2]}

#Take the image
#Scan through every line
#How do I determine if the cylinder has begun?
#I want a solid line of reds. I want the last line to have some red
#

dirVector = Vector(1,0);


rotTheta = math.atan2(dirVector.y, dirVector.x)
rotCos = math.cos(rotTheta)
rotSin = math.sin(rotTheta)

img=mpimg.imread('testInputData/red_cylinders.png')

halfYPixel = img.shape[0]/2

indices = scanH2(img, halfYPixel, 0, img.shape[1])
print indices
barrels = []
for xIndices in indices:
	keyPoints = scanBody3(img, halfYPixel, xIndices[0], xIndices[1], radius, robotHeight)
	for i in range(len(keyPoints['bottomPoint'])):
		cHeight = (keyPoints['bottomPoint'][i] - float(img.shape[0]) / 2.0 ) * pixHeight
		rHeight = robotHeight
		rDist = cDist * rHeight / cHeight + radius#Forward distance to the barrel
		cLDist = (float(img.shape[1]) / 2.0 - keyPoints['midPoint'][i] ) * pixWidth
		rLDist = cLDist * rDist / cDist #Left distance to the barrel
		newBarrel = Point(rotCos * rDist - rotSin * rLDist, rotSin * rDist + rotCos * rLDist)
		barrels.append(newBarrel)

for barrel in barrels:
	print "(" + str(barrel.x) + "," + str(barrel.y) + ")"

################################################################################

# I'm going to relate expected height to perceived width
#

#radius = 1.5
#barrelHeight = 5.0
#robotHeight = 2.0
#barrelWidth = 2 * radius

#cDist = 5
#pixHeight = 0.002820972
#pixWidth = pixHeight

#cHeight = (bottomIndices[0] - float(img.shape[0]/2) ) * pixHeight
#rHeight = robotHeight
# rdist / rheight = cdist / cheight
# rdist = cdist * rheight / cheight
#rDist = radius + cDist * rHeight / cHeight

################################################################################

##COMMENTED OUT TO TEST A DIFFERENT WAY
#cylinderPoints = []
#
#yIndex = img.shape[0]/2
#for xIndices in indices:
#	obstaclePoints = scanBody2(img, halfYPixel,xIndices[0],xIndices[1])
#	print obstaclePoints
#	print ""
#	print ""
##


# D = (r * tan(asin(edgeDist / r)) + robotHeight) / tan(asin(edgeDist / r))
# D = (r * tan(asin(edgeDist / r)) + robotHeight - barrelHeight) / tan(asin(edgeDist / r))
#
#distance from the ground is 2
#height of barrel is 5
#radius is 1.5
