#!/usr/bin/env python
from Tkinter import Tk
from tkFileDialog import askopenfilename
from PIL import Image
import sys

import transmogripy

Tk().withdraw()
imageFileName = askopenfilename()

transmogripy.init()
transmogripy.named_file_to_named_file(imageFileName, 'out.png', 8)

im = Image.open('out.png')
im.show()
