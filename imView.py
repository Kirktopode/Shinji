import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import numpy
import sys

if len(sys.argv) < 2 or (not sys.argv[1].endswith(".png") and not sys.argv[1].endswith(".jpg") ):
    print "Please select a .png or .jpg file to view."
    exit()

imgfile = sys.argv[1]

image = mpimg.imread(imgfile)

plt.imshow(image)
plt.show()
