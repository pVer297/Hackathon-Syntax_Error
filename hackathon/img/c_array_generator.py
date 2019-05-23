import argparse
import os
from PIL import Image

parser = argparse.ArgumentParser()
parser.add_argument('path')
args = parser.parse_args()

image = Image.open(args.path)
print "const uint32_t IMAGE_WIDTH = %d;" % (image.width)
print "const uint32_t IMAGE_HEIGHT = %d;" % (image.height)
print "const uint32_t IMAGE_DATA[] = {"
pixels = list(image.convert('RGBA').getdata())
for r, g, b, a in pixels:
  print ' 0x{:02x}{:02x}{:02x}{:02x},'.format(a, r, g, b)
print "};"