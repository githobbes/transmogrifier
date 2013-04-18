#!/usr/bin/env python
import sys

import transmogripy

transmogripy.init()

transmogripy.named_file_to_named_file('../testData/penrose-young.png', 'out.png', 8)
