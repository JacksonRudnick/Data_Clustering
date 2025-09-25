file = open("datasets/iris_bezdek.txt", "r")
file2 = open("datasets/iris_bezdek_mod.txt", "w")

first_line = file.readline()

num_of_points = 420
num_of_dimensions = 4

file2.write(str(num_of_points) + " " + str(num_of_dimensions) + "\n")

lines = file.readlines()

i = 0

for line in lines:
    file2.write(line)
    if i % 5 == 0 and i != 0:
        file2.write(line)
        file2.write(line)
        file2.write(line)
        file2.write(line)
    i += 1