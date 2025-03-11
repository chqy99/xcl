import sys
sys.path.append("/chenqiyang/my_script/xcl/build/lib/")
import torch
import xcl

image = torch.randint(0, 4, (1, 4, 4, 1), dtype=torch.uint8, device='cuda')
hispath = [[xcl.Point(0, 0)], [xcl.Point(5, 5)]]
outpath = [[xcl.Point(0, 0)]]

outpath = xcl.path_plan(image, hispath, [0], [1], xcl.ConnectedMode.FOUR_CONNECT)
print(outpath)