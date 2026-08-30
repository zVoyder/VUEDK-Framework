def get_id(share_link):
    share_id = share_link.split("/d/")[1].split("/")[0]
    return share_id


def get_export_link(share_link):
    return f"https://drive.google.com/uc?id={get_id(share_link)}&export=download"


# Example usage
if __name__ == "__main__":
    share_link = "https://drive.google.com/file/d/18V52npD2oy2rCup9bCwkTV2zLbkdK2eg/view?usp=sharing"
    print(get_id(share_link))
