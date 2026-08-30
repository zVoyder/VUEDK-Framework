import os.path
import drive_downloader as dd
import gitignore_utility as git

KEYS_FILE_NAME = "keys.json"
KEYS_FILE_FULL_PATH = os.path.join(os.getcwd(), KEYS_FILE_NAME)
KEYS_LINK = "https://drive.google.com/file/d/131OXmVV-z_Id6-Yagrw-sLVOI_FByD79/view?usp=drive_link"


def get_keys_path():
    if os.path.exists(KEYS_FILE_FULL_PATH):
        return KEYS_FILE_FULL_PATH

    keys = dd.download_file_from_google_drive(KEYS_LINK, KEYS_FILE_FULL_PATH)
    git.add_to_gitignore(KEYS_FILE_NAME)
    return KEYS_FILE_FULL_PATH if keys else None


# Example usage
if __name__ == "__main__":
    keys = get_keys_path()
    print(keys)
