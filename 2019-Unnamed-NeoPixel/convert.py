with open("main.cpp") as f:
    file_str = f.read()

# do stuff with file_str

with open("2019-Unnamed-NeoPixel.ino", "w") as f:
    f.write(file_str)
