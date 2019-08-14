from cpp.Polygonizer import polygonize
import numpy as np

def pr(mesh):
    print(f'max indices:{max(mesh[2])}')

nDiv=30
nFunc=0
vertices, normals, indices = polygonize(nDiv=nDiv, nFunc=nFunc)
mesh = polygonize(nDiv=nDiv, nFunc=nFunc)

print(vertices, normals, indices, sep='\n\n')
print(f'sizes vert:{vertices.shape[0]}, normls:{normals.shape[0]}, indices: {indices.size}')
print(f'indices, max:{np.max(indices)}, min: {np.min(indices)}')

print('traverse...', end='')
for i in indices:
    v=vertices[i]
    n=normals[i]
print('ok!')