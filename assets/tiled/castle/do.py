import os

def rename_to_lowercase(folder_path):
    # Get the list of files in the folder
    files = os.listdir(folder_path)

    # Create a list to store the new names
    new_names = []

    # Iterate through each file and generate the lowercase version of the name
    for file_name in files:
        # Get the lowercase version of the file name
        new_name = file_name.lower()
        new_names.append(new_name)

    # Iterate through the original files and rename them using the new names
    for i, file_name in enumerate(files):
        old_path = os.path.join(folder_path, file_name)
        new_path = os.path.join(folder_path, new_names[i])

        # Rename the file
        os.rename(old_path, new_path)
        print(f'Renamed: {file_name} to {new_names[i]}')

# Replace './' with the actual path to your folder
folder_path = './'

# Call the function to rename files to lowercase
rename_to_lowercase(folder_path)
