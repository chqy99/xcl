import torch
from xcl import *

image = torch.randint(0, 4, (8, 8, 1), dtype=torch.uint8, device='cuda')
colors = torch.tensor([[0], [1]], dtype=torch.uint8, device='cuda')
print(image)

result = count_in_block(image, colors, 2, 2)
print(result.shape)
print(result)