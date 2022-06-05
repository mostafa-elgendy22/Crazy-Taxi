import random
import numpy as np


# python script to generate passengers

entity = '''
           {
                "position": [30, 0.81, -40],
                "scale":[0.02, 0.02, 0.02],
                "components": [
                    {
                        "type": "Mesh Renderer",
                        "mesh": "man2",
                        "material": "man2"
                    },
                    {
                        "type": "Passenger",
                        "destination": [-30, 0, -40],
                        "speed": 2,
                        "steps": 500,
                        "moveProb": 500    // the probability of moving = 1/moveProb
                    }
                ]
            },
'''


# heights
y = 0.81 
s = 0.02
material = "man2"

typ = "Passenger"
# speed -> 2 to 5
# steps -> 300 to 500
# moveProb -> 50 to 700

print("------------------\n-----------\n-----------\n")

x1 = -200
x2 = 200
z1 = -200
z2 = 200

p = input("Enter min and max x and z: (default: -200 200 -200 200) ")
if p != "":
    x1, x2, z1, z2 = [int(x) for x in p.split()]


fil = open("gen/c.json", "w")


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
# get indices where world == 1
strs = np.where(world == 1)

# put people on random streets
num = 50
p = input("Enter number of people (default: 50): ")
if p != "":
    num = int(p)

for _ in range(num):
    # get random street
    st = random.randint(0, len(strs[0])-1)
    xi = strs[0][st]
    zi = strs[1][st]

    # from -180 to 180 with step = 40
    xp = x1 + (xi * 40)
    zp = z1 + (zi * 40)

    # get random place on street
    x = xp + random.randint(-20, 20)
    z = zp + random.randint(-20, 20)

    # another random street for destination
    st = random.randint(0, len(strs[0])-1)
    xdi = strs[0][st]
    zdi = strs[1][st]

    # from -180 to 180 with step = 40
    xd = x1 + (xdi * 40)
    zd = z1 + (zdi * 40)

    fil.write(f'{{"position":[{x},{y},{z}],"scale":[{s},{s},{s}],"components":[{{"type":"Mesh Renderer","mesh":"man2","material":"{material}"}}')
    fil.write(f',{{"type":"{typ}","destination":[{xd},{0},{zd}],"speed":{random.uniform(2,5)},"steps":{random.randint(300,500)},"moveProb":{random.randint(50,700)}}}]}},')



       
# mimick this:
entity = '''
           {
                "position": [30, 0.81, -40],
                "scale":[0.02, 0.02, 0.02],
                "components": [
                    {
                        "type": "Mesh Renderer",
                        "mesh": "man2",
                        "material": "man2"
                    },
                    {
                        "type": "Passenger",
                        "destination": [-30, 0, -40],
                        "speed": 2,
                        "steps": 500,
                        "moveProb": 500    // the probability of moving = 1/moveProb
                    }
                ]
            },
'''
