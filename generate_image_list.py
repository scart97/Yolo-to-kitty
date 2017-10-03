import os

file = open("imagelist.txt", "w")
filenames = os.listdir("Images")

for name in filenames:
	file.write("Images/" + name + "\n")

file.close()