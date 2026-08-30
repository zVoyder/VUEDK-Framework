import requests
import google_id_helper as gih


def download_file_from_google_drive(share_link, destination_path):
    url = gih.get_export_link(share_link)
    response = requests.get(url, stream=True)
    content = response.content

    if response.status_code != 200:
        print(f"Error downloading file '{response.status_code}'")
        return None

    with open(destination_path, 'wb') as handler:
        handler.write(content)

    print(f"File '{url}' downloaded to '{destination_path}'")
    return content
