import random
import numpy as np



# python script to generate buildings

entity = '''
             {
                 "name": "building",
                 "position": [0, -1, 0],
                 "rotation": [0, 0, 0],
                 "scale": [20, 20, 1],
                 "components": [
                     {
                         "type": "Mesh Renderer",
                         "mesh": "plane",
                         "material": "building"
                     }
                 ]
             },
'''


name = "building"
# heights
ys = [1.5, 10.6, 19.4]
rx, ry, rz = -90, 0, 0
sx, sy, sz = 20, 20, 1
material = "building"

print("SCRIPT FOR BUILDINGS:")

p = input("Enter name of the entity: (default: building) ")
if p != "":
    name = p

p = input("possible heights: (default: 1.5 10.6 19.4) ")
if p != "":
     ys = p.split()

p = input("Enter scale x y z: (default: 20 20 1) ")
if p != "":
    sx, sy, sz = [int(i) for i in p.split()]

p = input("Enter material: (default: building) ")
if p != "":
    material = p

x1 = -200
x2 = 200
z1 = -200
z2 = 200

p = input("Enter min and max x and z: (default: -200 200 -200 200) ")
if p != "":
    x1, x2, z1, z2 = [int(x) for x in p.split()]


fil = open("gen/b.json", "w")

# border of the game
# vertical
for x in [x1, x2]:
    for z in range(z1, z2 + 1, sy*2):
        y = random.choice(ys)
        fil.write(f'{{"name":"{name}","position":[{x},{y},{z+sy}],"rotation":[0,')
        # normal = x
        if x == x1:
            fil.write('90')
        # normal = -x
        else:
            fil.write('-90')
        fil.write(f',0],"scale":[{sx},{sy},{sz}],"components":[{{"type":"Mesh Renderer","mesh":"plane","material":"{material}"}}]}},')

# horizontal
for z in [z1, z2]:
    for x in range(x1, x2 + 1, sx*2):
        y = random.choice(ys)
        fil.write(f'{{"name":"{name}","position":[{x+sx},{y},{z}],"rotation":[0,')
        # normal = z
        if z == z1:
            fil.write('0')
        # normal = -z
        else:
            fil.write('180')
        fil.write(f',0],"scale":[{sx},{sy},{sz}],"components":[{{"type":"Mesh Renderer","mesh":"plane","material":"{material}"}}]}},')


# grid of buildings 
# world is -200 to 200 (400x400) in XZ plane
# buildings are 40x40
# can fit maximum of 100 buildings
# divide the world into a grid of 10x10
world = np.zeros((10, 10))
# no buildings in middle streets
# no buildings on the borders

world = [
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 5, 1, 5, 1, 1, 1, 5, 5, 1],
    [1, 5, 1, 1, 1, 1, 5, 5, 1, 1],
    [1, 5, 5, 5, 1, 1, 5, 5, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 5, 1, 5, 1, 1, 5, 5, 5, 1],
    [1, 5, 1, 5, 1, 1, 1, 1, 1, 1],
    [1, 5, 1, 5, 1, 1, 5, 5, 5, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
    ]

world = np.array(world)
# get indices where world == 5
builds = np.where(world == 5)

print("AAAAA")
print(len(builds))
# loop on the indices and put building on top of them
for i in range(len(builds[0])):
    xi = builds[0][i]
    zi = builds[1][i]

    # map to range -180 to 180 with step = 40
    x = xi * 40 - 200
    z = zi * 40 - 200
    y = random.choice(ys)

    print(x, y, z)
    # draw the 4 walls
    # front (normal = z)
    fil.write(f'{{"name":"{name}","position":[{x},{y},{z-sy}],"rotation":[0,0,0],"scale":[{sx},{sy},{sz}],"components":[{{"type":"Mesh Renderer","mesh":"plane","material":"{material}"}}]}},')
    # back (normal = -z)
    # if world[xi, zi+1] != 5:
    fil.write(f'{{"name":"{name}","position":[{x},{y},{z+sy}],"rotation":[0,180,0],"scale":[{sx},{sy},{sz}],"components":[{{"type":"Mesh Renderer","mesh":"plane","material":"{material}"}}]}},')
    # left (normal = x)
    fil.write(f'{{"name":"{name}","position":[{x-sx},{y},{z}],"rotation":[0,90,0],"scale":[{sx},{sy},{sz}],"components":[{{"type":"Mesh Renderer","mesh":"plane","material":"{material}"}}]}},')
    # right (normal = -x)
    # if world[xi-1, zi] != 5:
    fil.write(f'{{"name":"{name}","position":[{x+sx},{y},{z}],"rotation":[0,-90,0],"scale":[{sx},{sy},{sz}],"components":[{{"type":"Mesh Renderer","mesh":"plane","material":"{material}"}}]}},')



       
# mimick this:
entity = '''
             {
                 "name": "building",
                 "position": [0, -1, 0],
                 "rotation": [-90, 0, 0],
                 "scale": [10, 10, 1],
                 "components": [
                     {
                         "type": "Mesh Renderer",
                         "mesh": "plane",
                         "material": "building"
                     }
                 ]
             },
'''



