52
# D = (r * tan(asin(edgeDist / r)) + robotHeight) / tan(asin(edgeDist / r))
# D = (r * tan(asin(edgeDist / r)) + robotHeight - barrelHeight) / tan(asin(edgeDist / r))
#
# image array is in format [y][x][rgb]
################################################################################

#Can dimensions - 4.83 inches high, 2.13 inch diameter at lid, 2.60 inch diameter at the middle

#Calibration data for webcam


radius = 3.302 #NOTE The larger radius of the can
robotHeight = 3.22899921 #NOTE This is the webcam's elevation
barrelHeight = 12.2682 #NOTE Can height
barrelWidth = 2 * radius

widthHeightRatio = barrelWidth / robotHeight
widthHeightRatio2 = barrelWidth / barrelHeight

cDist = 160.02
pixHeight = 0.0902222559
pixWidth = pixHeight

#Calibration data for POVRay


#radius = 1.5
#barrelHeight = 5.0
#robotHeight = 2.0
#barrelWidth = 2 * radius

#cDist = 5
#pixHeight = 0.002820972
#pixWidth = pixHeight


#cHeight = (bottomIndices[0] - float(img.shape[0]/2) ) * pixHeight
#fHeight = robotHeight
# fdist / fheight = cdist / cheight
# fdist = cdist * fheight / cheight
#fDist = radius + cDist * fHeight / cHeight

################################################################################

#import matplotlib.image as mpimg
#import matplotlib.pyplot as plt
from numpy import ndarray
#from scipy.misc import imread, imsave # Used for analysis
from numpy import zeros # Used for analysis
import math
import numpy as np
import sys
from jvectors import Vector
#from scipy import ndimage as ndi

#from skimage import feature

import cv2


def isRed(pixel): ### FIXME change back from green to red
	if type(pixel) != ndarray:
		print "!--Error--> wrong type passed to isRed"
		return
	if len(pixel) != 3:
		print "!--Error--> wrong length passed to isRed"
		return
	#print pixel[0],pixel[1],pixel[2]
	#NOTE: Canny pixels are patterned as [B,G,R]
	if (pixel[1] > 1.1*pixel[0]) and (pixel[1] > 1.1*pixel[2]) and (pixel[1] > 50):
		return True
	else:
		return False

#Scan a select portion of a horizontal line, returning an array of index pairs.
#The first of each pair is where a red body begins, the second is where it ends.
#
#This version uses edges. An opening edge is only counted if there is red on
#the right. A closing edge is only counted if there is red on the left.
#
#The beginning and end of red is always counted.

def isEdge(pixel):
	return pixel == 1

def getEdgeHeight(edges, yStart, xStart):
	height = 1
	x = xStart
	y = yStart
	while y < edges.shape[0] and (isEdge(edges[y+1][x]) or isEdge(edges[y+1][x+1]) or isEdge(edges[y+1][x-1])):
		y += 1
		if isEdge(edges[y][x]):
			x = x
		elif isEdge(edges[y][x-1]):
			x -= 1
		elif isEdge(edges[y][x+1]):
			x += 1
	height += yStart - y
	while y >= 0 and (isEdge(edges[y-1][x]) or isEdge(edges[y-1][x+1]) or isEdge(edges[y-1][x-1])):
		y -= 1
		if isEdge(edges[y][x]):
			x = x
		elif isEdge(edges[y][x-1]):
			x -= 1
		elif isEdge(edges[y][x+1]):
			x += 1
	height += y - yStart
	return height

def scanH3(image, edges, y, xStart, xFinish):
	if type(image) != ndarray:
		print "!--Error--> var 'image' in scanHorizontalLineForRed must be ndarray"
		return []
	pixelIndices=[]
	for x in range(xStart, xFinish):
		if isRed(image[y][x]):
			if x == xStart or not isRed(image[y][x-1]):
				print "Found beginning index at " + str(x)
				pixelIndices.append([x])
			elif x == image.shape[1] - 1 or not isRed(image[y][x+1]):
				print "Found ending index at " + str(x)
				pixelIndices[-1].append(x+1)
			elif isRed(image[y][x-1]) and isRed(image[y][x]) and isRed(image[y][x+1]) and isEdge(edges[y][x]):
				print "Found edge index at " + str(x)
				pixelIndices[-1].append(x)
				pixelIndices.append([x])
	return pixelIndices

#Scan a select portion of a horizontal line, returning an array of index pairs.
#The first of each pair is where red begins, the second is where it ends.

def scanH2(image, y, xStart, xFinish):
	if type(image) != ndarray:
		print "!--Error--> var 'image' in scanHorizontalLineForRed must be ndarray"
		return []
	pixelIndices=[]
	for x in range(xStart, xFinish):
		if isRed(image[y][x]):
			if x == xStart or not isRed(image[y][x-1]):
				print "Found beginning index at " + str(x)
				pixelIndices.append([x])
			if x == image.shape[1] - 1 or not isRed(image[y][x+1]):
				print "Found ending index at " + str(x)
				pixelIndices[-1].append(x+1)
	return pixelIndices

##
#Gets the height in pixels from a defined x and y.
#Counts the number of red pixels until the pixels are no longer red.
#If it encounters a blue area, it judges whether the blue area is taller than
#half of the red area. If not, the blue area is treated as part of the red body
#and the measurement continues.

def getPHeight(image, y, x):
	pHeight = 0
	nonRedHeight = 1
	while nonRedHeight > 0:
		while isRed(image[y + pHeight][x]):
			pHeight += 1
		while y + pHeight + nonRedHeight < image.shape[0] and not isRed(image[y + pHeight + nonRedHeight][x]):
			nonRedHeight += 1
		if nonRedHeight < pHeight / 2:
			pHeight += nonRedHeight + 1
		else:
			nonRedHeight = 0
	return pHeight

##
#Gets the height in pixels from a defined x and y.
#Counts the number of red pixels until the pixels are no longer red.
#If it encounters a blue area, it judges whether the blue area is taller than
#half of the red area. If not, the blue area is treated as part of the red body
#and the measurement continues.
#
#Then does the same thing going up, adding the two numbers together.

def getPHeight2(image, y, x):
	pHeight = 0
	nonRedHeight = 1
	while nonRedHeight > 0:
		while isRed(image[y + pHeight][x]):
			pHeight += 1
		while y + pHeight + nonRedHeight < image.shape[0] and not isRed(image[y + pHeight + nonRedHeight][x]):
			nonRedHeight += 1
		if nonRedHeight < pHeight / 2:
			pHeight += nonRedHeight + 1
		else:
			nonRedHeight = 0
	pHeightUp = 0
	nonRedHeight = 1
	while nonRedHeight > 0:
		while isRed(image[y - pHeightUp][x]):
			pHeightUp += 1
		while y - (pHeightUp + nonRedHeight) > 0 and not isRed(image[y - (pHeightUp + nonRedHeight)][x]):
			nonRedHeight += 1
		if nonRedHeight < pHeightUp / 2:
			pHeightUp += nonRedHeight + 1
		else:
			nonRedHeight = 0
	pHeight += pHeightUp
	return pHeight

##
#Gets distance between the robot and the nearest point on the cylinder,
#taking pHeight as the number of pixels between the central x-axis of the
#image and the point where the cylinder's red ends. Returns distance
#as the distance to the perceived center of the cylinder, meaning it adds
#one radius to the computed distance to the near pixel.

def getRDist(pHeight):
	cHeight = pHeight * pixHeight
	rHeight = robotHeight
	rDist = cDist * rHeight / cHeight + radius #Forward distance to the barrel
	return rDist

##
#Translates RDist into a pHeight. This is useful for the newest and improvedest
#scanBody function.

def pHeightFromRDist(rDist):
	rDist -= radius
	rHeight = robotHeight
	cHeight = cDist * rHeight / rDist
	pHeight = int(round(cHeight / pixHeight))
	return pHeight

##
#Estimates the pixel radius of a red body based upon its pixel height.
#Assumes that pHeight is an accurate representation of distance to the
#cylinder; as such, it is best to use on the outer edge, lest it give you
#a more inaccurate calculation

def getPRad(pHeight):
	rHeight = robotHeight
	pRad = int(round(radius * pHeight / rHeight))
	return pRad

##
#Gets the distance left from the central axis of the camera's vision to the
#center of the cylinder

def getRLDist(pLDist, rDist):
	cLDist = (float(img.shape[1]) / 2.0 - keyPoints['midPoint'][i] ) * pixWidth
	rLDist = cLDist * rDist / cDist #Left distance to the barrel
	return rLDist

##
#Uses a rotation matrix to rotate a point. Requires theta, sin, and cos because
#trigonometry is expensive for computer resources.

def rotateVector(point, theta, sin, cos):
	return Vector(cos * point.x - sin * point.y,
				 sin * point.x + cos * point.y)

##
#Newest scan function
#Assume each red body to be one can
#Take the entire pHeight of the red body
#Judge based upon this and the ratio of height to width
#whether this cylinder is clipped behind another or not.

def scanBody4(image, y, xStart, xFinish, radius, roHeight):
	midPoint = (xFinish + xStart) / 2
	pHeight = getPHeight2(image, y, midPoint)
	print "Cylinder center is at " + str(midPoint) + ", with height " + str(pHeight)
	pWidth = xFinish - xStart
	print "Left center x:", xStart, "Right center x:", xFinish, "y:", y
	print "Width:", pWidth
	cWidth = pixWidth * pWidth
	cHeight = pixHeight * pHeight
	# if cWidth is not within 10% of 3/2 of cHeight, then I need to look for
	# other points depending on the height I do find
	if(cWidth >= widthHeightRatio2*cHeight):
		#I have one cylinder
		print "non-hidden cylinder found"
		return {"midPoint":midPoint, "height":pHeight}
	else: #I assume there is one and it is partially hidden.
		print "hidden cylinder calculated"
		#test two heights, take the greater as nearer the centerside
		midPoint1 = (xStart + midPoint) / 2
		midPoint2 = (midPoint + xFinish) / 2
		pHeight1 = getPHeight2(image, y, midPoint1)
		pHeight2 = getPHeight2(image, y, midPoint2)
		if(pHeight1 > pHeight):
		  pHeight = pHeight1
		  print "Center is to the left"
		elif(pHeight2 > pHeight):
		  pHeight = pHeight2
		  print "Center is to the right"
		else:
		  print "Center is in the middle"
		if(pHeight1 > pHeight2):
			pWidth = int(math.floor(widthHeightRatio * pHeight + radius))
			midPoint = xFinish - pWidth/2
		elif(pHeight2 > pHeight1):
			pWidth = int(math.floor(widthHeightRatio * pHeight + radius))
			midPoint = xStart + pWidth/2
		print "Estimated pixel width of the cylinder is " + str(pWidth)
		if(midPoint < 0 or midPoint > image.shape[1]):
			print "Estimated midPoint is at " + str(midPoint) + ", " + str(y + pHeight)
			return {"midPoint":midPoint,"height":pHeight}
		else:
			pHeight = getPHeight2(image, y, midPoint)
			print "Estimated midPoint is at " + str(midPoint) + ", " + str(y + pHeight)
			return {"midPoint":midPoint,"height":pHeight}

##
#Calculates the coordinates of a cylinder based upon its midpoint in the image,
#its height, and its

def calculateCoords(img, midPoint, pHeight, rotCos, rotSin):
	cHeight = pHeight * pixHeight
	rHeight = barrelHeight
	rDist = cDist * rHeight / cHeight + radius#Forward distance to the barrel
	cLDist = (float(img.shape[1]) / 2.0 - midPoint ) * pixWidth
	rLDist = cLDist * rDist / cDist #Left distance to the barrel
	return Vector(origin.x + rotCos * rDist - rotSin * rLDist,
	             origin.y + rotSin * rDist + rotCos * rLDist)
##
#New scan function
#Scan vertically from the middle
#Compare the barrel's radius and robot's height to the measured ratio
#Depending on how off this is, scan other portions of the red body
#Expect no more than one hidden barrel.

def scanBody3(image, y, xStart, xFinish, radius, roHeight):
	midPoint = (xFinish + xStart) / 2
	pHeight = getPHeight(image, y, midPoint)
	print "Bottom middle is " + str(midPoint) + ", " + str(y + pHeight)
	pWidth = xFinish - xStart
	cWidth = pixWidth * pWidth
	cHeight = pixHeight * pHeight
	# if cWidth is not within 10% of 3/2 of cHeight, then I need to look for
	# other points depending on the height I do find
	if(abs(cWidth-1.5*cHeight) <= .1*1.5*cHeight):
		#I have one cylinder
		print "Only one cylinder found"
		return {"midPoint":[midPoint], "bottomPoint":[y+pHeight]}
	elif(cWidth < 1.5 * cHeight): #I assume there is one and it is partially hidden.
		print "One hidden cylinder calculated"
		#test two heights, take the greater as nearer the centerside
		midPoint1 = (xStart + midPoint) / 2
		midPoint2 = (midPoint + xFinish) / 2
		pHeight1 = getPHeight(image, y, midPoint1)
		pHeight2 = getPHeight(image, y, midPoint2)
		if(pHeight1 > pHeight):
		  pHeight = pHeight1
		  print "Center is to the left"
		if(pHeight2 > pHeight):
		  pHeight = pHeight2
		  print "Center is to the right"
		if(pHeight1 > pHeight2):
			pWidth = int(math.floor(1.5 * pHeight + 0.5))
			midPoint = xFinish - pWidth/2
		elif(pHeight2 > pHeight1):
			pWidth = int(math.floor(1.5 * pHeight + 0.5))
			midPoint = xStart + pWidth/2
		print "Estimated pixel width of the cylinder is " + str(pWidth)
		if(midPoint < 0 or midPoint > image.shape[1]):
			print "Estimated midPoint is at " + str(midPoint) + ", " + str(y + pHeight)
			return {"midPoint":[midPoint],"bottomPoint":[y+pHeight]}
		else:
			pHeight = getPHeight(image, y, midPoint)
			print "Estimated midPoint is at " + str(midPoint) + ", " + str(y + pHeight)
			return {"midPoint":[midPoint],"bottomPoint":[y+pHeight]}

	elif(cWidth > 1.5 * cHeight):
		#I assume there are two.
		print "Two cylinders calculated"
		pHeight = getPHeight(image, y, (xFinish - xStart) / 2)
		pWidth = math.floor(1.5 * pHeight + 0.5)
		midLeft = int(math.floor(((xFinish - pWidth) + xStart)/2))
		midRight = int(math.floor(((xStart + pWidth) + xFinish)/2))
		lHeight = getPHeight(image, y, midLeft)
		rHeight = getPHeight(image, y, midRight)
		pHeight2 = 0
		print "Left searched midPoint at " + str(midLeft) + ", " + str(y + lHeight)
		print "Right searched midPoint at " + str(midRight) + ", " + str(y + rHeight)
		if(lHeight > rHeight):
			pHeight2 = rHeight
			pMid = xStart + math.floor(0.5 * pWidth + 0.5)
			pWidth2 = math.floor(1.5 * pHeight2 + 0.5)
			pMid2 = xFinish - math.floor(0.5 * pWidth2 + 0.5)
			print "Left is forward"
			print "Left midPoint is at " + str(pMid) + ", " + str(y+pHeight)
			print "Right midPoint is at " + str(pMid2) + ", " + str(y+pHeight2)
		elif(rHeight > lHeight):
			pHeight2 = lHeight
			pMid = xFinish - math.floor(0.5 * pWidth + 0.5)
			pWidth2 = math.floor(1.5 * pHeight2 + 0.5)
			pMid2 = xStart + math.floor(0.5 * pWidth2 + 0.5)
			print "Right is forward"
			print "Left midPoint is at " + str(pMid2) + ", " + str(y+pHeight2)
			print "Right midPoint is at " + str(pMid) + ", " + str(y+pHeight)
		else:
			pHeight = lHeight
			pHeight2 = rHeight
			pWidth = math.floor((xFinish - xStart) / 2)
			pWidth2 = pWidth
			pMid = xFinish - math.floor(0.5 * pWidth + 0.5)
			pMid2 = xStart + math.floor(0.5 * pWidth2 + 0.5)
			print "Cylinders are at equal distance"
			print "Left midPoint is at " + str(pMid) + ", " + str(y+pHeight)
			print "Right midPoint is at " + str(pMid2) + ", " + str(y+pHeight2)
		return {"midPoint":[pMid, pMid2], "bottomPoint":[y+pHeight, y+pHeight2]}

##
#Translates a compass heading into a heading based on a coordinate plane

def compHeadToCoordHead(heading):
	heading -= 90.0
	if(heading < 0.0): heading += 360
	heading -= (heading - 180) * 2
	return heading

#Take the image
#Scan through every line
#How do I determine if the cylinder has begun?
#I want a solid line of reds. I want the last line to have some red
#

if len(sys.argv) < 2:
	print "USAGE: python imgScan2.py [png file] [heading] [origin x] [origin y]"
	print "heading defaults to 90 (east)"
	print "origin defaults to 0 0"
	exit()
else:
	imgFile = sys.argv[1]
	if not imgFile.endswith(".png"):
		print "USAGE: python imgScan2.py [png file] [heading] [origin x] [origin y]"
		print "heading defaults to 90 (east)"
		print "origin defaults to 0 0"
		exit()

if len(sys.argv) < 3:
	heading = 90
else:
	heading = float(sys.argv[2])

if len(sys.argv) < 5:
	origin = Vector(0,0)
else:
	origin = Vector(float(sys.argv[3]), float(sys.argv[4]))

print "Calculating trigonometry for rotation matrix"

rotTheta = compHeadToCoordHead(heading) * math.pi / 180.0
rotCos = math.cos(rotTheta)
rotSin = math.sin(rotTheta)

print "rotTheta = " + str(rotTheta)
print "rotCos = " + str(rotCos)
print "rotSin = " + str(rotSin)

img=cv2.imread(imgFile, 1)
edges = cv2.Canny(img, 50, 150)

print "img shape: ", img.shape
print "edges shape: ", edges.shape
print "Edges at 500,500", edges[500][500]

halfYPixel = img.shape[0]/2

indices = scanH3(img, edges, halfYPixel, 0, img.shape[1])
print "Indices:", indices
print "halfYPixel:", halfYPixel
barrels = []
for index, xIndices in enumerate(indices):
	print "Scanning body " + str(index)
	keyPoints = scanBody4(img, halfYPixel, xIndices[0], xIndices[1], radius, robotHeight)
	coords = calculateCoords(img, keyPoints['midPoint'], keyPoints['height'], rotCos, rotSin)
	barrels.append(coords)
barrelPairs = []

for index, barrel in enumerate(barrels):
	print "Barrel " + str(index) + ": (" + str(barrel.x) + "," + str(barrel.y) + ")"

#Create a list of 'barrel pairs' and the distance between them

for i in range(len(barrels)):
	for j in range(i + 1, len(barrels)):
		barrelPairs.append([barrels[i].dist(barrels[j]), barrels[i], barrels[j]])

print "BarrelPairs:"

for index, barrelPair in enumerate(barrelPairs):
	print index,":", barrelPair[0], "(",barrelPair[1].x, barrelPair[1].y,")","(",barrelPair[2].x,barrelPair[2].y,")"
	midPoint = barrelPair[1].diff(barrelPair[2]).div(2).add(barrelPair[1])
	barrelPair.append(midPoint)
	print "Midpoint:", midPoint.x, midPoint.y

print ""

#Sort list by distance between barrels in descending order

for i in range(len(barrelPairs)):
	maxDistIndex = i
	for j in range(i + 1, len(barrelPairs)):
		if barrelPairs[j][0] > barrelPairs[maxDistIndex][0]:
			maxDistIndex = j
	temp = barrelPairs[maxDistIndex]
	barrelPairs[maxDistIndex] = barrelPairs[i]
	barrelPairs[i] = temp

print "Sorted BarrelPairs:"

for index, barrelPair in enumerate(barrelPairs):
	print index,":", barrelPair[0], "(",barrelPair[1].x, barrelPair[1].y,")","(",barrelPair[2].x,barrelPair[2].y,")"

print ""

#For each item, test if the path is clear between them.
#

for barrelPair in barrelPairs:
	v1 = barrelPair[2].diff(barrelPair[1])
	for barrel in barrels:
		if barrel != barrelPair[1] and barrel != barrelPair[2]:
			v2 = barrel.diff(barrelPair[1])
			print v1.dot(v2)/(v1.magnitude * v2.magnitude),
			print v1.dist(v2),
			print barrelPair[0]

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
