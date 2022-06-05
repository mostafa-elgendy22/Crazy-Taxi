
# python script to generate entites 

entity = '''
             {
                 "name": "road",
                 "position": [0, -1, 0],
                 "rotation": [-90, 0, 0],
                 "scale": [10, 10, 1],
                 "components": [
                     {
                         "type": "Mesh Renderer",
                         "mesh": "plane",
                         "material": "road"
                     }
                 ]
             },
'''


name = "road"
x, y, z = 0, -1, 0
rx, ry, rz = -90, 0, 0
sx, sy, sz = 20, 20, 1
material = "road"

p = input("Enter the name of entity: (default: road) ")
if p != "":
    name = p

p = input("Enter position x y z: (default: 0 -1 0) ")
if p != "":
    x, y, z = [int(i) for i in p.split()]

p = input("Enter rotation x y z: (default: -90 0 0) ")
if p != "":
    rx, ry, rz = [int(i) for i in p.split()]

p = input("Enter scale x y z: (default: 20 20 1) ")
if p != "":
    sx, sy, sz = [int(i) for i in p.split()]

p = input("Enter material: (default: road) ")
if p != "":
    material = p

x1 = -200
x2 = 200
z1 = -200
z2 = 200

p = input("Enter min and max x and z: (default: -200 200 -200 200) ")
if p != "":
    x1, x2, z1, z2 = [int(x) for x in p.split()]


fil = open("gen/a.json", "w")
# cover from x1 to x2 with step of scale.x
for x in range(x1, x2, sx*2):
    for z in range(z1, z2, sy*2):
        fil.write(str('{"name":"'))
        fil.write(str(name))
        fil.write(str('","position":['))
        fil.write(str(x+sx/2))
        fil.write(str(','))
        fil.write(str(y))
        fil.write(str(','))
        fil.write(str(z+sy/2))
        fil.write(str('],"rotation":['))
        fil.write(str(rx))
        fil.write(str(','))
        fil.write(str(ry))
        fil.write(str(','))
        fil.write(str(rz))
        fil.write(str('],"scale":['))
        fil.write(str(sx))
        fil.write(str(','))
        fil.write(str(sy))
        fil.write(str(','))
        fil.write(str(sz))
        fil.write(str('],"components":[{"type":"Mesh Renderer","mesh":"plane","material":"'))
        fil.write(str(material))
        fil.write(str('"}]},'))
        
# mimick this:
entity = '''
             {
                 "name": "road",
                 "position": [0, -1, 0],
                 "rotation": [-90, 0, 0],
                 "scale": [10, 10, 1],
                 "components": [
                     {
                         "type": "Mesh Renderer",
                         "mesh": "plane",
                         "material": "road"
                     }
                 ]
             },
'''



