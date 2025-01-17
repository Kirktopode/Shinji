import cv2
import numpy
from numpy import ndarray
from numpy import zeros
import sys

COLOR = "R"

def isRed(pixel): ### FIXME change back from green to red
	if type(pixel) != ndarray:
		if voberse: print ("!--Error--> wrong type passed to isRed")
		return
	if len(pixel) != 3:
		if voberse: print ("!--Error--> wrong length passed to isRed")
		return
	#print pixel[0],pixel[1],pixel[2]
	#NOTE: Canny pixels are patterned as [B,G,R]
	if COLOR == "G":
		if (pixel[1] > 1.1*pixel[0]) and (pixel[1] > 1.1*pixel[2]) and (pixel[1] > 50):
			return True
		else:
			return False
	elif COLOR == "R":
		if (pixel[2] > 2*pixel[0]) and (pixel[2] > 2*pixel[1]) and (pixel[2] > 50):
			return True
		else:
			return False
	elif COLOR == "B":
		if (pixel[0] > 1.1*pixel[1]) and (pixel[0] > 1.1*pixel[2]) and (pixel[0] > 50):
			return True
		else:
			return False

def paintImage(image, filename):
	imgPainted = zeros([len(image), len(image[0]), 3])
	h = len(imgPainted)
	w = len(imgPainted[0])

	for y in range(len(image)):
		for x in range(len(image[y])):
			if(isRed(image[y][x])):
				imgPainted[y][x] = [0, 0, 255]
			else:
				imgPainted[y][x] = [255, 0, 0]
	pathList = filename.split('/')
	newPath = ""
	for i in range(len(pathList)):
		if i == len(pathList) - 1:
			newPath += "painted_"
		newPath += pathList[i]
		if(i < len(pathList) - 1):
			newPath += "/"
	cv2.imwrite(newPath, imgPainted)

def edgeFind(image, filename):
	imgEdges1 = cv2.Canny(image, 50, 150)
	imgEdges2 = cv2.Canny(image, 100, 200)

	pathList = filename.split('/')
	newPath = ""
	for i in range(len(pathList)):
		if i == len(pathList) - 1:
			newPath2 = newPath + "edgesOf_2_" + pathList[i]
			newPath1 = newPath + "edgesOf_1_" + pathList[i]
		else:
			newPath += pathList[i]
			newPath += "/"
	cv2.imwrite(newPath1, imgEdges1)
	cv2.imwrite(newPath2, imgEdges2)

if len(sys.argv) < 2:
	print "USAGE: python imRedAndEdges.py [png or jpg file]"
	exit()
else:
	imgFile = sys.argv[1]
	if not imgFile.endswith(".png") and not imgFile.endswith(".jpg"):
		print "USAGE: python imRedAndEdges.py [png or jpg file]"
		exit()

img = cv2.imread(imgFile, 1)

print "Creating copy of " + imgFile + " with all red highlighted."
paintImage(img, imgFile)
print "Creating two copies of " + imgFile + " using Canny edge detection."
edgeFind(img, imgFile)
