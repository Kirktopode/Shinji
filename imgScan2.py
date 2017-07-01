
# D = (r * tan(asin(edgeDist / r)) + robotHeight) / tan(asin(edgeDist / r))
# D = (r * tan(asin(edgeDist / r)) + robotHeight - barrelHeight) / tan(asin(edgeDist / r))
#
#
# image array is in format [y][x][rgb]

import matplotlib.image as mpimg
import matplotlib.pyplot as plt
from numpy import ndarray
import math


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

def scanBody2(image, y, xStart, xFinish):
	topCorners = [y, xStart, y, xFinish]
	leftCorner = []
	rightCorner = []
	leftBottom = []
	rightBottom = []
	middleBottom = []
	i = 1
	indices = scanH2(image, y+i, xStart, xFinish)
	lastIndices = indices
	while len(lastIndices) > 0:
		if len(indices) > 0:
			xStart = indices[0][0]
			xFinish = indices[-1][-1]
		lastLastIndices = lastIndices
		lastIndices = indices
		indices = scanH2(image, y+i, xStart, xFinish)
		#if it just started getting smaller, it's a corner
		#if it just splits, it's identical to finding a sidebottom
		#if it ends, we've found a bottompoint
		if len(indices) == 0:
			middleBottom.append([y+i,
			 (lastIndices[0][0]+lastIndices[-1][-1])/2])
			lastIndices = indices
		else:
			if lastIndices[0][0] == lastLastIndices[0][0] and indices[0][0] > lastIndices[0][0]:
				leftCorner.append([y+i,lastIndices[0][0]])
			elif lastIndices[0][0] > lastLastIndices[0][0] and indices[0][0] == lastIndices[0][0]:
				leftBottom.append([y+i,lastIndices[0][0]])
			if lastIndices[-1][-1] == lastLastIndices[-1][-1] and indices[-1][-1] < lastIndices[-1][-1]:
				rightCorner.append([y+i,lastIndices[-1][-1]])
			elif lastIndices[-1][-1] < lastLastIndices[-1][-1] and indices[-1][-1] == lastIndices[-1][-1]:
				rightBottom.append([y+i,lastIndices[-1][-1]])
		i += 1
	return {"topCorners":topCorners, "leftCorner":leftCorner, "rightCorner":rightCorner,
	 "leftBottom":leftBottom, "rightBottom":rightBottom, "middleBottom":middleBottom}

#
##scanRedBody begins at the upper left pixel of a red body and finds the lowest
#central point of the body. If multiple red bodies are connected, it finds the
#lowest central point of each connected body. These lowest points are returned
#in an array of two-member arrays.
#I may scan for width in order to predict the centerpoint of partially hidden
#cylinders.
#

def scanRedBody(yIndex, xIndex, image):
	if type(image) != ndarray:
		print "!--Error--> var 'image' in scanRedBody must be ndarray"
		return []
	if type(yIndex) != int:
		print "!--Error--> var 'yIndex' in scanRedBody must be int"
		return []
	if type(xIndex) != int:
		print "!--Error--> var 'xIndex' in scanRedBody must be int"
		return []
	bottomIndices = []
	lastDownStep = []
	edgePoints = []
	lastn = 0
	n = 0
	i = 0
	while (xIndex + i < image.shape[1]) and isRed(img[yIndex][xIndex + i]):
		if n != 0:
			lastn = n
		n = 0
		#print i
		while isRed(img[yIndex + n + 1][xIndex + i]):
			n += 1
		if(lastn == 0):
			if len(edgePoints) == 0: #if edgePoints is empty, add this
				edgePoints.append([[yIndex + n, xIndex]])
		elif (lastn == n - 1):  #if the pixels are slowly inching downward
			lastDownStep = [yIndex + n, xIndex + i]
		elif (lastn == n + 1): #if the pixels are going up
			if len(lastDownStep) > 0 and (lastDownStep[0] == lastn): #if we went down and are going up
				#we have found a bottompoint for a cylinder
				edgePoints[len(edgePoints)-1].append([lastDownStep[1],(xIndex + (i - 1) - lastDownStep[0])/2.0])
		elif (lastn < n) or (lastn > n): #bigger jump indicates a new cylinder
			edgePoints[len(edgePoints)-1].append([yIndex + lastn, xIndex + i - 1])
			edgePoints.append([[yIndex + n, xIndex + i]])
		i += 1
	print yIndex, n, xIndex, i
	edgePoints[len(edgePoints)-1].append([yIndex + n,xIndex + i - 1])

		#else: #if we don't have a bottompoint, but do have the right side of a cylinder
	return edgePoints



#Take the image
#Scan through every line
#How do I determine if the cylinder has begun?
#I want a solid line of reds. I want the last line to have some red
#

img=mpimg.imread('testInputData/red_cylinders.png')

halfYPixel = img.shape[0]/2

indices = scanH2(img, halfYPixel, 0, img.shape[1])
print indices

cylinderPoints = []

yIndex = img.shape[0]/2
for xIndices in indices:
	obstaclePoints = scanBody2(img, halfYPixel,xIndices[0],xIndices[1])
	print obstaclePoints
	print ""
	print ""



# D = (r * tan(asin(edgeDist / r)) + robotHeight) / tan(asin(edgeDist / r))
# D = (r * tan(asin(edgeDist / r)) + robotHeight - barrelHeight) / tan(asin(edgeDist / r))
#
#distance from the ground is 2
#height of barrel is 5
#radius is 1.5
################################################################################
radius = 1.5
barrelHeight = 5.0
robotHeight = 2.0

cDist = 5
pixHeight = 0.002820972
#cHeight = (bottomIndices[0] - float(img.shape[0]/2) ) * pixHeight
#fHeight = robotHeight
# fdist / fheight = cdist / cheight
# fdist = cdist * fheight / cheight
#fDist = radius + cDist * fHeight / cHeight
