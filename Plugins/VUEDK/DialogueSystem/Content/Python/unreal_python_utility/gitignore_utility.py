import unreal
import os

GITIGNORE_PATH = unreal.Paths.project_dir() + "/.gitignore"


def add_to_gitignore(file_name):
    if not os.path.exists(GITIGNORE_PATH):
        with open(GITIGNORE_PATH, "w") as gitignore:
            gitignore.write(file_name + "\n")
    else:
        with open(GITIGNORE_PATH, "r+") as gitignore:
            lines = gitignore.readlines()
            if file_name not in [line.strip() for line in lines]:
                gitignore.write("\n" + file_name + "\n")


# Example usage
if __name__ == "__main__":
    add_to_gitignore("test.json")
