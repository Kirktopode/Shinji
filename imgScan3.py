
# D = (r * tan(asin(edgeDist / r)) + robotHeight) / tan(asin(edgeDist / r))
# D = (r * tan(asin(edgeDist / r)) + robotHeight - barrelHeight) / tan(asin(edgeDist / r))
#
# image array is in format [y][x][rgb]
################################################################################

import sys
import math

#Can dimensions - 4.83 inches high, 2.13 inch diameter at lid, 2.60 inch diameter at the middle

#Calibration data for webcam


radius = 3.302 # big radius
#robotHeight =  FIXME assign robotHeight
barrelHeight = 12.2682
barrelWidth = 2 * radius

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

import matplotlib.image as mpimg
import matplotlib.pyplot as plt
from numpy import ndarray
from scipy.misc import imread, imsave # Used for analysis
from numpy import zeros # Used for analysis
import math
import numpy as np
from scipy import ndimage as ndi

from skimage import feature

class Vector:
	def __init__(self, x, y):
		self.x = x
		self.y = y
class Point:
	def __init__(self, x, y):
		self.x = x
		self.y = y


def isRed(pixel): ### FIXME change back from green to red
	if type(pixel) != ndarray:
		print "!--Error--> wrong type passed to isRed"
		return
	if len(pixel) != 3:
		print "!--Error--> wrong length passed to isRed"
		return
	#print pixel[0],pixel[1],pixel[2]
	if (pixel[1] > 1.1*pixel[0]) and (pixel[1] > 1.1*pixel[2]) and (pixel[1] > 0.1):
		return True
	else:
		return False

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

def paintImage(image, filename):
	imgPainted = zeros([len(image), len(image[0]), 3])
	h = len(imgPainted)
	w = len(imgPainted[0])

	for y in range(len(image)):
		for x in range(len(image[y])):
			if(isRed(image[y][x])):
				imgPainted[y][x] = [255, 0, 0]
			else:
				imgPainted[y][x] = [0, 0, 255]
	pathList = filename.split('/')
	newPath = ""
	for i in range(len(pathList)):
		if i == len(pathList) - 1:
			newPath += "painted_"
		newPath += pathList[i]
		if(i < len(pathList) - 1):
			newPath += "/"
	imsave(newPath, imgPainted)

def edgeFind(image, filename):
	imgEdges1 = feature.canny(image)
	imgEdges2 = feature.canny(image)

	pathList = filename.split('/')
	newPath = ""
	for i in range(len(pathList)):
		if i == len(pathList) - 1:
			newPath2 = newPath + "edgesOf_2_" + pathList[i]
			newPath1 = newPath + "edgesOf_1_" + pathList[i]
		else:
			newPath += pathList[i]
			newPath += "/"
	imsave(newPath1, imgEdges1)
	imsave(newPath2, imgEdges2)


##
#Gets the height in pixels from a defined x and y.
#Counts the number of red pixels until the pixels are no longer red.

def getPHeight(image, y, x):
	pHeight = 0
	while(isRed(image[y+pHeight][x])):
		pHeight += 1
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

def rotatePoint(point, theta, sin, cos):
	return Point(cos * point.x - sin * point.y,
				 sin * point.x + cos * point.y)

##
#Newest scan function
#Take pHeight ten pixels from the left - maybe less, it depends
#Take pHeight ten pixels from the right - this improves ability to see hidden pieces
#First, compare height with width of the body
#If it's too tall for its width, there is clipping --
#	Shorter side is the outer edge, compute midpoint from there.
#If it's not tall enough, there are multiple barrels --
#	Take left edge and calculate a midPoint
#	Search for a difference in height here? no
#

def scanBody4(image, y, xStart, xFinish, radius, roHeight):
	leftPoint = xStart + 3
	rightPoint = xFinish - 3
	leftPHeight = getPHeight(image, y, leftPoint)
	print "Bottom left is " + str(leftPoint) + ", " + str(y + leftPHeight)
	rightPHeight = getPHeight(image, y, rightPoint)
	print "Bottom right is " + str(rightPoint) + ", " + str(y + rightPHeight)
	pHeight = leftPHeight if leftPHeight < rightPHeight else rightPHeight
	pWidth = xFinish - xStart
	cWidth = pixWidth * pWidth
	cHeight = pixHeight * pHeight
	targetWHRatio = 2 * radius / robotHeight
	print "pHeight:", pHeight, "cHeight:", cHeight, "cWidth:", cWidth
	# if cWidth is not within 10% of 3/2 of cHeight, then I need to look for
	# other points depending on the height I do find
	if(abs(cWidth - targetWHRatio * cHeight) <= .1 * targetWHRatio * cHeight):
		#I have one cylinder
		print "Only one cylinder found"
		midPoint = (xStart + xFinish) / 2
		pHeight = getPHeight(image, y, midPoint)
		return {"midPoint":[midPoint], "bottomPoint":[y+pHeight]}
	elif(cWidth < targetWHRatio * cHeight): #I assume there is one and it is partially hidden.
		print "One hidden cylinder calculated"
		#test two heights, take the greater as nearer the centerside
		pHeight = leftPHeight if leftPHeight > rightPHeight else rightPHeight
		pRad = getPRad(pHeight)
		if(pHeight == leftPHeight):
		  print "Center is to the left"
		if(pHeight == rightPHeight):
		  print "Center is to the right"
		midPoint = (xFinish - pRad) if leftPHeight > rightPHeight else (xStart + pRad)
		print "Estimated pixel width of the cylinder is " + str(pRad * 2)
		if(midPoint < 0 or midPoint > image.shape[1]):
			print "Estimated midPoint is at " + str(midPoint) + ", " + str(y + pHeight)
			return {"midPoint":[midPoint],"bottomPoint":[y+pHeight]}
		else:
			pHeight = getPHeight(image, y, midPoint)
			print "Estimated midPoint is at " + str(midPoint) + ", " + str(y + pHeight)
			return {"midPoint":[midPoint],"bottomPoint":[y+pHeight]}

	elif(cWidth > targetWHRatio * cHeight):
		#I assume there are multiple cylinders.
		print "Two cylinders calculated"
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
	origin = Point(0,0)
else:
	origin = Point(float(sys.argv[3]), float(sys.argv[4]))

print "Calculating trigonometry for rotation matrix"

rotTheta = compHeadToCoordHead(heading) * math.pi / 180.0
rotCos = math.cos(rotTheta)
rotSin = math.sin(rotTheta)

print "rotTheta = " + str(rotTheta)
print "rotCos = " + str(rotCos)
print "rotSin = " + str(rotSin)

img=mpimg.imread(imgFile)

paintImage(img, imgFile)
edgeFind(img, imgFile)

### FIXME remove this abominable multi-line quote
"""

print "img shape: ", img.shape
halfYPixel = img.shape[0]/2

indices = scanH2(img, halfYPixel, 0, img.shape[1])
print "Indices: ", indices
barrels = []
for index, xIndices in enumerate(indices):
	print "Scanning body " + str(index)
	keyPoints = scanBody4(img, halfYPixel, xIndices[0], xIndices[1], radius, robotHeight)
	for i in range(len(keyPoints['bottomPoint'])):
		cHeight = (keyPoints['bottomPoint'][i] - float(img.shape[0]) / 2.0 ) * pixHeight
		rHeight = robotHeight
		rDist = cDist * rHeight / cHeight + radius#Forward distance to the barrel
		cLDist = (float(img.shape[1]) / 2.0 - keyPoints['midPoint'][i] ) * pixWidth
		rLDist = cLDist * rDist / cDist #Left distance to the barrel
		newBarrel = Point(origin.x + rotCos * rDist - rotSin * rLDist,
		                  origin.y + rotSin * rDist + rotCos * rLDist)
		barrels.append(newBarrel)

for index, barrel in enumerate(barrels):
	print "Barrel " + str(index) + ": (" + str(barrel.x) + "," + str(barrel.y) + ")"

for
""" ### FIXME remove this abominable multi-line quote
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
