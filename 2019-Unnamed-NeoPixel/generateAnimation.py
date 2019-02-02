from PIL import Image

im = Image.open('earth.gif')
try:
    while True:
        new_frame = im.convert('RGBA')
        new_frame.save('foo%02d.png' % im.tell(), 'PNG')
        im.seek(im.tell()+1)
except EOFError:
    pass
