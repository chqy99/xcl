import torch
from xcl import *

image = torch.randint(0, 4, (1, 4, 4, 1), dtype=torch.uint8, device='cuda')
hispath = [[Point(0, 0)], [Point(5, 5)]]
outpath = [[Point(0, 0)]]

outpath = path_plan(image, hispath, [0], [1], ConnectedMode.FOUR_CONNECT)
print(outpath)