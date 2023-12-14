import os

def rename_files():
    for filename in os.listdir('.'):
        if '_' in filename:
            new_filename = filename.replace('_', '')
            os.rename(filename, new_filename)

rename_files()