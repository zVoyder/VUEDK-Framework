import google_keys as gk
from googleapiclient.discovery import build
from googleapiclient.errors import HttpError
from google.oauth2 import service_account
import google_id_helper as gih

SCOPES = ["https://www.googleapis.com/auth/spreadsheets.readonly"]


def download_google_sheet(spreadsheet_link, spreadsheet_range, sheet_render_option="FORMATTED_VALUE"):
    try:
        keys_path = gk.get_keys_path()
        creds = service_account.Credentials.from_service_account_file(keys_path, scopes=SCOPES)

        service = build("sheets", "v4", credentials=creds)
        spreadsheet_id = gih.get_id(spreadsheet_link)

        # Call the Sheets API
        sheet = service.spreadsheets()
        result = (
            sheet.values().get(
                spreadsheetId=spreadsheet_id,
                range=spreadsheet_range,
                valueRenderOption=sheet_render_option
            ).execute()
        )

        values = result.get("values", [])
        return values

    except HttpError as err:
        print(err)
        return None


# Example usage
if __name__ == "__main__":
    spreadsheet_link = "https://docs.google.com/spreadsheets/d/1nN8Km63gRnSoTZWWYl-tjoJT9Z7neZRCeAx2fF5S0BQ/edit?gid=382241709#gid=382241709"
    spreadsheet_range = "DA_DialogueContainer_0a!A2:Z"
    values = download_google_sheet(spreadsheet_link, spreadsheet_range)

    if values:
        for row in values:
            print(row)
